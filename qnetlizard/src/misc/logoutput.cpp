#include "logoutput.h"

#include <QDebug>
#include <QDateTime>
#include <QApplication>

#include <stdio.h>

#include "qdef.h"

static const QString LogOutputTypeNames[] = {
    "DEBUG",
    "WARNING",
    "CRITICAL",
    "FATAL",
};

QString LogOutputItem::Format() const
{
    const QString &typeName = LogOutputTypeNames[type];
    return QString("[%1][%2]: %3").arg(typeName).arg(datetime).arg(content);
}

LogOutput::LogOutput(QObject *parent) :
    QObject(parent),
    m_inited(false)
{
    setObjectName("LogOutput");
    connect(qApp, SIGNAL(aboutToQuit()), this, SLOT(Finish()));
    Start();
}

LogOutput::~LogOutput()
{
    DEBUG_DESTROY_Q
}

const LogOutputList & LogOutput::LogList() const
{
    return m_logList;
}

QString LogOutput::LogText() const
{
    return m_logText;
}

uint LogOutput::Count() const
{
    return m_logList.size();
}

void LogOutput::Clear()
{
    m_logList.clear();
    m_logText.clear();
}

void LogOutput::Push(int type, const QString &msg)
{
    if(!m_inited)
        return;
    LogOutputItem item(CurrentDatetime(), type, msg);
    AddItem(item);
}

QString LogOutput::CurrentDatetime() const
{
    QDateTime d = QDateTime::currentDateTime();
    return d.toString("yyyy-MM-dd HH:mm:ss");
}

void LogOutput::AddItem(const LogOutputItem &item)
{
    if(!m_inited)
        return;
    m_logList.push_back(item);
    QString str = item.Format();
    m_logText += str + "\n";
    emit outputLog(item.type, str);
}

void LogOutput::Finish()
{
    if(!m_inited)
        return;
    m_inited = false;
    qInstallMsgHandler(0);
}

void LogOutput::Start()
{
    if(m_inited)
        return;
    m_inited = true;
    //qInstallMsgHandler(log_output_msg_handler);
}

LogOutput * LogOutput::Instance()
{
    static LogOutput _instance;
    return &_instance;
}

void log_output_msg_handler(QtMsgType type, const char *msg)
{
    LogOutput *lo = LogOutput::Instance();
    FILE *f = type == 0 ? stdout : stderr;
    static const char *_console_color[] = {
        "",
        "\033[32m", // green
        "\033[33m", // yellow
        "\033[31m", // red
        "\033[35m", // pur
    };
    fprintf(f, "%s %s\n\033[0m", _console_color[type], msg);
    lo->Push(type, msg);
}
