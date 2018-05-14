#pragma once

#include <QObject>
#include <QTimer>
#include <chrono>
#include <QtWebSockets/QWebSocket>

class SharedWebsocket: public QObject
{
        Q_OBJECT
		Q_ENUMS(Statuses)
		Q_PROPERTY(int status READ status NOTIFY statusChanged)
		Q_PROPERTY(QUrl url READ url WRITE setUrl NOTIFY urlChanged)

	public:
		enum Statuses {Open, Closed};

    private:
        static SharedWebsocket* m_instance;
		Statuses m_status;
        QWebSocket m_socket;
        QTimer m_reconnectTimer;
        QUrl m_url;

    public:
		explicit SharedWebsocket(QObject* parent = nullptr);

		int status() const;
		QUrl url() const;
		static SharedWebsocket* instance();

    private slots:
        void reconnect();
		void setStatus(Statuses status);
        void onConnected();
        void onDisconnected();
		void onSslError(const QList<QSslError>& errors);

	public slots:
		void stopReconnecting();
	    void setReconnectInterval(std::chrono::milliseconds time);
	    void open(std::chrono::milliseconds reconnectTime);
	    void open();
	    void open(const QUrl& url);
	    void sendMsg(const QString& msg);
	    void setUrl(const QUrl& url);

    signals:
		void oldSocketClosed();
	    void connected();
	    void disconnected();
	    void error(const QString& msg);
	    void msgReceived(const QString& msg);
	    void statusChanged(int status);
	    void urlChanged(const QUrl& url);
};

