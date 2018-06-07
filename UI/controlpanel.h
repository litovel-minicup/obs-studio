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

		QGroupBox* finalScoreControls;
		QPushButton* m_showFinalScore;
		QPushButton* m_hideFinalScore;

		QGroupBox* shooterControls;
		QPushButton* m_showShooter;
		QPushButton* m_hideShooter;

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
		void setPlayersControlsActive();

		void showControl();
		void hideControl();

    signals:
        void connectRequest(const QUrl& url);
        void serverHostNameChanged(const QUrl& url);
        void subscribeMatchRequest(int id);

		void showCompactScoreBoardReq();
		void showFullScoreBoardReq();
		void hideScoreBoardReq();

		void showPlayersReq();
		void hidePlayersReq();

		void showFinalScoreReq();
		void hideFinalScoreReq();

		void showShooterReq();
		void hideShooterReq();
};
