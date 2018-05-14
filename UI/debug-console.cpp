#include "debug-console.hpp"
#include <QHBoxLayout>

DebugConsole::DebugConsole(QWidget* parent) : QWidget(parent)
{
	m_scrollArea = new QScrollArea(this);
	m_scrollArea->setWidgetResizable(true);
	m_scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

	m_text = new QLabel(this);
	QFont font = m_text->font();
	font.setPixelSize(16);
	m_text->setFont(font);
	m_text->setStyleSheet("background-color:white; color: black");

	m_scrollArea->setWidget(m_text);

	QHBoxLayout* layout = new QHBoxLayout(this);
	layout->addWidget(m_scrollArea);
	layout->setSpacing(0);
	layout->setContentsMargins(0, 0, 0, 0);
	this->setLayout(layout);
}

DebugConsole* DebugConsole::instance()
{
	static DebugConsole i{};
	return &i;
}

void DebugConsole::addMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
	QString text;
	switch (type) {
	case QtDebugMsg:
		text = QString("<font color='green'>DEBUG: </font>%1 (%2:%3, %4)<br />").arg(msg).arg(context.file).arg(context.line).arg(context.function);
		break;
	case QtInfoMsg:
		text = QString("<font color='green'>INFO: </font>%1 (%2:%3, %4)<br />").arg(msg).arg(context.file).arg(context.line).arg(context.function);
		break;
	case QtWarningMsg:
		text = QString("<font color='green'>WARNING: </font>%1 (%2:%3, %4)<br />").arg(msg).arg(context.file).arg(context.line).arg(context.function);
		break;
	case QtCriticalMsg:
		text = QString("<font color='green'>CRITICAL: </font>%1 (%2:%3, %4)<br />").arg(msg).arg(context.file).arg(context.line).arg(context.function);
		break;
	case QtFatalMsg:
		text = QString("<font color='green'>FATAL: </font>%1 (%2:%3, %4)<br />").arg(msg).arg(context.file).arg(context.line).arg(context.function);
		abort();
	}

	m_text->setText(m_text->text() + text);
}
