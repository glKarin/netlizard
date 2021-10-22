#ifndef _KARIN_SETTINGWIDGET_H
#define _KARIN_SETTINGWIDGET_H

#include <QWidget>
#include <QGroupBox>

class QFormLayout;

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

Q_SIGNALS:
    void valueChanged(const QString &name, const QVariant &val);

private Q_SLOTS:
    void OnValueChanged(const QString &val);

private:
    QString m_name;
    QFormLayout *m_layout;

    Q_DISABLE_COPY(SettingGroup)
};


#endif // _KARIN_SETTINGWIDGET_H
