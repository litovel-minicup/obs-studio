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

    connect(this, &StreamMatchInfoManager::matchChanged,
	    this, &StreamMatchInfoManager::retrieveCategoryTable);

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
	bool startEvent = false, endEvent = false;

    const QVariantMap data = QJsonDocument::fromJson(msg.toUtf8()).toVariant().toMap();
    if (data["type_content"].toList().contains("event")) {
	    const QVariantMap eventData = data["event"].toMap();
	    if (eventData["type"] == "start")
		    startEvent = true;
	    else if (eventData["type"] == "end")
		    endEvent = true;
	    else if (eventData["type"] == "goal") {
		    goalEvent = true;
		    if (!eventData["team_id"].isNull()) {
			    const int teamId = eventData["team_id"].toInt();
			    for (const QString& key : QStringList{ "away", "home" }) {
				    if (teamId == m_nExtMatchData[QString("%1_team_id").arg(key)].toInt()) {
					    m_shooterData["team_name"] = m_nExtMatchData[QString("%1_team_name").arg(key)];
					    m_shooterData["team_color_primary"] = m_nExtMatchData[QString("%1_team_color_primary").arg(key)];
					    m_shooterData["team_color_secondary"] = m_nExtMatchData[QString("%1_team_color_secondary").arg(key)];
					    m_shooterData["team_color_text"] = m_nExtMatchData[QString("%1_team_color_text").arg(key)];
					    m_shooterData["team_slug"] = m_nExtMatchData[QString("%1_team_slug").arg(key)];
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

    if (data["type_content"].toList().contains("category_table")) {
	    m_categoryTableData = data["category_table"].toList();
    }

    if (this->retrievedAllTeamsPlayersData())
	    emit this->matchDataChanged(this->matchData());

    if (goalEvent)
	    emit this->showShooterReq();
    else if (startEvent)
	    emit this->matchStarted();
    else if (endEvent)
	    emit this->matchEnded();
}

QVariantMap StreamMatchInfoManager::matchData() const {
	QVariantMap data = m_nExtMatchData;
	data["players"] = m_teamsPlayers;
	data["last_shooter"] = m_shooterData;
	data["category_table"] = m_categoryTableData;
	return data;
}

bool StreamMatchInfoManager::hasAllData() const
{
	return m_matchId != -1 && this->retrievedAllTeamsPlayersData() && !m_categoryTableData.isEmpty();
}

void StreamMatchInfoManager::setLastMatchId(int id) {
	if (id == m_matchId)
		return;

	if (m_matchId != -1) {
		const QVariantMap data{
			{ "action", "unsubscribe" },
		{ "match", m_matchId }
		};
		m_teamsPlayers.clear();
		m_shooterData.clear();
		m_categoryTableData.clear();

		this->sendJsonData(data);
	}

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

void StreamMatchInfoManager::retrieveCategoryTable() {
	if (m_matchId == -1)
		return;

	const QVariantMap data{
		{ "action", "fetch_category_table" },
		{ "category", m_nExtMatchData["category_id"].toInt() }
	};

	this->sendJsonData(data);
}

void StreamMatchInfoManager::sendJsonData(const QVariant& data) {
	QJsonDocument jsonDoc = QJsonDocument::fromVariant(data);
	/*qDebug().noquote() << "Send"
		<< jsonDoc.toJson(QJsonDocument::Indented).data();*/

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