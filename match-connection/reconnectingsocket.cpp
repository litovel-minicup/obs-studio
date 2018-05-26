//
// Created by Sony on 25.05.2018.
//

#include "reconnectingsocket.h"
#include <chrono>

ThreadedSharedWebsocketWrapper* ThreadedSharedWebsocketWrapper::m_instance = nullptr;

ReconnectingWebsocket::ReconnectingWebsocket(QObject * parent) : QObject(parent)
{
	using sslErrorSignal = void(QWebSocket::*)(const QList<QSslError>&);

	connect(&m_reconnectTimer, &QTimer::timeout, this, &ReconnectingWebsocket::reconnect);
	connect(&m_socket, &QWebSocket::connected, this, &ReconnectingWebsocket::connected);
	connect(&m_socket, &QWebSocket::disconnected,
		this, &ReconnectingWebsocket::disconnected);
	connect(&m_socket, &QWebSocket::connected, this, &ReconnectingWebsocket::onConnected);
	connect(&m_socket, &QWebSocket::disconnected,
		this, &ReconnectingWebsocket::onDisconnected);
	connect(&m_socket, static_cast<sslErrorSignal>(&QWebSocket::sslErrors),
		this, &ReconnectingWebsocket::onSslError);
}

void ReconnectingWebsocket::sendMsg(const QString & msg) {
	if (m_socket.state() == QAbstractSocket::ConnectedState)
		m_socket.sendTextMessage(msg);
	else
		qWarning() << "trying to send message without connection";
}

void ReconnectingWebsocket::onConnected() {
	qInfo() << "Connected to" << m_url;
	m_reconnectTimer.stop();

	connect(&m_socket, &QWebSocket::textMessageReceived, this,
		&ReconnectingWebsocket::msgReceived);
}

void ReconnectingWebsocket::onSslError(const QList<QSslError> &errors) {
	QString repr;

	for (const QSslError& singleError : errors) {
		repr += singleError.errorString() + ", ";
	}

	emit this->error(repr);
}

QUrl ReconnectingWebsocket::url() const {
	return m_url;
}

void ReconnectingWebsocket::setUrl(const QUrl &url) {
	if (url == m_url)
		return;
	m_url = url;
	emit this->urlChanged(url);
}

void ReconnectingWebsocket::open(std::chrono::milliseconds reconnectTime) {
	if (reconnectTime == std::chrono::milliseconds(0)) {
		m_socket.open(m_url);
		return;
	}

	m_reconnectTimer.stop();
	if (m_socket.state() == QAbstractSocket::ConnectedState)
		m_socket.close();

	m_socket.open(m_url);
	m_reconnectTimer.setInterval(reconnectTime);
	m_reconnectTimer.start();
}

void ReconnectingWebsocket::onDisconnected() {
	qInfo() << "Disconnected";
	m_reconnectTimer.start();
}

void ReconnectingWebsocket::setReconnectInterval(std::chrono::milliseconds time) {
	m_reconnectTimer.setInterval(time);
}

void ReconnectingWebsocket::open() {
	this->open(m_reconnectTimer.intervalAsDuration());
}
void ReconnectingWebsocket::open(const QUrl &url) {
	this->setUrl(url);
	this->open(m_reconnectTimer.intervalAsDuration());
}

void ReconnectingWebsocket::reconnect() {
	qInfo() << "Reconnecting..." << m_url;
	if (m_socket.state() != QAbstractSocket::ConnectedState)
		m_socket.open(m_url);
	else
		m_reconnectTimer.stop();
}

ThreadedSharedWebsocket::ThreadedSharedWebsocket(QObject *parent) : QThread(parent) {
	m_socket = new ReconnectingWebsocket{ this };
}

void ThreadedSharedWebsocket::run() {
	this->exec();
}

void ThreadedSharedWebsocket::bindWrapper(ThreadedSharedWebsocketWrapper *wrapper) {
	connect(m_socket, &ReconnectingWebsocket::connected,
		wrapper, &ThreadedSharedWebsocketWrapper::connected);

	connect(m_socket, &ReconnectingWebsocket::disconnected,
		wrapper, &ThreadedSharedWebsocketWrapper::disconnected);

	connect(m_socket, &ReconnectingWebsocket::error,
		wrapper, &ThreadedSharedWebsocketWrapper::error);

	connect(m_socket, &ReconnectingWebsocket::msgReceived,
		wrapper, &ThreadedSharedWebsocketWrapper::msgReceived);

	connect(wrapper, &ThreadedSharedWebsocketWrapper::sendMsg,
		m_socket, &ReconnectingWebsocket::sendMsg);

	connect(wrapper, &ThreadedSharedWebsocketWrapper::open,
		m_socket,
		static_cast<void(ReconnectingWebsocket::*)(const QUrl&)>
		(&ReconnectingWebsocket::open));

	connect(wrapper, &ThreadedSharedWebsocketWrapper::setReconnectInterval,
		m_socket, &ReconnectingWebsocket::setReconnectInterval);

}

ThreadedSharedWebsocketWrapper::ThreadedSharedWebsocketWrapper(QObject * parent) : QObject{ parent }
{
	QObject::connect(this, &ThreadedSharedWebsocketWrapper::connected, [this]() {
		m_connected = true;
	});
}

ThreadedSharedWebsocketWrapper *ThreadedSharedWebsocketWrapper::instance() {
	if (ThreadedSharedWebsocketWrapper::m_instance == nullptr)
		ThreadedSharedWebsocketWrapper::m_instance = new ThreadedSharedWebsocketWrapper();
	return ThreadedSharedWebsocketWrapper::m_instance;
}

bool ThreadedSharedWebsocketWrapper::isConnected() const
{
	return m_connected;
}
