#ifndef _KARIN_NLPROPFORMGROUPWIDGET_H
#define _KARIN_NLPROPFORMGROUPWIDGET_H

#include <QScrollArea>
#include <QGroupBox>

#include "nldef.h"
#include "nlsequencemap.h"
#include "nlproperties.h"

class QFormLayout;
class QToolButton;

class NLFormGroupWidget : public QGroupBox
{
    Q_OBJECT
public:
    explicit NLFormGroupWidget(QWidget *widget = 0);
    explicit NLFormGroupWidget(const QString &title, QWidget *widget = 0);
    virtual ~NLFormGroupWidget();
    void AddRow(const QString &name, QWidget *widget);
    QFormLayout * Layout() { return m_layout; }
    void AddAction(QAction *action);
    QVariant Data() const { return m_data; }
    void SetData(const QVariant &data);
    QWidget * WidgetForName(const QString &name) { return m_nameWidgetMap.value(name); }

Q_SIGNALS:
    void actionTriggered(QAction *action);

public Q_SLOTS:
    virtual void Reset();

protected:
    virtual void resizeEvent(QResizeEvent *event);

private Q_SLOTS:
    void ToggleGroupBox(bool on);

private:
    void Init();

private:
    typedef NLSequenceHash<QString, QWidget *> WidgetHash;
    QFormLayout *m_layout;
    QToolButton *m_actionButton;
    WidgetHash m_nameWidgetMap;
    bool m_expand;
    QVariant m_data;

    Q_DISABLE_COPY(NLFormGroupWidget)
};

class NLPropFormGroupWidget : public NLFormGroupWidget
{
    Q_OBJECT
public:
    explicit NLPropFormGroupWidget(QWidget *widget = 0);
    explicit NLPropFormGroupWidget(const QString &title, QWidget *widget = 0);
    virtual ~NLPropFormGroupWidget();
    void SetObject(QObject *obj);
    QObject * Object() { return m_object; }

public Q_SLOTS:
    virtual void Reset();
    void NotifyPropertyChanged(const QString &name, const QVariant &value);
    void OnObjectChanged();

protected:
    virtual QWidget * GenWidget(QObject *obj, const NLPropertyInfo &prop);
    virtual void SetObjectProperty(QObject *obj, const QString &name, const QVariant &value) = 0;
    virtual void CoverObjectProperty(QObject *obj, const QString &name, const QVariant &value) = 0;
    virtual NLPropertyInfoList GetPropertyInfoList(QObject *obj) = 0;
    virtual void SortProperties(NLPropertyInfoList &list) { Q_UNUSED(list); }

private:
    void SetupObjectProperty();

private Q_SLOTS:
    void OnItemDestroy(QObject *obj);
    void OnIntChanged(int i);
    void OnBoolChanged(bool b);
    void OnDoubleChanged(double f);
    void OnVector3Changed(const NLVector3 &v);
    void OnIndexChanged(int i);
    void OnStringChanged(const QString &str = QString());
    void OnStringReload(const QString &str = QString());
    void OnColorChanged(const QColor &color);

private:
    QObject *m_object;

    Q_DISABLE_COPY(NLPropFormGroupWidget)
};

#endif // _KARIN_NLPROPFORMGROUPWIDGET_H
