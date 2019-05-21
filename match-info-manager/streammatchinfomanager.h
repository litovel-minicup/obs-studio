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
	    QVariantMap m_nExtMatchData;
	    QVariantMap m_teamsPlayers;
	    QVariantMap m_shooterData;
	    QVariantList m_categoryTableData;
	    int m_matchId = -1;

        explicit StreamMatchInfoManager(QObject* parent = nullptr);

    public:
        static StreamMatchInfoManager* instance();

    private:
	    bool retrievedAllTeamsPlayersData() const;

    public:
        QVariantMap matchData() const;
	    Q_INVOKABLE bool hasAllData() const;

    private slots:
	    void retrieveCategoryTable();
        void retrieveTeamPlayers();
        void setLastMatchId(int id);
        void handlleMsg(const QString& msg);
	    void sendJsonData(const QVariant& data);
	    void setTeamsPlayersData(const QVariantMap& data);

    public slots:
        void subscribeMatch(int id);

    signals:
        void matchDataChanged(const QVariantMap& matchData);
	    void matchChanged();

	    void showCompactScoreBoardReq();
	    void showFullScoreBoardReq();
	    void hideScoreBoardReq();

	    void showPlayersReq();
	    void hidePlayersReq();
	    void nextPagePlayersReq();

	    void showFinalScoreReq();
	    void hideFinalScoreReq();

	    void showShooterReq();
	    void hideShooterReq();

	    void showTeamTilesReq();
	    void hideTeamTilesReq();

	    void showCategoryTableReq();
	    void hideCategoryTableReq();
	    void nextPageCategoryTableReq();
};


#endif //OBS_STUDIO_STREAMMATCHINFOMANAGER_H
