#include "sharedwebsocket.h"

SharedWebsocket::SharedWebsocket(QObject * parent): QObject(parent)
{
	connect(&m_socket, &QWebSocket::connected, this, &SharedWebsocket::onConnected);
}

void SharedWebsocket::openSocket(const QUrl& url) {

}

void SharedWebsocket::loopedOpenSocket(const QUrl & url, int time)
{

}

void SharedWebsocket::sendMsg(const QString & msg)
{
}

void SharedWebsocket::onConnected()
{

}