#include "settings.h"

#include <QDebug>
#include <QStringList>

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

const SettingItemMap & Settings::SettingsConfig()
{
    static SettingItemMap _settingsConfig;
    if(_settingsConfig.isEmpty())
    {
        _settingsConfig.insert("3d_control", SettingItem("CONTROL_3D/move_sens", 1800, "Move sensitive", "int").AddProp("min", 1).AddProp("max", 10000));
        _settingsConfig.insert("3d_control", SettingItem("CONTROL_3D/turn_sens", 180, "Turn sensitive", "int").AddProp("min", 1).AddProp("max", 1000));
        _settingsConfig.insert("3d_control", SettingItem("CONTROL_3D/freelook_sens", 0.5, "Freelook sensitive", "float").AddProp("min", 0.01).AddProp("max", 10));
        _settingsConfig.insert("3d_control", SettingItem("CONTROL_3D/fovy_sens", 0.01, "Fovy sensitive", "float").AddProp("min", 0.01).AddProp("max", 0.1));

        _settingsConfig.insert("2d_control", SettingItem("CONTROL_2D/trans_sens", 1000, "Translate sensitive", "int").AddProp("min", 1).AddProp("max", 10000));
        _settingsConfig.insert("2d_control", SettingItem("CONTROL_2D/rot_sens", 100, "Rotation sensitive", "int").AddProp("min", 1).AddProp("max", 10000));

        _settingsConfig.insert("render", SettingItem("RENDER/fps", 0, "FPS(0 to not limit)", "int").AddProp("min", 0).AddProp("max", 300));
        _settingsConfig.insert("render", SettingItem("RENDER/scene_cull", true, "Cull map scene", "bool"));
        QVariantList list;
        QVariantMap map;
        map.insert("name", "No shadow");
        map.insert("value", 0);
        list << map;
        map.insert("name", "Stencil shadow: Z-FAIL");
        map.insert("value", 2);
        list << map;
        map.insert("name", "Stencil shadow: Z-PASS");
        map.insert("value", 1);
        list << map;
        _settingsConfig.insert("render", SettingItem("RENDER/shadow", SHADOW_Z_FAIL, "Render simple shadow", "option").AddProp("items", list));

        list.clear();
        map.clear();
//        map.insert("name", "No render shadow");
//        map.insert("value", NETLIZARD_SHADOW_RENDER_NONE);
//        list << map;
        map.insert("name", "Only render item shadow");
        map.insert("value", NETLIZARD_SHADOW_RENDER_ITEM);
        list << map;
        map.insert("name", "Only render scene wall shadow");
        map.insert("value", NETLIZARD_SHADOW_RENDER_SCENE_WALL);
        list << map;
        map.insert("name", "Only render scene shadow");
        map.insert("value", NETLIZARD_SHADOW_RENDER_SCENE);
        list << map;
        map.insert("name", "Only render item and scene wall shadow");
        map.insert("value", NETLIZARD_SHADOW_RENDER_ITEM | NETLIZARD_SHADOW_RENDER_SCENE_WALL);
        list << map;
        map.insert("name", "Render all shadow");
        map.insert("value", NETLIZARD_SHADOW_RENDER_ALL);
        list << map;
        _settingsConfig.insert("render", SettingItem("RENDER/shadow_object", NETLIZARD_SHADOW_RENDER_ITEM, "Render shadow object", "option").AddProp("items", list));
    }
    return _settingsConfig;
}

QVariant Settings::GetDefaultSetting(const QString &name)
{
    const SettingItemMap &_settingsConfig = SettingsConfig();
    QList<SettingItem> values = _settingsConfig.values();
    Q_FOREACH(const SettingItem &item, values)
    {
        if(item.name == name)
            return item.value;
    }
    return QVariant();
}
