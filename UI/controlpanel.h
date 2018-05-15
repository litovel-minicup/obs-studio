#pragma once

#include <QWidget>
#include <QLineEdit>
#include <QtWidgets/QPushButton>

class ControlPanel: public QWidget
{
    Q_OBJECT

    private:
        QLineEdit* m_serverHostEdit;
        QPushButton* m_connectButton;
        bool m_connected;

    protected:
        void paintEvent(QPaintEvent*) override;

    public:
        ControlPanel(QWidget* parent = nullptr);
        QString serverHostName() const;

        QUrl serverUrl() const;

    private slots:
        bool validateServerHostName(const QString& hostName);

    public slots:
        void setConnected();
        void setDisconnected();

    signals:
        void connectRequest(const QUrl& url);
        void serverHostNameChanged(const QUrl& url);
};
