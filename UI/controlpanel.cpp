//
// Created by thejoeejoee on 14.5.18.
//

#include <QtCore/QUrl>
#include "controlpanel.h"
#include <QVBoxLayout>
#include <QtGui/QPainter>

ControlPanel::ControlPanel(QWidget* parent): QWidget(parent) {
    m_serverHostEdit = new QLineEdit(this);
    m_connected = false;
    m_serverHostEdit->setPlaceholderText("Server host name");
    m_serverHostEdit->setText("wss://live.minicup.tatranlitovel.cz/ws/live");

    m_connectButton = new QPushButton{"Connect", this};

    m_matchIdEdit = new QLineEdit{this};
    m_matchIdEdit->setPlaceholderText("Id of match");
    m_subscribeButton = new QPushButton{"Subscribe", this};

    auto layout = new QVBoxLayout(this);
    layout->addWidget(m_serverHostEdit);
    layout->addWidget(m_connectButton);
    layout->addWidget(m_matchIdEdit);
    layout->addWidget(m_subscribeButton);

    layout->addSpacerItem(new QSpacerItem(
            10,
            10,
            QSizePolicy::Expanding,
            QSizePolicy::Expanding)
    );

    this->setLayout(layout);

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
    QPainter painter(this);
    painter.setPen(QColor(Qt::transparent));
    painter.setBrush((m_connected) ? QColor("#41c1fc") :QColor(Qt::red));

    painter.drawRect(QRect(0, 0, this->rect().width(), 70));
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
