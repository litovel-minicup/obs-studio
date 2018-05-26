//
// Created by sony on 24.5.18.
//

#ifndef OBS_STUDIO_STREAMMATCHINFOMANAGER_H
#define OBS_STUDIO_STREAMMATCHINFOMANAGER_H


#include <QObject>
#include "../match-connection/reconnectingsocket.h"

#ifdef MATCH_IM_EXPORT
#define MATCH_IM_DECL_EXPORT Q_DECL_EXPORT
#else
#define MATCH_IM_DECL_EXPORT Q_DECL_IMPORT
#endif


class MATCH_IM_DECL_EXPORT StreamMatchInfoManager: public QObject {
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
