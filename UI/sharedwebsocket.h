#pragma once

#include <QObject>
#include <QWebSocket>

class SharedWebsocket: QObject
{
        Q_OBJECT
    private:
        QWebSocket m_socket;

    public:
        SharedWebsocket(QObject* parent = nullptr);

    private slots:
        void onConnected();

	public slots:
	    void open(const QUrl& url);
	    void loopedOpen(const QUrl& url, int time = 1);
	    void sendMsg(const QString& msg);

    signals:
	    void connected();
	    void disconnected();
	    void error(const QString& msg);
	    void msgReceived(const QString& msg);
};

