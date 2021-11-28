#include "logoutput.h"

#include <QDebug>
#include <QDateTime>
#include <QApplication>
#include <QDir>
#include <QFile>
#include <QFileInfo>

#include <stdio.h>

#include "qdef.h"
#include "ioutility.h"

#define LOG_DIR_NAME "logs"
#define LOG_FILE_NAME APP_NAME
#define MAX_LOG 20

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
    m_outputChannel(LogOutput::Output_Console | LogOutput::Output_File)
{
    setObjectName("LogOutput");
    connect(qApp, SIGNAL(aboutToQuit()), this, SLOT(Finish()));
    //Start();
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
    IOUtility::file_put_contents(m_logFile, str + "\n", 1);
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
#if(MAX_LOG != 0)
    m_logList.push_back(item);
#if(MAX_LOG > 0)
    while(m_logList.count() > MAX_LOG)
        m_logList.pop_front();
#endif
#endif
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

void log_output_msg_handler(QtMsgType type, const char *msg)
{
    LogOutput *lo = LogOutput::Instance();
    lo->Push(type, msg);
}
