#include "settingwidget.h"

#include <QDebug>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QStackedLayout>
#include <QLabel>
#include <QCheckBox>
#include <QComboBox>
#include <QButtonGroup>
#include <QPushButton>
#include <QColorDialog>
#include <QTabWidget>

#include "qdef.h"

SettingGroup::SettingGroup(QWidget *parent)
    : QGroupBox(parent)
{
    setObjectName("SettingGroup");
    Init();
}

SettingGroup::~SettingGroup()
{
    Clear();
    DEBUG_DESTROY_Q;
}

void SettingGroup::Init()
{
}

void SettingGroup::Clear()
{
    setTitle(QString());
    QLayout *l = layout();
    if(l)
    {
        QLayoutItem *item;
        while(!l->isEmpty())
        {
            item = l->takeAt(0);
            delete item->widget();
            delete item;
        }
        delete l;
    }
}

const Settings::SettingItemCategory * SettingGroup::FindSettingConfig(const Settings::SettingItemCategory *c, const QString &name)
{
    Q_FOREACH(const Settings::SettingItem *item, c->settings)
    {
        if(item->item_type != Settings::SettingItem::Item_Category)
            continue;
        if(name == item->name)
        {
            return static_cast<const Settings::SettingItemCategory *>(item);
        }
        const Settings::SettingItemCategory *b = FindSettingConfig(static_cast<const Settings::SettingItemCategory *>(item), name);
        if(b)
            return b;
    }
    return 0;
}

void SettingGroup::SetSettingConfig(const QString &name, const QString &title)
{
    if(m_name == name)
        return;
    Clear();
    m_name = name;
    setTitle(title);

    const Settings::SettingItemMap &_settingsConfig = Settings::SettingsConfig();
    Q_FOREACH(const Settings::SettingItemCategory *c, _settingsConfig)
    {
        const Settings::SettingItemCategory *sc = c->name == name ? c : FindSettingConfig(c, name);
        if(sc)
        {
            if(IsGroupSettingConfig(sc))
            {
                QStackedLayout *m_layout = new QStackedLayout;
                QTabWidget *tabWidget = new QTabWidget;
                m_layout->addWidget(tabWidget);
                setLayout(m_layout);
                Q_FOREACH(const Settings::SettingItem *cc, sc->settings)
                {
                    if(cc->item_type != Settings::SettingItem::Item_Category)
                        continue;
                    QWidget *widget = new QWidget(tabWidget);
                    GenSettingConfig(widget, static_cast<const Settings::SettingItemCategory *>(cc));
                    tabWidget->addTab(widget, cc->title);
                }
            }
            else
            {
                GenSettingConfig(this, sc);
            }
            break;
        }
    }
}

bool SettingGroup::IsGroupSettingConfig(const Settings::SettingItemCategory *category)
{
    Q_FOREACH(const Settings::SettingItem *c, category->settings)
    {
        if(c->item_type != Settings::SettingItem::Item_Category)
            return false;
    }
    return true;
}

