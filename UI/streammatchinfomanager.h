//
// Created by sony on 24.5.18.
//

#ifndef OBS_STUDIO_STREAMMATCHINFOMANAGER_H
#define OBS_STUDIO_STREAMMATCHINFOMANAGER_H


#include <QtCore/QObject>
#include "sharedwebsocket.h"

class StreamMatchInfoManager: public QObject {
    Q_OBJECT
    Q_PROPERTY(QVariantMap matchData READ matchData NOTIFY matchDataChanged)

    private:
        static StreamMatchInfoManager* m_instance;
        ThreadedSharedWebsocketWrapper* m_socket;
        QVariantMap m_matchData;

        explicit StreamMatchInfoManager(QObject* parent = nullptr);

    public:
        static StreamMatchInfoManager* instance();

        QVariantMap matchData() const;

    private slots:
        void setMatchData(const QVariantMap& data);
        void handlleMsg(const QString& msg);

    public slots:
        void subscribeMatch(int id);
    signals:
        void matchDataChanged(const QVariantMap& matchData);
};


#endif //OBS_STUDIO_STREAMMATCHINFOMANAGER_H
