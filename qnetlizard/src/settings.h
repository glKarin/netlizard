#ifndef _KARIN_SETTINGS_H
#define _KARIN_SETTINGS_H

#include <QObject>
#include <QSettings>
#include <QVariant>

#include "qdef.h"

struct SettingItem {
    QString name;
    QVariant value;
    QString title;
    QString type;
    QVariantHash prop;
    SettingItem(const QString &n, const QVariant &val, const QString &t, const QString &c)
        : name(n),
          value(val),
          title(t),
          type(c)
    {
    }
    SettingItem & AddProp(const QString &name, const QVariant &val)
    {
        prop.insert(name, val);
        return *this;
    }
};

typedef QMultiHash<QString, SettingItem> SettingItemMap;

class Settings : public QObject
{
    Q_OBJECT
public:
    virtual ~Settings();
    SINGLE_INSTANCE_DEF(Settings);
    static const SettingItemMap & SettingsConfig();
    template<class T> T GetSetting(const QString &name, const T &def = T());
    template<class T> void SetSetting(const QString &name, const T &val);
    QVariant operator[](const QString &name);
    QVariant GetDefaultSetting(const QString &name);
    
signals:
    void settingChanged(const QString &name, const QVariant &value, const QVariant &oldValue);
    
public slots:

private:
    explicit Settings(QObject *parent = 0);

private:
    QSettings *m_settings;
    
    Q_DISABLE_COPY(Settings)
};

template<class T> T Settings::GetSetting(const QString &name, const T &def)
{
    if(!m_settings->contains(name))
        SetSetting<T>(name, def);
    return m_settings->value(name).value<T>();
}

template<class T> void Settings::SetSetting(const QString &name, const T &val)
{
    QVariant v = QVariant::fromValue(val);
    QVariant old = m_settings->value(name);
    if(!m_settings->contains(name) || v != old)
    {
        m_settings->setValue(name, v);
        emit settingChanged(name, v, old);
    }
}

#endif // _KARIN_SETTINGS_H
