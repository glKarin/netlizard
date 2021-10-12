#include "settings.h"

#include <QDebug>
#include <QStringList>

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

        _settingsConfig.insert("2d_control", SettingItem("CONTROL_2D/trans_sens", 1000, "Translate sensitive", "int").AddProp("min", 1).AddProp("max", 10000));
        _settingsConfig.insert("2d_control", SettingItem("CONTROL_2D/rot_sens", 100, "Rotation sensitive", "int").AddProp("min", 1).AddProp("max", 10000));
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
