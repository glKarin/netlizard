#ifndef _KARIN_NLPROPFORMGROUPWIDGET_H
#define _KARIN_NLPROPFORMGROUPWIDGET_H

#include <QScrollArea>
#include <QGroupBox>

#include "nldef.h"
#include "nlsequencemap.h"
#include "nlproperties.h"

class QFormLayout;
class QToolButton;

class NLPropFormGroupWidget : public QGroupBox
{
    Q_OBJECT
public:
    explicit NLPropFormGroupWidget(QWidget *widget = 0);
    virtual ~NLPropFormGroupWidget();
    void AddRow(const QString &name, QWidget *widget);
    QFormLayout * Layout() { return m_layout; }
    void AddAction(QAction *action);
    QVariant Data() const { return m_data; }
    void SetData(const QVariant &data);
    void SetObject(QObject *obj);
    QObject * Object() { return m_object; }

Q_SIGNALS:
    void actionTriggered(QAction *action);

public Q_SLOTS:
    void Reset();
    void NotifyPropertyChanged(const QString &name, const QVariant &value);

protected:
    virtual void resizeEvent(QResizeEvent *event);
    virtual QWidget * GenWidget(QObject *obj, const NLPropertyInfo &prop);
    virtual void SetObjectProperty(QObject *obj, const QString &name, const QVariant &value) = 0;
    virtual void CoverObjectProperty(QObject *obj, const QString &name, const QVariant &value) = 0;
    virtual NLPropertyInfoList GetPropertyInfoList(QObject *obj) = 0;
    virtual void SortProperties(NLPropertyInfoList &list) { Q_UNUSED(list); }

private:
    void Init();
    void SetupObjectProperty();
    void OnObjectChanged();

private Q_SLOTS:
    void OnItemDestroy(QObject *obj);
    void ToggleGroupBox(bool on);
    void OnIntChanged(int i);
    void OnBoolChanged(bool b);
    void OnDoubleChanged(double f);
    void OnVector3Changed(const NLVector3 &v);
    void OnIndexChanged(int i);
    void OnStringChanged(const QString &str = QString());
    void OnStringReload(const QString &str = QString());

private:
    typedef NLSequenceHash<QString, QWidget *> WidgetHash;
    QFormLayout *m_layout;
    QToolButton *m_actionButton;
    WidgetHash m_nameWidgetMap;
    bool m_expand;
    QVariant m_data;
    QObject *m_object;

    Q_DISABLE_COPY(NLPropFormGroupWidget)
};

#endif // _KARIN_NLPROPFORMGROUPWIDGET_H
