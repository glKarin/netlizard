#ifndef _KARIN_SETTINGWIDGET_H
#define _KARIN_SETTINGWIDGET_H

#include <QWidget>

#include "settings.h"
#include "nlpropformgroupwidget.h"

class SettingsPropFormGroupWidget : public NLPropFormGroupWidget
{
    Q_OBJECT
public:
    explicit SettingsPropFormGroupWidget(const Settings::SettingItemCategory *c, QWidget *widget = 0);
    explicit SettingsPropFormGroupWidget(const Settings::SettingItemCategory *c, const QString &title, QWidget *widget = 0);
    virtual ~SettingsPropFormGroupWidget();

protected:
    virtual void SetObjectProperty(QObject *obj, const QString &name, const QVariant &value);
    virtual void CoverObjectProperty(QObject *obj, const QString &name, const QVariant &value);
    virtual NLPropertyInfoList GetPropertyInfoList(QObject *obj);
    virtual void SortProperties(NLPropertyInfoList &list);

private:
    const Settings::SettingItemCategory *m_sc;
};


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
