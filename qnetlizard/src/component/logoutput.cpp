#include "logoutput.h"

#include <QDebug>
#include <QDateTime>

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
    QObject(parent)
{
    setObjectName("LogOutput");
}

LogOutput::~LogOutput()
{
    DEBUG_DESTROY_Q
}

LogOutputList LogOutput::LogList() const
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
    m_logList.push_back(item);
    QString str = item.Format();
    m_logText += str + "\n";
    emit outputLog(item.type, str);
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
    fprintf(f, msg);
    fprintf(f, "\n");
    lo->Push(type, msg);
}
