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

#include "settingwidget.h"
#include "qdef.h"

SettingDialog::SettingDialog(QWidget *parent) :
    QDialog(parent),
    m_content(0),
    m_tree(0)
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
    m_tree = new QTreeWidget;
    QScrollArea *area = new QScrollArea;

    const Settings::SettingItemMap &_settingsConfig = Settings::SettingsConfig();
    Q_FOREACH(const Settings::SettingItemCategory *c, _settingsConfig)
    {
        QTreeWidgetItem *subItem = new QTreeWidgetItem;
        subItem->setText(0, c->title);
        subItem->setData(0, Qt::UserRole, c->name);
        subItem->setToolTip(0, c->description);
        m_tree->addTopLevelItem(subItem);

        AddSettingNode(c, subItem);
    }

    m_content = new SettingGroup;
    m_content->setCheckable(false);
    area->setWidget(m_content);
    connect(m_content, SIGNAL(openSettingGroup(const QString &)), this, SLOT(OpenSettingGroup(const QString &)));

    m_tree->expandAll();
    m_tree->setHeaderLabel(tr("Setting items"));
    m_tree->setMaximumWidth(128);
    m_tree->setMinimumWidth(96);
    contentLayout->addWidget(m_tree);
    contentLayout->addWidget(area, 1);
    area->setWidgetResizable(true);

    layout->addLayout(contentLayout);

    connect(m_tree, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this, SLOT(InitSettingContent(QTreeWidgetItem *, int)));

    setLayout(layout);

    //SetContentText(list->item(0));
    //list->setCurrentRow(0);

    setWindowTitle(tr("Settings"));
    resize(480, 360);
}

int SettingDialog::Show(QWidget *parent)
{
    SettingDialog dialog(parent);
    return dialog.exec();
}

void SettingDialog::InitSettingContent(QTreeWidgetItem *item, int column)
{
    QVariant c = item->data(0, Qt::UserRole);
    m_content->SetSettingConfig(c.toString(), item->text(0));
}

void SettingDialog::AddSettingNode(const Settings::SettingItemCategory *s, QTreeWidgetItem *parent)
{
    Q_FOREACH(const Settings::SettingItem *a, s->settings)
    {
        if(a->item_type != Settings::SettingItem::Item_Category)
            continue;

        QTreeWidgetItem *subItem = new QTreeWidgetItem;
        subItem->setText(0, a->title);
        subItem->setData(0, Qt::UserRole, a->name);
        subItem->setToolTip(0, a->description);
        if(parent)
            parent->addChild(subItem);
        else
            m_tree->addTopLevelItem(subItem);

        AddSettingNode(static_cast<const Settings::SettingItemCategory *>(a), subItem);
    }
}

QTreeWidgetItem * SettingDialog::FindSettingNode(const QString &name, QTreeWidgetItem *parent)
{
    const int Count = parent->childCount();
    for(int i = 0; i < Count; i++)
    {
        QTreeWidgetItem *item = parent->child(i);
        qDebug() << i << item->data(0, Qt::UserRole).toString();
        if(item->data(0, Qt::UserRole).toString() == name)
            return item;
        if(item->childCount() > 0)
        {
            QTreeWidgetItem *subitem = FindSettingNode(name, item);
            if(subitem)
                return subitem;
        }
    }
    return 0;
}

void SettingDialog::OpenSettingGroup(const QString &name)
{
    qDebug() << name;
    const int Count = m_tree->topLevelItemCount();
    for(int i = 0; i < Count; i++)
    {
        QTreeWidgetItem *item = m_tree->topLevelItem(i);
        if(item->data(0, Qt::UserRole).toString() == name)
        {
            m_tree->setCurrentItem(item);
            InitSettingContent(item, 0);
            return;
        }
        QTreeWidgetItem *subitem = FindSettingNode(name, item);
        if(subitem)
        {
            m_tree->setCurrentItem(subitem);
            InitSettingContent(subitem, 0);
            return;
        }
    }
}
