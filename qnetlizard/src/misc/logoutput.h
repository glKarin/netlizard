#ifndef _KARIN_LOGOUTPUT_H
#define _KARIN_LOGOUTPUT_H

#include <QObject>
#include <QList>

struct LogOutputItem
{
    QString datetime;
    int type;
    QString content;
    LogOutputItem(const QString &datetime, int type, const QString &content) {
        this->datetime = datetime;
        this->type = type;
        this->content = content;
    }
    QString Format() const;
};

typedef QList<LogOutputItem> LogOutputList;

class LogOutput : public QObject
{
    Q_OBJECT
public:
    enum Output_Channel
    {
        Output_Console = 1,
        Output_File = 2
    };

public:
    virtual ~LogOutput();
    static LogOutput * Instance();
    const LogOutputList & LogList() const { return m_logList; }
    uint Count() const { return m_logList.size(); }
    int OutputChannel() const;
    void SetOutputChannel(int c);
    void SetMaxCache(int c);
    
signals:
    void outputLog(int type, const QString &msg);
    
public slots:
    void Clear();
    void Start();
    void Finish();
    void Push(int type, const char *msg);

private:
    explicit LogOutput(QObject *parent = 0);
    QString CurrentDatetime() const;
    void AddItem(const LogOutputItem &item);
    void OutputConsole(int type, const char *msg);
    void MkLogFile();
    bool MkLogDir();
    void OutputFile(const QString &item);
    
private:
    LogOutputList m_logList;
    bool m_inited;
    int m_outputChannel;
    QString m_logFile;
    int m_maxCache;

    Q_DISABLE_COPY(LogOutput)
};

void log_output_msg_handler(QtMsgType type, const char *msg);

#endif // _KARIN_LOGOUTPUT_H
