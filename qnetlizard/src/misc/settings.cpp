#include "settings.h"

#include <QStringList>
#include <QFile>
#include <QXmlSimpleReader>
#include <QXmlInputSource>
#include <QDomDocument>
#include <QDomNode>
#include <QDomNodeList>

#include "stencil_shadow.h"
#include "nl_shadow_render.h"

static Settings::SettingItemContent * LoadSettingContents(const QDomElement &item)
{
    Settings::SettingItemContent *si = 0;
    if(item.tagName() != "item")
    {
        return si;
    }

    si = new Settings::SettingItemContent;
    si->title = item.attribute("name");
    si->name = item.attribute("alias");
    si->type = item.attribute("type");
    si->value = item.attribute("default-value");
    si->widget = item.attribute("widget");
    si->description = item.attribute("description");

    QDomNodeList options = item.childNodes();
    QMap<QString, QList<QVariantHash> > op;
    for(int k = 0; k < options.size(); k++)
    {
        QDomNode node = options.at(k);
        if(!node.isElement())
            continue;
        QDomElement item = node.toElement();
        if(item.tagName() != "option")
            continue;

        QString o = item.attribute("name");
        QDomNamedNodeMap nm = item.attributes();
        QVariantHash l;
        for(int m = 0; m < nm.size(); m++)
        {
            QDomNode node = nm.item(m);
            if(!node.isAttr())
                continue;
            QDomAttr attr = node.toAttr();
            l.insert(attr.name(), attr.value());
        }
        op[o].push_back(l);
    }

    Q_FOREACH(const QString &nn, op.keys())
    {
        const QList<QVariantHash> &oo = op.value(nn);
        if(oo.size() == 1)
        {
            const QVariantHash &vh = oo[0];
            (*si)(vh.value("name").toString(), vh.value("value"));
        }
        else if(oo.size() > 1)
        {
            const QVariantHash &vh = oo[0];
            QVariantList vl;
            Q_FOREACH(const QVariantHash &h, oo)
            {
                vl.push_back(QVariant::fromValue(h));
            }

            (*si)(vh.value("name").toString(), QVariant::fromValue(vl));
        }
    }
    return si;
}

static Settings::SettingItemCategory * LoadSettingCategories(const QDomElement &category)
{
    Settings::SettingItemCategory *sc = 0;
    if(category.tagName() != "category")
    {
        return sc;
    }

    sc = new Settings::SettingItemCategory;
    sc->title = category.attribute("name");
    sc->name = category.attribute("alias");
    sc->description = category.attribute("description");

    QDomNodeList items = category.childNodes();
    for(int j = 0; j < items.size(); j++)
    {
        QDomNode node = items.at(j);
        if(!node.isElement())
            continue;
        QDomElement item = node.toElement();
        if(item.tagName() == "item")
        {
            Settings::SettingItemContent *si = LoadSettingContents(item);
            if(si)
                *sc << si;
        }
        else if(item.tagName() == "category")
        {
            Settings::SettingItemCategory *si = LoadSettingCategories(item);
            if(si)
                *sc << si;
        }
    }

    return sc;
}

Settings::Settings(QObject *parent) :
    QObject(parent),
    m_settings(new QSettings(APP_NAME ".ini", QSettings::IniFormat, this))
{
    setObjectName("Settings");
}

Settings::~Settings()
{
    ClearSettingsConfig();
    DEBUG_DESTROY_Q
}


QVariant Settings::operator[](const QString &name)
{
    return GetSetting<QVariant>(name, GetDefaultSetting(name));
}

SINGLE_INSTANCE_DECL(Settings)

Settings::SettingItemMap Settings::_settingsConfig;

const Settings::SettingItemMap & Settings::SettingsConfig()
{
    if(_settingsConfig.isEmpty())
    {
        LoadSettings(_settingsConfig);
    }
    return _settingsConfig;
}

void Settings::ClearSettingsConfig()
{
    if(_settingsConfig.isEmpty())
        return;
    Settings::SettingItemMap map = _settingsConfig;
    _settingsConfig.clear();
    Q_FOREACH(SettingItemCategory *c, map)
    {
        delete c;
    }
}

QVariant Settings::GetDefaultSetting(const QString &name)
{
    const Settings::SettingItemMap &_settingsConfig = SettingsConfig();
    Q_FOREACH(const Settings::SettingItemCategory *c, _settingsConfig)
    {
        QVariant va = FindDefaultSetting(c, name);
        if(va.isValid())
            return va;
    }
    return QVariant();
}

QVariant Settings::FindDefaultSetting(const Settings::SettingItemCategory *c, const QString &name)
{
    Q_FOREACH(const Settings::SettingItem *item, c->settings)
    {
        if(!item->IsValid())
            continue;
        if(item->item_type == Settings::SettingItem::Item_Content)
        {
            if(item->name == name)
                return static_cast<const Settings::SettingItemContent *>(item)->value;
        }
        else if(item->item_type == Settings::SettingItem::Item_Category)
        {
            QVariant va = FindDefaultSetting(static_cast<const Settings::SettingItemCategory *>(item), name);
            if(va.isValid())
                return va;
        }
    }
    return QVariant();
}

bool Settings::LoadSettings(Settings::SettingItemMap &map)
{
    const QString Help(":/SETTING");

    QFile f(Help);
    QXmlSimpleReader reader;
    QXmlInputSource source(&f);
    QDomDocument doc;
    if(!doc.setContent(&source, &reader))
        return false;

    QDomElement setting = doc.documentElement();
    if (setting.tagName() != "setting")
        return false;

    QDomNodeList categorys = setting.childNodes();
    for(int i = 0; i < categorys.size(); i++)
    {
        QDomNode node = categorys.at(i);
        if(!node.isElement())
            continue;
        QDomElement category = node.toElement();
        if(category.tagName() != "category")
            continue;
        Settings::SettingItemCategory *sc = LoadSettingCategories(category);
        if(sc)
            map.push_back(sc);
    }
    return true;
}
