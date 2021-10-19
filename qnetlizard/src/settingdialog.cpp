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

    item = new QTreeWidgetItem;
    item->setText(0, "Viewer");
    subItem = new QTreeWidgetItem;
    subItem->setText(0, "3D control");
    subItem->setData(0, Qt::UserRole, "3d_control");
    item->addChild(subItem);
    subItem = new QTreeWidgetItem;
    subItem->setText(0, "2D control");
    subItem->setData(0, Qt::UserRole, "2d_control");
    item->addChild(subItem);
    subItem = new QTreeWidgetItem;
    subItem->setText(0, "Render");
    subItem->setData(0, Qt::UserRole, "render");
    item->addChild(subItem);
    tree->addTopLevelItem(item);

    item = new QTreeWidgetItem;
    item->setText(0, "Others");
    tree->addTopLevelItem(item);

    m_content = new SettingGroup;
    m_content->setCheckable(false);
    area->setWidget(m_content);

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
