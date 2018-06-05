#include "debug-console.hpp"
#include <QHBoxLayout>
#include <QCloseEvent>

DebugConsole* DebugConsole::m_instance = nullptr;

void DebugConsole::closeEvent(QCloseEvent * e)
{
	e->ignore();
}

DebugConsole::DebugConsole(QWidget* parent) : QWidget(parent)
{
	m_scrollArea = new QScrollArea(this);
	m_scrollArea->setWidgetResizable(true);
	m_scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

	m_text = new QLabel(this);
	m_text->setAlignment(Qt::AlignTop);
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
	if (DebugConsole::m_instance == nullptr) {
		DebugConsole::m_instance = new DebugConsole;
		DebugConsole::m_instance->setWindowFlags(Qt::WindowTitleHint | Qt::WindowMinimizeButtonHint);
		DebugConsole::m_instance->setWindowTitle("Debug console");
	}
	return DebugConsole::m_instance;
}

void DebugConsole::addMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
	QString text;
	switch (type) {
	case QtDebugMsg:
		if(QString(context.file).contains(".qml")) {
			text = QString("<font color='magenta'>QML: </font>%1 (%2:%3)<br />")
					.arg(msg)
					.arg(context.line)
					.arg(context.function);
		}

		else {
			text = QString("<font color='green'>DEBUG: </font>%1 (%2:%3, %4)<br />")
					.arg(msg)
					.arg(context.file)
					.arg(context.line)
					.arg(context.function);
		}
		break;
	case QtInfoMsg:
		text = QString("<font color='blue'>INFO: </font>%1 (%2:%3, %4)<br />").arg(msg).arg(context.file).arg(context.line).arg(context.function);
		break;
	case QtWarningMsg:
		text = QString("<font color='orange'>WARNING: </font>%1 (%2:%3, %4)<br />").arg(msg).arg(context.file).arg(context.line).arg(context.function);
		break;
	case QtCriticalMsg:
		text = QString("<font color='green'>CRITICAL: </font>%1 (%2:%3, %4)<br />").arg(msg).arg(context.file).arg(context.line).arg(context.function);
		break;
	case QtFatalMsg:
		text = QString("<font color='green'>FATAL: </font>%1 (%2:%3, %4)<br />").arg(msg).arg(context.file).arg(context.line).arg(context.function);
		abort();
	}

	m_text->setText(m_text->text() + text.replace("\n", "<br/>").replace("\t", "  "));
}
