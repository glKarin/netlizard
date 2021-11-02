#ifndef _KARIN_LOGDIALOG_H
#define _KARIN_LOGDIALOG_H

#include <QDialog>

class QTextBrowser;
class LogOutput;

class LogDialog : public QDialog
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

    Q_DISABLE_COPY(LogDialog)
    
};

#endif // _KARIN_LOGDIALOG_H
