#include "settingwidget.h"

#include <QDebug>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

#include "settings.h"
#include "qdef.h"

SettingGroupItem::SettingGroupItem(QWidget *parent)
    : QWidget(parent),
      m_label(0),
      m_widget(0),
      m_layout(0)
{
    setObjectName("SettingGroupItem");
    Init();
}

SettingGroupItem::~SettingGroupItem()
{
    DEBUG_DESTROY_Q;
}

void SettingGroupItem::Init()
{
    m_layout = new QHBoxLayout;
    m_label = new QLabel;
    m_label->setMaximumWidth(128);
    m_layout->addWidget(m_label);
    setLayout(m_layout);
}

void SettingGroupItem::Clear()
{
    if(m_widget)
    {
        m_layout->removeWidget(m_widget);
        m_widget = 0;
    }
}

void SettingGroupItem::SetTitle(const QString &title)
{
    m_label->setText(title);
}

void SettingGroupItem::SetWidget(QWidget *widget)
{
    if(m_widget != widget)
    {
        Clear();
        if(widget)
        {
            m_layout->addWidget(widget);
            m_widget = widget;
        }
    }
}

void SettingGroupItem::Set(const QString &title, QWidget *widget)
{
    SetTitle(title);
    SetWidget(widget);
}

SettingGroupItem * SettingGroupItem::Make(const QString &title, QWidget *widget)
{
    SettingGroupItem *item = new SettingGroupItem;
    item->Set(title, widget);
    return item;
}



SettingGroup::SettingGroup(QWidget *parent)
    : QGroupBox(parent),
      m_layout(0)
{
    setObjectName("SettingGroup");
    Init();
}

SettingGroup::~SettingGroup()
{
    DEBUG_DESTROY_Q;
}

void SettingGroup::Init()
{
    m_layout = new QVBoxLayout;
    setLayout(m_layout);
}

void SettingGroup::Clear()
{
    setTitle(QString());
    QLayoutItem *item;
    while(!m_layout->isEmpty())
    {
        item = m_layout->takeAt(0);
        delete item->widget();
        //delete item->spacerItem();
        delete item;
    }
}

void SettingGroup::SetSettingConfig(const QString &name, const QString &title)
{
    if(m_name == name)
        return;
    Clear();
    setTitle(title);
    const SettingItemMap &_settingsConfig = Settings::SettingsConfig();
    if(!_settingsConfig.contains(name))
        return;
    QList<SettingItem> items = _settingsConfig.values(name);

    Q_FOREACH(const SettingItem &item, items)
    {
        QWidget *widget = 0;
        if(item.type == "int")
        {
            QSpinBox *w = new QSpinBox;
            w->setObjectName(item.name);
            w->setSingleStep(1);
            w->setMinimum(item.prop.value("min").toInt());
            w->setMaximum(item.prop.value("max").toInt());
            w->setValue(SINGLE_INSTANCE_OBJ(Settings)->GetSetting<int>(item.name, item.value.toInt()));
            connect(w, SIGNAL(valueChanged(const QString &)), this, SLOT(OnValueChanged(const QString &)));
            widget = w;
        }
        else if(item.type == "float")
        {
            QDoubleSpinBox *w = new QDoubleSpinBox;
            w->setObjectName(item.name);
            w->setDecimals(2);
            w->setSingleStep(0.01);
            w->setMinimum(item.prop.value("min").toFloat());
            w->setMaximum(item.prop.value("max").toFloat());
            w->setValue(SINGLE_INSTANCE_OBJ(Settings)->GetSetting<double>(item.name, item.value.toFloat()));
            connect(w, SIGNAL(valueChanged(const QString &)), this, SLOT(OnValueChanged(const QString &)));
            widget = w;
        }
        if(widget)
        {
            SettingGroupItem *gi = SettingGroupItem::Make(item.title, widget);
            m_layout->addWidget(gi);
        }
    }
    //if(!m_layout->isEmpty())
    {
        m_layout->addStretch();
    }
}

void SettingGroup::OnValueChanged(const QString &val)
{
    QObject *obj = sender();
    QString name = obj->objectName();
    Settings *settings = SINGLE_INSTANCE_OBJ(Settings);
    if(instanceofv(obj, QSpinBox))
    {
        settings->SetSetting(name, val.toInt());
    }
    else if(instanceofv(obj, QDoubleSpinBox))
    {
        settings->SetSetting(name, val.toFloat());
    }
}
