#ifndef _KARIN_SETTINGWIDGET_H
#define _KARIN_SETTINGWIDGET_H

#include <QWidget>
#include <QGroupBox>

#include "settings.h"

class SettingGroup : public QGroupBox
{
    Q_OBJECT
public:
    explicit SettingGroup(QWidget *parent = 0);
    virtual ~SettingGroup();
    void SetSettingConfig(const QString &name, const QString &title = QString());
    void Clear();

private:
    void Init();
    void GenSettingConfig(QWidget *parent, const Settings::SettingItemCategory *c);
    bool IsGroupSettingConfig(const Settings::SettingItemCategory *c);
    const Settings::SettingItemCategory * FindSettingConfig(const Settings::SettingItemCategory *c, const QString &name);

Q_SIGNALS:
    void valueChanged(const QString &name, const QVariant &val);
    void openSettingGroup(const QString &name);

private Q_SLOTS:
    void OnValueChanged(const QString &val);
    void OnBoolChanged(bool b);
    void OnIntChanged(int i);
    void OnItemDestroy(QObject *item = 0);
    void ChooseColor();

private:
    QString m_name;

    Q_DISABLE_COPY(SettingGroup)
};


#endif // _KARIN_SETTINGWIDGET_H
