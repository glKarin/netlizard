#ifndef _KARIN_ABOUTDIALOG_H
#define _KARIN_ABOUTDIALOG_H

#include <QMessageBox>

class AboutDialog : public QMessageBox
{
    Q_OBJECT
public:
    explicit AboutDialog(QWidget *parent = 0);
    virtual ~AboutDialog();
    static int Show(QWidget *parent);
    
signals:
    
public slots:

private:
    void Init();

private:
    Q_DISABLE_COPY(AboutDialog)
};

#endif // _KARIN_ABOUTDIALOG_H
