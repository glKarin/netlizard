#ifndef _KARIN_SETTINGDIALOG_H
#define _KARIN_SETTINGDIALOG_H

#include <QDialog>

class SettingGroup;
class QTreeWidgetItem;

class SettingDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SettingDialog(QWidget *parent = 0);
    virtual ~SettingDialog();
    static int Show(QWidget *parent);
    
signals:
    
public slots:

private:
    void Init();

private Q_SLOTS:
    void InitSettingContent(QTreeWidgetItem *item, int column);

private:
    SettingGroup *m_content;
    Q_DISABLE_COPY(SettingDialog)
};

#endif // _KARIN_SETTINGDIALOG_H
