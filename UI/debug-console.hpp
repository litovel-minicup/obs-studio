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

	protected:
		void closeEvent(QCloseEvent* e) override;

	public:
		DebugConsole(QWidget* parent = nullptr);

		static DebugConsole* instance();
		void addMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg);

};