void SettingGroup::GenSettingConfig(QWidget *parent, const Settings::SettingItemCategory *c)
{
    QFormLayout *m_layout = new QFormLayout;
    m_layout->setRowWrapPolicy(QFormLayout::WrapLongRows);
    parent->setLayout(m_layout);
    Settings *settings = SINGLE_INSTANCE_OBJ(Settings);

    Q_FOREACH(const Settings::SettingItem *si, c->settings)
    {
        if(!si->IsValid())
            continue;

        QWidget *widget = 0;
        QString tail = si->description.isEmpty() ? "" : QString("(%1)").arg(si->description);
        QString label(si->title + tail);
        if(si->item_type == Settings::SettingItem::Item_Category)
        {
            QLabel *w = new QLabel;
            w->setObjectName(si->name);
            w->setText(QString("<a href='%1'>%2 &gt;&gt;</a>").arg(si->name).arg(si->title));
            connect(w, SIGNAL(linkActivated(const QString &)), this, SIGNAL(openSettingGroup(const QString &)));
            widget = w;
        }
        else
        {
            const Settings::SettingItemContent &item = static_cast<const Settings::SettingItemContent &>(*si);
            if(item.widget == "spinbox")
            {
                if(item.type == "int")
                {
                    QSpinBox *w = new QSpinBox;
                    w->setObjectName(item.name);
                    w->setSingleStep(1);
                    w->setMinimum(item.prop.value("min").toInt());
                    w->setMaximum(item.prop.value("max").toInt());
                    w->setValue(settings->GetSetting<int>(item.name, item.value.toInt()));
                    connect(w, SIGNAL(valueChanged(const QString &)), this, SLOT(OnValueChanged(const QString &)));
                    widget = w;
                }
                else
                {
                    QDoubleSpinBox *w = new QDoubleSpinBox;
                    w->setObjectName(item.name);
                    w->setSingleStep(0.01);
                    w->setDecimals(2);
                    w->setMinimum(item.prop.value("min").toFloat());
                    w->setMaximum(item.prop.value("max").toFloat());
                    w->setValue(settings->GetSetting<double>(item.name, item.value.toFloat()));
                    connect(w, SIGNAL(valueChanged(const QString &)), this, SLOT(OnValueChanged(const QString &)));
                    widget = w;
                }
            }
            else if(item.widget == "checkbox")
            {
                if(item.type == "bool")
                {
                    QCheckBox *w = new QCheckBox;
                    w->setObjectName(item.name);
                    w->setChecked(settings->GetSetting<bool>(item.name, item.value.toBool()));
                    connect(w, SIGNAL(clicked(bool)), this, SLOT(OnBoolChanged(bool)));
                    widget = w;
                }
                else if(item.type == "int")
                {
                    QGroupBox *w = new QGroupBox;
                    w->setObjectName(item.name);
                    QButtonGroup *g = new QButtonGroup(w);
                    g->setObjectName(item.name);
                    g->setExclusive(false);
                    QVBoxLayout *vbox = new QVBoxLayout;
                    const QVariantList & items = item.prop.value("option").toList();
                    int target = settings->GetSetting<int>(item.name, item.value.toInt());
                    for(int i = 0; i < items.size(); i++)
                    {
                        const QVariantHash p = items[i].toHash();
                        const QVariant v = p.value("value");
                        int iv = v.toInt();
                        QCheckBox *cb = new QCheckBox(p.value("label").toString());
                        g->addButton(cb, iv);

                        if(target & iv)
                            cb->setChecked(true);
                        vbox->addWidget(cb);
                    }
                    vbox->addStretch(1);

                    w->setLayout(vbox);
                    w->setTitle(item.title);
                    label.clear();
                    //w->setCheckable(true);
                    connect(g, SIGNAL(buttonClicked(int)), this, SLOT(OnIntChanged(int)));
                    widget = w;
                }
            }
            else if(item.widget == "combobox")
            {
                QComboBox *w = new QComboBox;
                w->setObjectName(item.name);
                const QVariantList & items = item.prop.value("option").toList();
                int cur = 0;
                int target = settings->GetSetting<int>(item.name, item.value.toInt());
                for(int i = 0; i < items.size(); i++)
                {
                    const QVariantHash p = items[i].toHash();
                    const QVariant v = p.value("value");
                    w->addItem(p.value("label").toString(), v);
                    if(v.toInt() == target)
                        cur = i;
                }
                w->setCurrentIndex(cur);
                connect(w, SIGNAL(currentIndexChanged(int)), this, SLOT(OnIntChanged(int)));
                widget = w;
            }
            else if(item.widget == "color_dialog")
            {
                QPushButton *w = new QPushButton;
                w->setObjectName(item.name);
                QString target = settings->GetSetting<QString>(item.name, item.value.toString());
                w->setProperty("color", target);
                w->setText(target.toUpper());
                w->setStyleSheet(QString("QPushButton { color: %1; }").arg(target));
                connect(w, SIGNAL(clicked()), this, SLOT(ChooseColor()));
                widget = w;
            }
        }

        if(widget)
        {
            connect(widget, SIGNAL(destroyed(QObject *)), this, SLOT(OnItemDestroy(QObject *)));
            if(label.isEmpty())
                m_layout->addRow(widget);
            else
                m_layout->addRow(label, widget);
        }
    }
}

void SettingGroup::OnValueChanged(const QString &val)
{
    QObject *obj = sender();
    QString name = obj->objectName();
    Settings *settings = SINGLE_INSTANCE_OBJ(Settings);
    if(instanceofv(obj, QSpinBox))
    {
        settings->SetSetting<int>(name, val.toInt());
    }
    else if(instanceofv(obj, QDoubleSpinBox))
    {
        settings->SetSetting<double>(name, val.toDouble());
    }
}

void SettingGroup::OnBoolChanged(bool b)
{
    QObject *obj = sender();
    QString name = obj->objectName();
    Settings *settings = SINGLE_INSTANCE_OBJ(Settings);
    settings->SetSetting<bool>(name, b);
}

void SettingGroup::OnIntChanged(int i)
{
    QObject *obj = sender();
    QString name = obj->objectName();
    Settings *settings = SINGLE_INSTANCE_OBJ(Settings);
    if(instanceofv(obj, QComboBox))
    {
        QComboBox *cb = static_cast<QComboBox *>(obj);
        settings->SetSetting<int>(name, cb->itemData(i).toInt());
    }
    else if(instanceofv(obj, QButtonGroup))
    {
        QButtonGroup *bg = static_cast<QButtonGroup *>(obj);
        QAbstractButton *btn = bg->button(i);
        int cs = settings->GetSetting<int>(name);
        if(btn->isChecked())
        {
            cs |= i;
        }
        else
        {
            cs &= ~i;
        }
        settings->SetSetting<int>(name, cs);
    }
}

void SettingGroup::OnItemDestroy(QObject *obj)
{
    QObject *o = obj ? obj : sender();
    if(o)
        DEBUG_DESTROY_QQV(obj)
}

void SettingGroup::ChooseColor()
{
    QObject *s = sender();
    if(!s)
        return;
    QWidget *w = dynamic_cast<QWidget *>(s);
    if(!w)
        return;
    Settings *settings = SINGLE_INSTANCE_OBJ(Settings);
    QString name = s->objectName();
    QVariant va = s->property("color");
    QColor color = QColorDialog::getColor(QColor(va.toString()), w);
    QString c(color.name());
    if(instanceofv(w, QPushButton))
    {
        QPushButton *button = static_cast<QPushButton *>(w);
        button->setText(c.toUpper());
        button->setStyleSheet(QString("QPushButton { color: %1; }").arg(c));
    }
    settings->SetSetting<QString>(name, c);
}
