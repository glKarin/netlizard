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
#include <QAction>

#include "qdef.h"
#include "utils/nlguiutility.h"
#include "lang.h"

SettingsPropFormGroupWidget::SettingsPropFormGroupWidget(const Settings::SettingItemCategory *c, QWidget *widget)
    : NLPropFormGroupWidget(widget),
      m_sc(c)
{
    setObjectName("SettingsPropFormGroupWidget");
}

SettingsPropFormGroupWidget::SettingsPropFormGroupWidget(const Settings::SettingItemCategory *c, const QString &title, QWidget *widget)
    : NLPropFormGroupWidget(title, widget),
      m_sc(c)
{
    setObjectName("SettingsPropFormGroupWidget");
}

SettingsPropFormGroupWidget::~SettingsPropFormGroupWidget()
{
    m_sc = 0;
}

QVariant SettingsPropFormGroupWidget::GetObjectProperty(QObject *obj, const QString &name)
{
    return static_cast<Settings *>(obj)->operator[](name);
}

void SettingsPropFormGroupWidget::SetObjectProperty(QObject *obj, const QString &name, const QVariant &value)
{
    static_cast<Settings *>(obj)->operator()(name, value);
}

void SettingsPropFormGroupWidget::CoverObjectProperty(QObject *obj, const QString &name, const QVariant &value)
{
    static_cast<Settings *>(obj)->operator()(name, value);
}

bool SettingsPropFormGroupWidget::HandleAction(QAction *action)
{
    if(action->property("_Type").toString() == "link")
    {
        SettingGroup *p = static_cast<SettingGroup *>(Data().value<QWidget *>());
        emit p->openSettingGroup(action->data().toString());
        return true;
    }
    return false;
}

NLPropertyInfoList SettingsPropFormGroupWidget::GetPropertyInfoList(QObject *obj)
{
    NLPropertyInfoList list;
    Settings *settings = static_cast<Settings *>(obj);
    const LangHelper lang("SETTING");

    Q_FOREACH(const Settings::SettingItem *si, m_sc->settings)
    {
        if(!si->IsValid())
            continue;

        if(si->item_type == Settings::SettingItem::Item_Category)
        {
            NLPropertyInfo info(si->name, QString("<a href='%1'>%2 &gt;&gt;</a>").arg(si->name).arg(lang[si->title]), "QString", "label", false, QVariant(), QVariantHash(), lang[si->title], lang[si->description]);
            list.push_back(info);
        }
        else
        {
            const Settings::SettingItemContent &item = static_cast<const Settings::SettingItemContent &>(*si);
            NLProperties props;
            if(item.widget == "spinbox")
            {
                if(item.prop.contains("min") || item.prop.contains("max"))
                {
                    NLProperties range;
                    if(item.prop.contains("min"))
                        range.Insert("min", item.prop.value("min"));
                    if(item.prop.contains("max"))
                        range.Insert("max", item.prop.value("max"));
                    props.Insert("range", range);
                }
            }
            else if(item.widget == "combobox")
            {
                NLPropertyPairList items;

                QVariantList options = item.prop.value("option").toList();
                for(int i = 0; i < options.size(); i++)
                {
                    QVariantHash p = options[i].toHash();
                    items.push_back(NLPropertyPair(lang[p.value("label").toString()], p.value("value")));
                }
                props.Insert("enum", QVariant::fromValue<NLPropertyPairList>(items));
            }
            else if(item.widget == "checkbox")
            {
                if(item.type == "int")
                {
                    NLPropertyPairList items;

                    QVariantList options = item.prop.value("option").toList();
                    for(int i = 0; i < options.size(); i++)
                    {
                        QVariantHash p = options[i].toHash();
                        items.push_back(NLPropertyPair(lang[p.value("label").toString()], p.value("value")));
                    }
                    props.Insert("option", QVariant::fromValue<NLPropertyPairList>(items));
                }
            }

            QVariant value = settings->operator[](item.name);
            NLPropertyInfo info(item.name, value, item.type, item.widget, false, item.value, props, lang[item.title], lang[item.description]);
            list.push_back(info);
        }
    }
    return list;
}

void SettingsPropFormGroupWidget::SortProperties(NLPropertyInfoList &list)
{
    //qSort(list.begin(), list.end(), NLPropertyInfoCmp);
}


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
    QStackedLayout *m_layout = new QStackedLayout;
    setLayout(m_layout);
}

void SettingGroup::Clear()
{
    setTitle(QString());
    QLayout *l = layout();
    NLGUIUtility::ClearLayout(l);
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
                QTabWidget *tabWidget = new QTabWidget;
                layout()->addWidget(tabWidget);
                const LangHelper lang("SETTING");
                Q_FOREACH(const Settings::SettingItem *cc, sc->settings)
                {
                    if(cc->item_type != Settings::SettingItem::Item_Category)
                        continue;
                    SettingsPropFormGroupWidget *widget = GenSettingConfig(tabWidget, static_cast<const Settings::SettingItemCategory *>(cc));
                    tabWidget->addTab(widget, lang[cc->title]);
                }
            }
            else
            {
                SettingsPropFormGroupWidget *widget = GenSettingConfig(this, sc);
                layout()->addWidget(widget);
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

SettingsPropFormGroupWidget * SettingGroup::GenSettingConfig(QWidget *parent, const Settings::SettingItemCategory *c)
{
    SettingsPropFormGroupWidget *form = new SettingsPropFormGroupWidget(c, parent);
    form->SetCanExpand(false);
    form->SetData(QVariant::fromValue<QWidget *>(this));
    Settings *settings = SINGLE_INSTANCE_OBJ(Settings);
    form->SetObject(settings);
    return form;
}

