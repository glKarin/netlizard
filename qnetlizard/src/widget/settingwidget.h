#ifndef _KARIN_SETTINGWIDGET_H
#define _KARIN_SETTINGWIDGET_H

#include <QWidget>
#include <QGroupBox>

class QLabel;
class QBoxLayout;

class SettingGroupItem : public QWidget
{
    Q_OBJECT
public:
    virtual ~SettingGroupItem();
    static SettingGroupItem * Make(const QString &title, QWidget *widget);
    void SetWidget(QWidget *widget = 0);
    void SetTitle(const QString &title);
    void Set(const QString &title, QWidget *widget = 0);
    void Clear();

private:
    void Init();

private:
    QLabel *m_label;
    QWidget *m_widget;
    QBoxLayout *m_layout;
    explicit SettingGroupItem(QWidget *parent = 0);

    Q_DISABLE_COPY(SettingGroupItem)
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

Q_SIGNALS:
    void valueChanged(const QString &name, const QVariant &val);

private Q_SLOTS:
    void OnValueChanged(const QString &val);

private:
    QString m_name;
    QBoxLayout *m_layout;

    Q_DISABLE_COPY(SettingGroup)
};


#endif // _KARIN_SETTINGWIDGET_H
