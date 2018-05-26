//
// Created by sony on 24.5.18.
//

#include "streammatchinfomanager.h"
#include <QJsonDocument>
#include <iostream>

StreamMatchInfoManager* StreamMatchInfoManager::m_instance = nullptr;

StreamMatchInfoManager::StreamMatchInfoManager(QObject *parent) : QObject(parent) {
    m_socket = ThreadedSharedWebsocketWrapper::instance();

    connect(m_socket, &ThreadedSharedWebsocketWrapper::msgReceived,
            this, &StreamMatchInfoManager::handlleMsg);
}

StreamMatchInfoManager *StreamMatchInfoManager::instance() {
    if(StreamMatchInfoManager::m_instance == nullptr)
        StreamMatchInfoManager::m_instance = new StreamMatchInfoManager;
    return StreamMatchInfoManager::m_instance;
}

void StreamMatchInfoManager::subscribeMatch(int id) {
    QVariantMap data;
    data["action"] = "subscribe";
    data["match"] = id;

    QJsonDocument jsonDoc = QJsonDocument::fromVariant(data);
    m_socket->sendMsg(QString::fromUtf8(jsonDoc.toJson()));
}

void StreamMatchInfoManager::handlleMsg(const QString &msg) {
    qDebug().noquote() << "Received"
                      << QJsonDocument::fromJson(msg.toUtf8())
                                      .toJson(QJsonDocument::Indented).data();

    const QVariantMap data = QJsonDocument::fromJson(msg.toUtf8()).toVariant().toMap();
    if(!data.keys().contains("match"))
        return;

    this->setMatchData(data["match"].toMap());

}

QVariantMap StreamMatchInfoManager::matchData() const {
    return m_matchData;
}

void StreamMatchInfoManager::setMatchData(const QVariantMap &data) {
    qDebug() << data;
    if(data == m_matchData)
        return;
    m_matchData = data;
    emit this->matchDataChanged(data);
}
