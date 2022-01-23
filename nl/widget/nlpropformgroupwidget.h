#ifndef _KARIN_NLPROPFORMGROUPWIDGET_H
#define _KARIN_NLPROPFORMGROUPWIDGET_H

#include <QScrollArea>
#include <QGroupBox>

#include "engine/nldef.h"
#include "template/nlsequencemap.h"
#include "engine/nlproperties.h"

#define NL_FORM_WIDGET_DRAG_DROP_MIME "application/nl-property-variant"
#define NL_FORM_WIDGET_DRAG_DROP_DATA_KEY "_NL_property_variant"

class QFormLayout;
class QToolButton;
class QDrag;
class QMimeData;
class NLFormLabelWidget;

class NLLIB_EXPORT NLFormGroupWidget : public QGroupBox
{
    Q_OBJECT
public:
    explicit NLFormGroupWidget(QWidget *widget = 0);
    explicit NLFormGroupWidget(const QString &title, QWidget *widget = 0);
    virtual ~NLFormGroupWidget();
    void AddRow(const QString &name, QWidget *widget, const QString &desc = QString(), bool readonly = false);
    void AddRow(const QString &name, const QString &label, QWidget *widget, const QString &desc = QString(), bool readonly = false);
    QFormLayout * Layout() { return m_layout; }
    void AddAction(QAction *action);
    QVariant Data() const { return m_data; }
    void SetData(const QVariant &data);
    QWidget * WidgetForName(const QString &name) { return m_nameWidgetMap.value(name); }
    void SetCanExpand(bool b);
    bool IsExpand() const { return m_expand; }
    bool CanExpand() const { return m_canExpand; }
    bool AllowDragDrop() const { return m_dragDrop; }
    void SetAllowDragDrop(bool on);

Q_SIGNALS:
    void actionTriggered(QAction *action);

public Q_SLOTS:
    virtual void Reset();

protected:
    virtual void resizeEvent(QResizeEvent *event);
    void PushLayout(const QString &name, QWidget *widget, const QString &label = QString(), const QString &desc = QString(), bool readonly = false);
    virtual QDrag * Drag(const QString &name, QWidget *widget) { Q_UNUSED(name); Q_UNUSED(widget); return 0; }
    virtual bool Drop(const QMimeData *d, QWidget *widget) { Q_UNUSED(d); Q_UNUSED(widget); return false; }
    virtual bool CheckDragData(const QMimeData *d, QWidget *widget) { Q_UNUSED(d); Q_UNUSED(widget); return false; }

private Q_SLOTS:
    void ToggleGroupBox(bool on);
    void OnItemDestroyed();

private:
    void Init();
    void CreateLayout();

private:
    typedef NLSequenceHash<QString, QWidget *> WidgetHash;
    QFormLayout *m_layout;
    QToolButton *m_actionButton;
    WidgetHash m_nameWidgetMap;
    bool m_expand;
    bool m_canExpand;
    QVariant m_data;
    bool m_dragDrop;

    friend class NLFormLabelWidget;

    Q_DISABLE_COPY(NLFormGroupWidget)
};

class NLLIB_EXPORT NLPropFormGroupWidget : public NLFormGroupWidget
{
    Q_OBJECT
public:
    explicit NLPropFormGroupWidget(QWidget *widget = 0);
    explicit NLPropFormGroupWidget(const QString &title, QWidget *widget = 0);
    virtual ~NLPropFormGroupWidget();
    virtual void SetObject(QObject *obj);
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
    virtual QVariant GetObjectProperty(QObject *obj, const QString &name) = 0;
    virtual void SortProperties(NLPropertyInfoList &list) { Q_UNUSED(list); }
    virtual QWidget * GenUserWidget(QObject *obj, const NLPropertyInfo &prop) { Q_UNUSED(obj); Q_UNUSED(prop); return 0; }
    virtual bool HandleAction(QAction *action) { return false; }
    virtual QDrag * Drag(const QString &name, QWidget *widget);
    virtual bool Drop(const QMimeData *d, QWidget *widget);
    virtual bool CheckDragData(const QMimeData *d, QWidget *widget);

private:
    void SetupObjectProperty();
    QWidget * GenMemoryPointerField(const QVariant &value);

private Q_SLOTS:
    void OnItemDestroy(QObject *obj = 0);
    void OnIntChanged(int i);
    void OnBoolChanged(bool b);
    void OnDoubleChanged(double f);
    void OnVector3Changed(const NLVector3 &v);
    void OnIndexChanged(int i);
    void OnStringChanged(const QString &str = QString());
    void OnStringReload(const QString &str = QString());
    void OnColorChanged(const QColor &color);
    void OnLinkActivated(const QString &link);

private:
    QObject *m_object;

    Q_DISABLE_COPY(NLPropFormGroupWidget)
};

#endif // _KARIN_NLPROPFORMGROUPWIDGET_H
