#include "settings.h"

#include <QDebug>
#include <QStringList>
#include <QFile>
#include <QXmlSimpleReader>
#include <QXmlInputSource>
#include <QDomDocument>
#include <QDomNode>
#include <QDomNodeList>

#include "stencil_shadow.h"
#include "nl_shadow_render.h"

Settings::Settings(QObject *parent) :
    QObject(parent),
    m_settings(new QSettings(APP_NAME ".ini", QSettings::IniFormat, this))
{
    setObjectName("Settings");
}

Settings::~Settings()
{
    DEBUG_DESTROY_Q
}


QVariant Settings::operator[](const QString &name)
{
    return GetSetting<QVariant>(name, GetDefaultSetting(name));
}

SINGLE_INSTANCE_DECL(Settings)

const Settings::SettingItemMap & Settings::SettingsConfig()
{
    static Settings::SettingItemMap _settingsConfig;
    if(_settingsConfig.isEmpty())
    {
        LoadSettings(_settingsConfig);
    }
    return _settingsConfig;
}

QVariant Settings::GetDefaultSetting(const QString &name)
{
    const Settings::SettingItemMap &_settingsConfig = SettingsConfig();
    Q_FOREACH(const Settings::SettingItemCategory &c, _settingsConfig)
    {
        Q_FOREACH(const Settings::SettingItem &item, c.settings)
        {
            if(item.name == name)
                return item.value;
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

    QDomElement help = doc.documentElement();
    if (help.tagName() != "setting")
        return false;

    QDomNodeList categorys = help.childNodes();
    for(int i = 0; i < categorys.size(); i++)
    {
        QDomNode node = categorys.at(i);
        if(!node.isElement())
            continue;
        QDomElement category = node.toElement();
        if(category.tagName() != "category")
            continue;
        QString categoryTitle = category.attribute("name");
        QString categoryName = category.attribute("alias");
        QString categoryDesc = category.attribute("description");
        QString categoryCate = category.attribute("category");
        SettingItemCategory sc(categoryName, categoryTitle, categoryDesc);
        QDomNodeList items = category.childNodes();
        for(int j = 0; j < items.size(); j++)
        {
            QDomNode node = items.at(j);
            if(!node.isElement())
                continue;
            QDomElement item = node.toElement();
            if(item.tagName() != "item")
                continue;
            QString title = item.attribute("name");
            QString name = item.attribute("alias");
            QString type = item.attribute("type");
            QString value = item.attribute("default-value");
            QString widget = item.attribute("widget");
            QString description = item.attribute("description");
            Settings::SettingItem si(name, value, title, type, widget, description);

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
                    si.AddProp(vh.value("name").toString(), vh.value("value"));
                }
                else if(oo.size() > 1)
                {
                    const QVariantHash &vh = oo[0];
                    QVariantList vl;
                    Q_FOREACH(const QVariantHash &h, oo)
                    {
                        vl.push_back(QVariant::fromValue(h));
                    }

                    si.AddProp(vh.value("name").toString(), QVariant::fromValue(vl));
                }
            }
            sc << si;
        }
        map.push_back(sc);
    }
    return true;
}
