#ifndef _KARIN_CHANGELOGDIALOG_H
#define _KARIN_CHANGELOGDIALOG_H

#include <QDialog>

class ChangelogDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ChangelogDialog(QWidget *parent = 0);
    virtual ~ChangelogDialog();
    static int Show(QWidget *parent);
    
signals:
    
public slots:

private:
    void Init();
    bool LoadChangelog();

private:
    QString m_text;
    
    Q_DISABLE_COPY(ChangelogDialog)
};

#endif // _KARIN_CHANGELOGDIALOG_H
