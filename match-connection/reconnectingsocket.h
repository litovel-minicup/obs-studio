//
// Created by Sony on 25.05.2018.
//

#ifndef OBS_STUDIO_RECONNECTINGSOCKET_H
#define OBS_STUDIO_RECONNECTINGSOCKET_H

#include <QObject>
#include <QTimer>
#include <chrono>
#include <QtWebSockets/QWebSocket>
#include <QThread>
#include "matchconnectionglobal.h"


class MATCH_CONN_DECL_EXPORT ReconnectingWebsocket : public QObject
{
	Q_OBJECT
		Q_PROPERTY(QUrl url READ url WRITE setUrl NOTIFY urlChanged)

    private:
	    QWebSocket m_socket;
	    QTimer m_reconnectTimer;
	    QUrl m_url;

    public:
	    explicit ReconnectingWebsocket(QObject* parent = nullptr);

	    QUrl url() const;

	    private slots:
	    void reconnect();
	    void onConnected();
	    void onDisconnected();
	    void onSslError(const QList<QSslError>& errors);

	public slots:
	    void setReconnectInterval(std::chrono::milliseconds time);
	    void open(std::chrono::milliseconds reconnectTime);
	    void open();
	    void open(const QUrl& url);
	    void sendMsg(const QString& msg);
	    void setUrl(const QUrl& url);

    signals:
	    void connected();
	    void disconnected();
	    void error(const QString& msg);
	    void msgReceived(const QString& msg);
	    void urlChanged(const QUrl& url);
};

class MATCH_CONN_DECL_EXPORT ThreadedSharedWebsocketWrapper : public QObject
{
	Q_OBJECT

    private:
	    static ThreadedSharedWebsocketWrapper* m_instance;
	    bool m_connected = false;

    public:
	    explicit ThreadedSharedWebsocketWrapper(QObject* parent = nullptr);

	    static ThreadedSharedWebsocketWrapper* instance();

	    bool isConnected() const;

    signals:
	    void sendMsg(const QString& msg);
	    void connected();
	    void disconnected();
	    void error(const QString& msg);
	    void msgReceived(const QString& msg);
	    void open(const QUrl& url);
	    void setReconnectInterval(std::chrono::milliseconds time);
};

class MATCH_CONN_DECL_EXPORT ThreadedSharedWebsocket : public QThread
{
	Q_OBJECT

    private:
	    ReconnectingWebsocket * m_socket;

    public:
	    explicit ThreadedSharedWebsocket(QObject* parent = nullptr);

    protected:
	    void run() override;

	public slots:
	    void bindWrapper(ThreadedSharedWebsocketWrapper* wrapper);

    signals:
	    void foo();
};


#endif //OBS_STUDIO_RECONNECTINGSOCKET_H
