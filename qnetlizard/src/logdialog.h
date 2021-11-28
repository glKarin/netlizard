#ifndef _KARIN_LOGDIALOG_H
#define _KARIN_LOGDIALOG_H

#include <QDockWidget>

class QTextBrowser;
class LogOutput;

class LogDialog : public QDockWidget
{
    Q_OBJECT
public:
    explicit LogDialog(QWidget *parent = 0);
    virtual ~LogDialog();
    
signals:
    
public slots:
    void ResetPosAndSize();
    void ClearLog();

protected:
    virtual void hideEvent(QHideEvent *e);
    virtual void showEvent(QShowEvent *e);

private:
    void Init();

private Q_SLOTS:
    void PushLog(int type, const QString &str);

private:
    LogOutput *m_log;
    QTextBrowser *m_textViewer;

    static QString _logText;

    Q_DISABLE_COPY(LogDialog)
};

#endif // _KARIN_LOGDIALOG_H
