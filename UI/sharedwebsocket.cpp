#include "sharedwebsocket.h"
#include <chrono>

SharedWebsocket::SharedWebsocket(QObject * parent): QObject(parent)
{
    using sslErrorSignal = void(QWebSocket::*)(const QList<QSslError>&);

    connect(&m_reconnectTimer, &QTimer::timeout, this, &SharedWebsocket::reconnect);
	connect(&m_socket, &QWebSocket::connected, this, &SharedWebsocket::onConnected);
	connect(&m_socket, &QWebSocket::disconnected,
            this, &SharedWebsocket::onDisconnected);
	connect(&m_socket, static_cast<sslErrorSignal>(&QWebSocket::sslErrors),
            this, &SharedWebsocket::onSslError);
}

void SharedWebsocket::sendMsg(const QString & msg) {
    if(m_socket.state() == QAbstractSocket::ConnectedState)
        m_socket.sendTextMessage(msg);
    else
        qWarning() << "trying to send message without connection";
}

void SharedWebsocket::onConnected() {
    qInfo() << "Connected to" << m_url;
    m_reconnectTimer.stop();
    this->setStatus(SharedWebsocket::Open);

    connect(&m_socket, &QWebSocket::textMessageReceived, this,
            &SharedWebsocket::msgReceived);
}

void SharedWebsocket::onSslError(const QList<QSslError> &errors) {
    QString repr;

    for(const QSslError& singleError: errors) {
        repr += singleError.errorString() + ", ";
    }

    emit this->error(repr);
}

void SharedWebsocket::setStatus(SharedWebsocket::Statuses status) {
    if(m_status == status)
        return;

    m_status = status;
    emit this->statusChanged(static_cast<int>(status));
}

int SharedWebsocket::status() const {
    return static_cast<int>(m_status);
}

QUrl SharedWebsocket::url() const {
    return m_url;
}

void SharedWebsocket::setUrl(const QUrl &url) {
    if(url == m_url)
        return;
    m_url = url;
    emit this->urlChanged(url);
}

void SharedWebsocket::open(std::chrono::milliseconds reconnectTime) {
    if(reconnectTime == std::chrono::milliseconds(0)) {
        m_socket.open(m_url);
        return;
    }

    m_reconnectTimer.stop();
    if(m_socket.state() == QAbstractSocket::ConnectedState)
        m_socket.close();

    m_socket.open(m_url);
    m_reconnectTimer.setInterval(reconnectTime);
    m_reconnectTimer.start();
}

void SharedWebsocket::onDisconnected() {
    qInfo() << "Disconnected";
    m_reconnectTimer.start();
    this->setStatus(SharedWebsocket::Closed);
}

void SharedWebsocket::stopReconnecting() {
    m_reconnectTimer.stop();
}

void SharedWebsocket::setReconnectInterval(std::chrono::milliseconds time) {
    m_reconnectTimer.setInterval(time);
}

void SharedWebsocket::open() {
    this->open(m_reconnectTimer.intervalAsDuration());
}
void SharedWebsocket::open(const QUrl &url) {
    this->setUrl(url);
    this->open(m_reconnectTimer.intervalAsDuration());
}

void SharedWebsocket::reconnect() {
    qInfo() << "Reconnecting...";
    if(m_socket.state() != QAbstractSocket::ConnectedState)
        m_socket.open(m_url);
    else
        m_reconnectTimer.stop();
}
