#ifndef _KARIN_STATUSBAR_H
#define _KARIN_STATUSBAR_H

#include <QStatusBar>

class QLabel;
class QTimer;

class StatusBar : public QStatusBar
{
    Q_OBJECT
public:
    explicit StatusBar(QWidget *parent = 0);
    virtual ~StatusBar();
    QString CurrentPermanentMessage() const;
    QString CurrentTemporaryMessage() const;
    
signals:
    void permanentMessageChanged(const QString &message);
    void temporaryMessageChanged(const QString &message);
    
public slots:
    void ShowPermanentMessage(const QString &str);
    void ClearPermanentMessage();
    void ShowTemporaryMessage(const QString &str, int timeout = 0);
    void ClearTemporaryMessage();
    void Reset(bool visible = false);

private:
    void ShowText();

private Q_SLOTS:
    void OnTimeOut();

private:
    QString m_permanentLabel;
    QString m_temporaryLabel;
    QTimer *m_timer;

    Q_DISABLE_COPY(StatusBar)
};

#endif // _KARIN_STATUSBAR_H
