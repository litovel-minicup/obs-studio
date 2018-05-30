//
// Created by thejoeejoee on 14.5.18.
//

#include <QtCore/QUrl>
#include "controlpanel.h"
#include <QVBoxLayout>
#include <QtGui/QPainter>

ControlPanel::ControlPanel(QWidget* parent): QWidget(parent) {
	m_connected = false;
	auto mainLayout = new QVBoxLayout(this);

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
    auto scoreBoardControls = new QGroupBox("Scoreboard control");
    m_showCompactScore = new QPushButton("Show Compact");
    m_showFullScore = new QPushButton("Show Full");
    m_hideScore = new QPushButton("Hide");

    layout = new QVBoxLayout{scoreBoardControls};
    layout->addWidget(m_showCompactScore);
    layout->addWidget(m_showFullScore);
    layout->addWidget(m_hideScore);
    mainLayout->addWidget(scoreBoardControls);

	// PLAYERS control
    auto playersViewControls = new QGroupBox{"Players view control"};
    m_showPlayers = new QPushButton{ "Show" };
    m_hidePlayers = new QPushButton{ "Hide" };

    layout = new QVBoxLayout(playersViewControls);
    layout->addWidget(m_showPlayers);
    layout->addWidget(m_hidePlayers);
    mainLayout->addWidget(playersViewControls);

	// FINAL SCORE control
    auto finalScoreControls = new QGroupBox{"Final score control"};
    m_showFinalScore = new QPushButton{ "Show" };
    m_hideFinalScore = new QPushButton{ "Hide" };

    layout = new QVBoxLayout(finalScoreControls);
    layout->addWidget(m_showFinalScore);
    layout->addWidget(m_hideFinalScore);
    layout->addStretch(1);

    mainLayout->addWidget(finalScoreControls);

    mainLayout->setSpacing(20);
    mainLayout->addSpacerItem(new QSpacerItem(
	    10,
	    10,
	    QSizePolicy::Expanding,
	    QSizePolicy::Expanding)
    );

    this->setLayout(mainLayout);

    connect(m_showCompactScore, &QPushButton::clicked, this, &ControlPanel::showCompactScoreBoardReq);
    connect(m_showFullScore, &QPushButton::clicked,  this, &ControlPanel::showFullScoreBoardReq);
    connect(m_hideScore, &QPushButton::clicked, this, &ControlPanel::hideScoreBoardReq);

    connect(m_showPlayers, &QPushButton::clicked, this, &ControlPanel::showPlayersReq);
    connect(m_hidePlayers, &QPushButton::clicked, this, &ControlPanel::hidePlayersReq);

    connect(m_showFinalScore, &QPushButton::clicked, this, &ControlPanel::showFinalScoreReq);
    connect(m_hideFinalScore, &QPushButton::clicked, this, &ControlPanel::hideFinalScoreReq);

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
