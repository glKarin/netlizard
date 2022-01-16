#include "logoutput.h"

#include <QDebug>
#include <QDateTime>
#include <QApplication>
#include <QDir>
#include <QFile>
#include <QFileInfo>

#include <stdio.h>

#include "qdef.h"
#include "utils/nlioutility.h"
#include "settings.h"

#define LOG_DIR_NAME "logs"
#define LOG_FILE_NAME APP_NAME
#define MAX_LOG 0

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
    m_inited(false),
    m_outputChannel(LogOutput::Output_Console | LogOutput::Output_File),
    m_maxCache(MAX_LOG)
{
    setObjectName("LogOutput");
    Settings *settings = SINGLE_INSTANCE_OBJ(Settings);
    SetOutputChannel(settings->GetSetting<bool>("APP/log_file", false) ? m_outputChannel | LogOutput::Output_File : m_outputChannel & (~LogOutput::Output_File));
    connect(settings, SIGNAL(settingChanged(const QString &, const QVariant &, const QVariant &)), this, SLOT(OnSettingChanged(const QString &, const QVariant &, const QVariant &)));
    connect(qApp, SIGNAL(aboutToQuit()), this, SLOT(Finish()));
#ifndef _DEV_TEST
    Start();
#endif
}

LogOutput::~LogOutput()
{
    DEBUG_DESTROY_Q
}

void LogOutput::Clear()
{
    m_logList.clear();
}

void LogOutput::Push(int type, const char *msg)
{
    if(!m_inited)
        return;
    LogOutputItem item(CurrentDatetime(), type, msg);
    OutputConsole(type, msg);
    AddItem(item);
}

void LogOutput::OutputConsole(int type, const char *msg)
{
    if(!(m_outputChannel & LogOutput::Output_Console))
        return;
    FILE *f = type == 0 ? stdout : stderr;
#ifdef Q_OS_UNIX
    static const char *_console_color[] = {
        "",
        "\033[32m", // green
        "\033[33m", // yellow
        "\033[31m", // red
        "\033[35m", // pur
    };
    fprintf(f, "%s %s\n\033[0m", _console_color[type], msg);
#else
    fprintf(f, "%s\n", msg);
#endif
}

bool LogOutput::MkLogDir()
{
    QDir dir(QDir::currentPath() + "/" + LOG_DIR_NAME);
    if(dir.exists())
        return true;
    return QDir::current().mkdir(LOG_DIR_NAME);
}

void LogOutput::MkLogFile()
{
    if(m_logFile.isEmpty())
        m_logFile = QDir::currentPath() + "/" + LOG_DIR_NAME + "/" + LOG_FILE_NAME + "_" + (CurrentDatetime().replace(" ", "_").replace(":", "-")) + ".log";
    //return m_logFile;
}

void LogOutput::OutputFile(const QString &str)
{
    if(!(m_outputChannel & LogOutput::Output_File))
        return;
    if(!MkLogDir())
        return;
    MkLogFile();
    NLIOUtility::file_put_contents(m_logFile, str + "\n", "UTF-8", 1);
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

    if(m_maxCache != 0)
    {
        m_logList.push_back(item);
        if(m_maxCache > 0)
        {
            while(m_logList.count() > m_maxCache)
                m_logList.pop_front();
        }
    }
    const QString str(item.Format());
    OutputFile(str);
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
    qInstallMsgHandler(log_output_msg_handler);
}

LogOutput * LogOutput::Instance()
{
    static LogOutput _instance;
    return &_instance;
}

int LogOutput::OutputChannel() const
{
    return m_outputChannel;
}

void LogOutput::SetOutputChannel(int c)
{
    if(m_outputChannel != c)
    {
        m_outputChannel = c;
    }
}

void LogOutput::SetMaxCache(int c)
{
    if(m_maxCache != c)
    {
        m_maxCache = c;
        if(m_maxCache > 0)
        {
            while(m_logList.count() > m_maxCache)
                m_logList.pop_front();
        }
        else if(m_maxCache == 0)
            m_logList.clear();
    }
}

void LogOutput::OnSettingChanged(const QString &name, const QVariant &value, const QVariant &oldValue)
{
    if(name == "APP/log_file")
        SetOutputChannel(value.toBool() ? m_outputChannel | LogOutput::Output_File : m_outputChannel & (~LogOutput::Output_File));
}

void log_output_msg_handler(QtMsgType type, const char *msg)
{
    LogOutput *lo = LogOutput::Instance();
    lo->Push(type, msg);
}
