//
// Created by thejoeejoee on 14.5.18.
//

#include <QtCore/QUrl>
#include "controlpanel.h"
#include <QVBoxLayout>

ControlPanel::ControlPanel(QWidget* parent): QWidget(parent) {
    m_serverHostEdit = new QLineEdit(this);
    m_serverHostEdit->setPlaceholderText("Server host name");
    m_serverHostEdit->setText("ws://localhost:8888/ws/broadcast");

    m_connectButton = new QPushButton{"Connect", this};

    auto layout = new QVBoxLayout(this);
    layout->addWidget(m_serverHostEdit);
    layout->addWidget(m_connectButton);

    layout->addSpacerItem(new QSpacerItem(
            10,
            10,
            QSizePolicy::Expanding,
            QSizePolicy::Expanding)
    );

    this->setLayout(layout);

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
