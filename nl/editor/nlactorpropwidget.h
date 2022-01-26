#ifndef _KARIN_NLACTORPROPWIDGET_H
#define _KARIN_NLACTORPROPWIDGET_H

#include <QScrollArea>

#include "widget/nlpropformgroupwidget.h"

class NLActor;
class NLObject;
class NLComponent;
class NLScript;
class NLPropSection;

class NLObjectPropFormGroupWidget : public NLPropFormGroupWidget
{
    Q_OBJECT
public:
    explicit NLObjectPropFormGroupWidget(QWidget *widget = 0);
    explicit NLObjectPropFormGroupWidget(const QString &title, QWidget *widget = 0);
    virtual ~NLObjectPropFormGroupWidget();
    virtual void SetObject(QObject *obj);

protected:
    virtual void SetObjectProperty(QObject *obj, const QString &name, const QVariant &value);
    virtual void CoverObjectProperty(QObject *obj, const QString &name, const QVariant &value);
    virtual NLPropertyInfoList GetPropertyInfoList(QObject *obj);
    virtual void SortProperties(QObject *obj, NLPropertyInfoList &list);
    virtual QVariant GetObjectProperty(QObject *obj, const QString &name);
    virtual NLPropFormGroupWidget * GenFormGroup(QObject *obj);
    virtual void mousePressEvent(QMouseEvent *ev);
    virtual void mouseMoveEvent(QMouseEvent *ev);

private Q_SLOTS:
    void OnPropertyChanged(const QString &name, const QVariant &value, int action);

private:
    QPoint m_dragStartPosition;
};

class NLLIB_EXPORT NLActorPropWidget : public QScrollArea
{
    Q_OBJECT

public:
    explicit NLActorPropWidget(QWidget *widget = 0);
    virtual ~NLActorPropWidget();
    NLActor * Actor() { return m_actor; }
    void SetActor(NLActor *sctor);

public Q_SLOTS:
    void Reset();

protected:
    void UpdateActorData();

private:
    void Init();
    void SetupActorProperty();
    void SetupComponentProperties();
    void SetupComponentProperty(NLComponent *comp);
    void SortProperties(NLPropertyInfoList &list);
    void SetupScriptProperties();
    void SetupScriptProperty(NLScript *script);
    void ClearComponentProperties();
    void ClearScriptProperties();

private Q_SLOTS:
    void OnActorChanged();
    void SetupComponent();
    void SetupScript();
    void OnActionTriggered(QAction *action);

private:
    typedef QHash<NLObject *, NLPropFormGroupWidget *> ObjectPropWidgetHash;
    typedef QHash<QString, QList<NLObject *> > ObjectHash;
    NLActor *m_actor;
    NLPropFormGroupWidget *m_actorGroupBox;
    NLPropSection *m_componentSection;
    NLPropSection *m_scriptSection;
    ObjectPropWidgetHash m_propWidgetMap;
    ObjectHash m_objectMap;

    Q_DISABLE_COPY(NLActorPropWidget)
};

#endif // _KARIN_NLACTORPROPWIDGET_H
