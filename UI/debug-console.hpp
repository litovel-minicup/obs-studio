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
		static DebugConsole* m_instance;

	public:
		DebugConsole(QWidget* parent = nullptr);

		static void setInstance(DebugConsole* instance);
		static DebugConsole* instance();
		void addMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg);

};

