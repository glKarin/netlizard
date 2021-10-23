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
    virtual ~LogOutput();
    static LogOutput * Instance();
    const LogOutputList & LogList() const;
    QString LogText() const;
    uint Count() const;
    
signals:
    void outputLog(int type, const QString &msg);
    
public slots:
    void Clear();
    void Start();
    void Finish();
    void Push(int type, const QString &msg);

private:
    explicit LogOutput(QObject *parent = 0);
    QString CurrentDatetime() const;
    void AddItem(const LogOutputItem &item);
    
private:
    LogOutputList m_logList;
    QString m_logText;
    bool m_inited;

    Q_DISABLE_COPY(LogOutput)
};

void log_output_msg_handler(QtMsgType type, const char *msg);

#endif // _KARIN_LOGOUTPUT_H
