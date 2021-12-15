#ifndef _KARIN_SETTINGDIALOG_H
#define _KARIN_SETTINGDIALOG_H

#include <QDialog>

#include "settings.h"

class QTreeWidgetItem;
class QTreeWidget;
class SettingGroup;

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
    void AddSettingNode(const Settings::SettingItemCategory *s, QTreeWidgetItem *parent);
    QTreeWidgetItem * FindSettingNode(const QString &name, QTreeWidgetItem *parent);

private Q_SLOTS:
    void InitSettingContent(QTreeWidgetItem *item, int column);
    void OpenSettingGroup(const QString &name);

private:
    SettingGroup *m_content;
    QTreeWidget *m_tree;

    Q_DISABLE_COPY(SettingDialog)
};

#endif // _KARIN_SETTINGDIALOG_H
