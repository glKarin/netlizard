#ifndef _KARIN_SETTINGS_H
#define _KARIN_SETTINGS_H

#include <QSettings>

class Settings : public QObject
{
    Q_OBJECT
public:
    struct SettingItem
    {
        enum ItemType_e
        {
            Item_Invalid = 0,
            Item_Category = 1,
            Item_Content = 2
        };

        QString name;
        QString title;
        ItemType_e item_type;
        QString description;
        SettingItem(const QString &n = QString(), const QString &t = QString(), ItemType_e c = Item_Invalid, const QString &d = QString())
            : name(n),
              title(t),
              item_type(c),
              description(d)
        {
        }
        virtual ~SettingItem();

        bool IsValid() const { return item_type != Item_Invalid; }
        void Invalid() { item_type = Item_Invalid; }
    };

    struct SettingItemContent : public SettingItem
    {
        QVariant value;
        QString type;
        QString widget;
        QVariantHash prop;
        SettingItemContent(const QString &n = QString(), const QString &t = QString(), const QString &c = QString(), const QString &w = QString(), const QVariant &val = QVariant(), const QString &d = QString())
            : SettingItem(n, t, SettingItem::Item_Content, d),
              value(val),
              type(c),
              widget(w)
        {
        }
        virtual ~SettingItemContent();
        SettingItem & operator()(const QString &name, const QVariant &val)
        {
            prop.insert(name, val);
            return *this;
        }
    };
    typedef QList<SettingItem *> SettingItemList;

    struct SettingItemCategory : public SettingItem
    {
        SettingItemList settings;
        SettingItemCategory(const QString &n = QString(), const QString &t = QString(), const QString &d = QString())
            : SettingItem(n, t, SettingItem::Item_Category, d)
        {
        }
        virtual ~SettingItemCategory();

        SettingItemCategory & operator<<(SettingItem *item)
        {
            settings.push_back(item);
            return *this;
        }

    private:
        Q_DISABLE_COPY(SettingItemCategory)
    };

    typedef QList<SettingItemCategory *> SettingItemMap;

public:
    virtual ~Settings();
    static Settings * Instance();
    static const SettingItemMap & SettingsConfig();
    template<class T> T GetSetting(const QString &name, const T &def = T());
    template<class T> void SetSetting(const QString &name, const T &val);
    QVariant operator[](const QString &name);
    void operator()(const QString &name, const QVariant &val);
    QVariant GetDefaultSetting(const QString &name);
    
signals:
    void settingChanged(const QString &name, const QVariant &value, const QVariant &oldValue);
    
public slots:

private:
    static void ClearSettingsConfig();
    explicit Settings(QObject *parent = 0);
    static bool LoadSettings(SettingItemMap &map);
    QVariant FindDefaultSetting(const Settings::SettingItemCategory *c, const QString &name);

private:
    QSettings *m_settings;

    static SettingItemMap _settingsConfig;
    
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
