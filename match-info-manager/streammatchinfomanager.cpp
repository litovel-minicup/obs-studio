//
// Created by sony on 24.5.18.
//

#include "streammatchinfomanager.h"
#include <QJsonDocument>
#include <iostream>

StreamMatchInfoManager* StreamMatchInfoManager::m_instance = nullptr;

StreamMatchInfoManager::StreamMatchInfoManager(QObject *parent) : QObject(parent) {
    m_socket = ThreadedSharedWebsocketWrapper::instance();

    connect(this, &StreamMatchInfoManager::matchChanged, 
	    this, &StreamMatchInfoManager::retrieveTeamPlayers);

    connect(m_socket, &ThreadedSharedWebsocketWrapper::msgReceived,
	    this, &StreamMatchInfoManager::handlleMsg);
}

StreamMatchInfoManager *StreamMatchInfoManager::instance() {
    if(StreamMatchInfoManager::m_instance == nullptr)
        StreamMatchInfoManager::m_instance = new StreamMatchInfoManager;
    return StreamMatchInfoManager::m_instance;
}

void StreamMatchInfoManager::subscribeMatch(int id) {
	const QVariantMap data {
		{"action", "subscribe"},
	    {"match", id}
	};

    this->sendJsonData(data);
}

void StreamMatchInfoManager::handlleMsg(const QString &msg) {
    /*qDebug().noquote() << "Received"
                      << QJsonDocument::fromJson(msg.toUtf8())
                                      .toJson(QJsonDocument::Indented).data();*/
	bool goalEvent = false;

    const QVariantMap data = QJsonDocument::fromJson(msg.toUtf8()).toVariant().toMap();
    if (data["type_content"].toList().contains("event")) {
	    const QVariantMap eventData = data["event"].toMap();
	    if (eventData["type"] == "goal") {
		    goalEvent = true;
		    if (!eventData["team_id"].isNull()) {
			    const int teamId = eventData["team_id"].toInt();
			    for (const QString& key : QStringList{ "away", "home" }) {
				    if (teamId == m_nExtMatchData[QString("%1_team_id").arg(key)].toInt()) {
					    m_shooterData["team_name"] = m_nExtMatchData[QString("%1_team_name").arg(key)];
					    break;
				    }
			    }
		    }

			m_shooterData["player_name"] = eventData["player_name"];
		    m_shooterData["player_number"] = eventData["player_number"];
	    }
	}

    if (data["type_content"].toList().contains("match")) {
	    m_nExtMatchData = data["match"].toMap();
		// calculate time diff
	    m_nExtMatchData["time_diff"] = static_cast<double>(QDateTime::currentSecsSinceEpoch()) 
		    - data["server_time"].toDouble();

	    this->setLastMatchId(m_nExtMatchData["id"].toInt());
    }

    if (data["type_content"].toList().contains("team_players")) {
	    this->setTeamsPlayersData(data);
    }

    if (this->retrievedAllTeamsPlayersData())
	    emit this->matchDataChanged(this->matchData());

	if(goalEvent)
		emit this->showShooterReq();
}

QVariantMap StreamMatchInfoManager::matchData() const {
	QVariantMap data = m_nExtMatchData;
	data["players"] = m_teamsPlayers;
	data["last_shooter"] = m_shooterData;
	return data;
}

bool StreamMatchInfoManager::hasAllData() const
{
	return m_matchId != -1 && this->retrievedAllTeamsPlayersData();
}

void StreamMatchInfoManager::setLastMatchId(int id) {
	if (id == m_matchId)
		return;
	m_matchId = id;
	emit this->matchChanged();
}

void StreamMatchInfoManager::retrieveTeamPlayers() {
	if (m_matchId == -1)
		return;

	for (const QString& team : { "home", "away" }) {
		const QVariantMap dataTeam{
			{ "action", "fetch_team_roster" },
		    { "team", m_nExtMatchData[QString("%1_team_id").arg(team)].toInt() }
		};

		this->sendJsonData(dataTeam);
	}
}

void StreamMatchInfoManager::sendJsonData(const QVariant& data) {
	QJsonDocument jsonDoc = QJsonDocument::fromVariant(data);
	m_socket->sendMsg(QString::fromUtf8(jsonDoc.toJson()));
}

bool StreamMatchInfoManager::retrievedAllTeamsPlayersData() const {
	if (!m_teamsPlayers.contains("away") || !m_teamsPlayers.contains("home"))
		return false;

	QSet<int> currentTeamsIds;
	QSet<int> updatedTeamsIds;

	for (const QString& key : QStringList{ "away", "home" }) {
		currentTeamsIds.insert(m_teamsPlayers[key].toMap()["team"].toInt());
		updatedTeamsIds.insert(m_nExtMatchData[QString("%1_team_id").arg(key)].toInt());
	}

	if ((currentTeamsIds & updatedTeamsIds).size() != 2)
		return false;

	return true;
}

void StreamMatchInfoManager::setTeamsPlayersData(const QVariantMap& data) {
	const int teamId = data["team"].toInt();
	for (const QString& key : QStringList{ "away", "home" }) {
		if (teamId == m_nExtMatchData[QString("%1_team_id").arg(key)].toInt()) {
			m_teamsPlayers[key] = data;
			return;
		}
	}
}