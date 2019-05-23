//
// Created by thejoeejoee on 14.5.18.
//

#include <QtCore/QUrl>
#include "controlpanel.h"
#include <QVBoxLayout>
#include <QtGui/QPainter>
#include <QCloseEvent>
#include <QDebug>


ControlPanel::ControlPanel(QWidget* parent): QWidget(parent) {
	m_connected = false;
	auto mainLayout = new QVBoxLayout(this);

	this->setWindowTitle("Control panel");
	this->setWindowFlags(Qt::WindowTitleHint | Qt::WindowMinimizeButtonHint);

	// CONNECTION control
	m_connectionControls = new QGroupBox{"Connection control"};
	QVBoxLayout* layout = new QVBoxLayout{ m_connectionControls };
    m_serverHostEdit = new QLineEdit(this);
    m_serverHostEdit->setPlaceholderText("Server host name");
    m_serverHostEdit->setText("wss://live.minicup.tatranlitovel.cz/ws/live");

    m_connectButton = new QPushButton{"Connect", this};

    m_matchIdEdit = new QLineEdit{this};
    m_matchIdEdit->setPlaceholderText("Id of match");
    m_subscribeButton = new QPushButton{"Subscribe", this};

    layout->addWidget(m_serverHostEdit);
    layout->addWidget(m_connectButton);
    layout->addWidget(m_matchIdEdit);
    layout->addWidget(m_subscribeButton);
    mainLayout->addWidget(m_connectionControls);

	// SCOREBOARD control
    scoreBoardControls = new QGroupBox("Scoreboard control");
    //m_showCompactScore = new QPushButton("Show Compact");
    m_showFullScore = new QPushButton("Show Full");
    m_hideScore = new QPushButton("Hide");

    layout = new QVBoxLayout{scoreBoardControls};
    //layout->addWidget(m_showCompactScore);
    layout->addWidget(m_showFullScore);
    layout->addWidget(m_hideScore);
    mainLayout->addWidget(scoreBoardControls);

	// PLAYERS control
    playersViewControls = new QGroupBox{"Players view control"};
    m_showPlayers = new QPushButton{ "Show" };
    m_hidePlayers = new QPushButton{ "Hide" };
    m_nextPagePlayers = new QPushButton{ "Next page" };

    layout = new QVBoxLayout(playersViewControls);
    layout->addWidget(m_showPlayers);
    layout->addWidget(m_nextPagePlayers);
    layout->addWidget(m_hidePlayers);
    mainLayout->addWidget(playersViewControls);

	// FINAL SCORE control
    finalScoreControls = new QGroupBox{"Final score control"};
    m_showFinalScore = new QPushButton{ "Show" };
    m_hideFinalScore = new QPushButton{ "Hide" };

    layout = new QVBoxLayout(finalScoreControls);
    layout->addWidget(m_showFinalScore);
    layout->addWidget(m_hideFinalScore);
    layout->addStretch(1);

    mainLayout->addWidget(finalScoreControls);

    // TEAM TILES control
    teamTilesControls = new QGroupBox{ "Team tiles control" };
    m_showTiles = new QPushButton{ "Show" };
    m_hideTiles = new QPushButton{ "Hide" };

    layout = new QVBoxLayout(teamTilesControls);
    layout->addWidget(m_showTiles);
    layout->addWidget(m_hideTiles);
    mainLayout->addWidget(teamTilesControls);

    // CATEGORY TABLE
    m_categoryTableControls = new QGroupBox{ "Category table control" };
    m_showCategoryTable = new QPushButton{ "Show" };
    m_hideCategoryTable = new QPushButton{ "Hide" };
    m_nextPageCategoryTable = new QPushButton{ "Next page" };

    layout = new QVBoxLayout(m_categoryTableControls);
    layout->addWidget(m_showCategoryTable);
    layout->addWidget(m_nextPageCategoryTable);
    layout->addWidget(m_hideCategoryTable);
    mainLayout->addWidget(m_categoryTableControls);

    // SOCIAL MEDIAS
    m_socialMediasControls = new QGroupBox{ "Social medias control" };
    m_showSocialMedias = new QPushButton{ "Show" };
    m_hideSocialMedias = new QPushButton{ "Hide" };

    layout = new QVBoxLayout{ m_socialMediasControls };
    layout->addWidget(m_showSocialMedias);
    layout->addWidget(m_hideSocialMedias);
    mainLayout->addWidget(m_socialMediasControls);

    // SHOOTER control
    shooterControls = new QGroupBox{ "Shooter control" };
    m_showShooter = new QPushButton{ "Show" };
    m_hideShooter = new QPushButton{ "Hide" };

    layout = new QVBoxLayout{ shooterControls };
    layout->addWidget(m_showShooter);
    layout->addWidget(m_hideShooter);
    mainLayout->addWidget(shooterControls);

    mainLayout->setSpacing(20);
    mainLayout->addSpacerItem(new QSpacerItem(
	    10,
	    10,
	    QSizePolicy::Expanding,
	    QSizePolicy::Expanding)
    );

    this->setLayout(mainLayout);

    //connect(m_showCompactScore, &QPushButton::clicked, this, &ControlPanel::showCompactScoreBoardReq);
    connect(m_showFullScore, &QPushButton::clicked,  this, &ControlPanel::showFullScoreBoardReq);
    connect(m_hideScore, &QPushButton::clicked, this, &ControlPanel::hideScoreBoardReq);

    connect(m_showPlayers, &QPushButton::clicked, this, &ControlPanel::showPlayersReq);
    connect(m_nextPagePlayers, &QPushButton::clicked, this, &ControlPanel::nextPagePlayersReq);
    connect(m_hidePlayers, &QPushButton::clicked, this, &ControlPanel::hidePlayersReq);

    connect(m_showFinalScore, &QPushButton::clicked, this, &ControlPanel::showFinalScoreReq);
    connect(m_hideFinalScore, &QPushButton::clicked, this, &ControlPanel::hideFinalScoreReq);

    connect(m_showShooter, &QPushButton::clicked, this, &ControlPanel::showShooterReq);
    connect(m_hideShooter, &QPushButton::clicked, this, &ControlPanel::hideShooterReq);

    connect(m_showTiles, &QPushButton::clicked, this, &ControlPanel::showTeamTilesReq);
    connect(m_hideTiles, &QPushButton::clicked, this, &ControlPanel::hideTeamTilesReq);

    connect(m_showCategoryTable, &QPushButton::clicked, this, &ControlPanel::showCategoryTableReq);
    connect(m_hideCategoryTable, &QPushButton::clicked, this, &ControlPanel::hideCategoryTableReq);
    connect(m_nextPageCategoryTable, &QPushButton::clicked, this, &ControlPanel::nextPageCategoryTableReq);

    connect(m_showSocialMedias, &QPushButton::clicked, this, &ControlPanel::showSocialMediasReq);
    connect(m_hideSocialMedias, &QPushButton::clicked, this, &ControlPanel::hideSocialMediasReq);

    connect(m_subscribeButton, &QPushButton::clicked, [this]() {
        emit this->subscribeMatchRequest(m_matchIdEdit->text().toInt());
    });

    connect(m_connectButton, &QPushButton::clicked, [this]() {
        emit this->connectRequest(this->serverUrl());
    });
    connect(m_serverHostEdit, &QLineEdit::textChanged, [this](const QString& text) {
        if(this->validateServerHostName(text)) {
            m_serverHostEdit->setStyleSheet("color: #000000");
            emit this->serverHostNameChanged(this->serverUrl());
        }

        else
            m_serverHostEdit->setStyleSheet("color: #FF0000");
    });
}

