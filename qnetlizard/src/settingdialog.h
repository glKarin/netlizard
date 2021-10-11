#ifndef _KARIN_SETTINGDIALOG_H
#define _KARIN_SETTINGDIALOG_H

#include <QDialog>

class SettingDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SettingDialog(QWidget *parent = 0);
    virtual ~SettingDialog();
    static int Show(QWidget *parent);
    
signals:
    
public slots:

private Q_SLOTS:
    void SaveSetting();

    Q_DISABLE_COPY(SettingDialog)
};

#endif // _KARIN_SETTINGDIALOG_H
