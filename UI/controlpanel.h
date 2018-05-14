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

    public:
        ControlPanel(QWidget* parent = nullptr);
        QString serverHostName() const;

        QUrl serverUrl() const;

    private slots:
        bool validateServerHostName(const QString& hostName);

    signals:
        void connectRequest(const QUrl& url);
        void serverHostNameChanged(const QUrl& url);
};
