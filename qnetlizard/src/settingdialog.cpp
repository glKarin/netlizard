#include "settingdialog.h"

#include <QDebug>
#include <QTreeWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTreeWidgetItem>
#include <QGroupBox>
#include <QScrollArea>
#include <QSpinBox>
#include <QLabel>

#include "settings.h"
#include "settingwidget.h"
#include "qdef.h"

SettingDialog::SettingDialog(QWidget *parent) :
    QDialog(parent),
    m_content(0)
{
    setObjectName("SettingDialog");
    Init();
}

SettingDialog::~SettingDialog()
{
    DEBUG_DESTROY_Q
}

void SettingDialog::Init()
{
    QVBoxLayout *layout = new QVBoxLayout;
    QHBoxLayout *contentLayout = new QHBoxLayout;
    QTreeWidget *tree = new QTreeWidget;
    QTreeWidgetItem *item;
    QTreeWidgetItem *subItem;
    QScrollArea *area = new QScrollArea;

    QStringList groups;
    QHash<QString, QList<Settings::SettingItemCategory> > map;
    const Settings::SettingItemMap &_settingsConfig = Settings::SettingsConfig();
    for(int i = 0; i < _settingsConfig.size(); i++)
    {
        const Settings::SettingItemCategory &c = _settingsConfig[i];
        if(!groups.contains(c.group))
            groups.push_back(c.group);
        map[c.group].push_back(c);
    }

    Q_FOREACH(const QString &g, groups)
    {
        item = new QTreeWidgetItem;
        item->setText(0, g);
        Q_FOREACH(const Settings::SettingItemCategory &sic, map[g])
        {
            subItem = new QTreeWidgetItem;
            subItem->setText(0, sic.title);
            subItem->setData(0, Qt::UserRole, sic.name);
            item->addChild(subItem);
        }
        tree->addTopLevelItem(item);
    }

    m_content = new SettingGroup;
    m_content->setCheckable(false);
    area->setWidget(m_content);

    tree->expandAll();
    tree->setHeaderLabel("Setting items");
    tree->setMaximumWidth(128);
    tree->setMinimumWidth(96);
    contentLayout->addWidget(tree);
    contentLayout->addWidget(area, 1);
    area->setWidgetResizable(true);

    layout->addLayout(contentLayout);

    connect(tree, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this, SLOT(InitSettingContent(QTreeWidgetItem *, int)));

    setLayout(layout);

    //SetContentText(list->item(0));
    //list->setCurrentRow(0);

    setWindowTitle("Settings");
    resize(360, 320);
}

int SettingDialog::Show(QWidget *parent)
{
    SettingDialog dialog(parent);
    return dialog.exec();
}

void SettingDialog::InitSettingContent(QTreeWidgetItem *item, int column)
{
    QVariant c = item->data(0, Qt::UserRole);
    if(!c.isValid())
        return;
    m_content->SetSettingConfig(c.toString(), item->text(0));
}