bool ControlPanel::validateServerHostName(const QString &hostName) {
    if(hostName.isEmpty())
        return false;
    return this->serverUrl().isValid();
}

QString ControlPanel::serverHostName() const {
    return m_serverHostEdit->text();
}

QUrl ControlPanel::serverUrl() const {
    return QUrl(m_serverHostEdit->text(), QUrl::StrictMode);
}

void ControlPanel::paintEvent(QPaintEvent*) {
	if (m_connectionControls == nullptr)
		return;

    QPainter painter(this);
    painter.setPen(QColor(Qt::transparent));
    painter.setBrush((m_connected) ? QColor("#41c1fc") :QColor(Qt::red));
    painter.drawRect(QRect(0, m_connectionControls->y(), this->rect().width(), m_connectionControls->height()));

    if (m_activeWidget != nullptr) {
	    painter.setBrush(QColor("orange"));
	    painter.drawRect(QRect(0, m_activeWidget->y(), this->rect().width(), m_activeWidget->height()));
	}
}
void ControlPanel::closeEvent(QCloseEvent *e)
{
	e->ignore();
}
void ControlPanel::setConnected() {
    if(m_connected)
        return;

    m_connected = true;
    this->update();
}
void ControlPanel::setDisconnected() {
    if(!m_connected)
        return;

    m_connected = false;
    this->update();
}

void ControlPanel::setScoreBoardControlsActive()
{
	m_activeWidget = scoreBoardControls;
	this->update();
}

void ControlPanel::setFinalScoreControlsActive()
{
	m_activeWidget = finalScoreControls;
	this->update();
}

void ControlPanel::setPlayersControlsActive()
{
	m_activeWidget = playersViewControls;
	this->update();
}

void ControlPanel::setTeamTilesControlsActive()
{
	m_activeWidget = teamTilesControls;
	this->update();
}

void ControlPanel::setCategoryTableControlsActive()
{
	m_activeWidget = m_categoryTableControls;
	this->update();
}

void ControlPanel::setSocialMediasControlsActive()
{
	m_activeWidget = m_socialMediasControls;
	this->update();
}

void ControlPanel::show2Control()
{
	if (m_activeWidget == scoreBoardControls)
		emit this->showCompactScoreBoardReq();
	else if (m_activeWidget == m_categoryTableControls)
		emit this->nextPageCategoryTableReq();
	else if (m_activeWidget == playersViewControls)
		emit this->nextPagePlayersReq();
}

void ControlPanel::showControl()
{
	if (m_activeWidget == scoreBoardControls)
		emit this->showFullScoreBoardReq();
	else if (m_activeWidget == playersViewControls)
		emit this->showPlayersReq();
	else if (m_activeWidget == finalScoreControls)
		emit this->showFinalScoreReq();
	else if (m_activeWidget == teamTilesControls)
		emit this->showTeamTilesReq();
	else if (m_activeWidget == m_categoryTableControls)
		emit this->showCategoryTableReq();
	else if (m_activeWidget == m_socialMediasControls)
		emit this->showSocialMediasReq();
}

void ControlPanel::hideControl()
{
	if (m_activeWidget == scoreBoardControls)
		emit this->hideScoreBoardReq();
	else if (m_activeWidget == playersViewControls)
		emit this->hidePlayersReq();
	else if (m_activeWidget == finalScoreControls)
		emit this->hideFinalScoreReq();
	else if (m_activeWidget == teamTilesControls)
		emit this->hideTeamTilesReq();
	else if (m_activeWidget == m_categoryTableControls)
		emit this->hideCategoryTableReq();
	else if (m_activeWidget == m_socialMediasControls)
		emit this->hideSocialMediasReq();
}
