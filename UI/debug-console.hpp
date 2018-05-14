#pragma once
#include <QWidget>
#include <QLabel>
#include <QScrollArea>

class DebugConsole: public QWidget
{
	Q_OBJECT

	private:
		QScrollArea* m_scrollArea;
		QLabel* m_text;
		DebugConsole* m_instance;

		DebugConsole(QWidget* parent = nullptr);

	public:
		static DebugConsole* instance();
		void addMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg);

};

