#pragma once

#include <QWidget>
#include <QLineEdit>
#include <QGroupBox>
#include <QtWidgets/QPushButton>

class ControlPanel: public QWidget
{
    Q_OBJECT

    private:
	    QWidget * m_activeWidget = nullptr;
	    QGroupBox * m_connectionControls;
        QLineEdit* m_serverHostEdit;
        QPushButton* m_connectButton;

        QLineEdit* m_matchIdEdit;
        QPushButton* m_subscribeButton;

		QGroupBox* scoreBoardControls;
		QPushButton* m_showCompactScore;
		QPushButton* m_showFullScore;
		QPushButton* m_hideScore;

		QGroupBox* playersViewControls;
		QPushButton* m_showPlayers;
		QPushButton* m_hidePlayers;
		QPushButton* m_nextPagePlayers;

		QGroupBox* finalScoreControls;
		QPushButton* m_showFinalScore;
		QPushButton* m_hideFinalScore;

		QGroupBox* shooterControls;
		QPushButton* m_showShooter;
		QPushButton* m_hideShooter;

		QGroupBox* teamTilesControls;
		QPushButton* m_showTiles;
		QPushButton* m_hideTiles;

		QGroupBox* m_categoryTableControls;
		QPushButton* m_showCategoryTable;
		QPushButton* m_nextPageCategoryTable;
		QPushButton* m_hideCategoryTable;

		QGroupBox* m_socialMediasControls;
		QPushButton* m_showSocialMedias;
		QPushButton* m_hideSocialMedias;

		QGroupBox* m_nameDisplayControls;
		QPushButton* m_showNameDisplay;
		QPushButton* m_hideNameDisplay;

		QGroupBox* m_teamPlacementControls;
		QPushButton* m_showTeamPlacement;
		QPushButton* m_hideTeamPlacement;
		QPushButton* m_nextPageTeamPlacement;

		QGroupBox* m_subsControls;
		QPushButton* m_showSubs;
		QPushButton* m_hideSubs;

        bool m_connected;

    protected:
        void paintEvent(QPaintEvent*) override;
		void closeEvent(QCloseEvent*) override;

    public:
        ControlPanel(QWidget* parent = nullptr);
        QString serverHostName() const;

        QUrl serverUrl() const;

    private slots:
        bool validateServerHostName(const QString& hostName);

    public slots:
        void setConnected();
        void setDisconnected();

		void setScoreBoardControlsActive();
		void setFinalScoreControlsActive();
		void setTeamTilesControlsActive();
		void setPlayersControlsActive();
		void setCategoryTableControlsActive();
		void setSocialMediasControlsActive();
		void setNameDisplayControlsActive();
		void setTeamPlacementControlsActive();
		void setSubsControlsActive();

		void showControl();
		void hideControl();
		void show2Control();

    signals:
        void connectRequest(const QUrl& url);
        void serverHostNameChanged(const QUrl& url);
        void subscribeMatchRequest(int id);

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

		void showNameDisplayReq();
		void hideNameDisplayReq();

		void showTeamTilesReq();
		void hideTeamTilesReq();

		void showCategoryTableReq();
		void hideCategoryTableReq();
		void nextPageCategoryTableReq();

		void showTeamPlacementReq();
		void hideTeamPlacementReq();
		void nextPageTeamPlacementReq();

		void showSocialMediasReq();
		void hideSocialMediasReq();

		void showSubsReq();
		void hideSubsReq();
};
