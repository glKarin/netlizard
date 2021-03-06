#ifndef _KARIN_ACTORPROPERTYWIDGET_H
#define _KARIN_ACTORPROPERTYWIDGET_H

#include <QDockWidget>

class NLActorPropWidget;
class NLActor;

class ActorPropertyWidget : public QDockWidget
{
    Q_OBJECT
public:
    explicit ActorPropertyWidget(QWidget *parent = 0);
    virtual ~ActorPropertyWidget();
    NLActor * Actor() { return m_actor; }
    void Reset();
    
signals:
    
public slots:
    void SetActor(NLActor *actor);
    
protected Q_SLOTS:

private:
    void Init();

private:
    NLActor *m_actor;
    NLActorPropWidget *m_actorWidget;

    Q_DISABLE_COPY(ActorPropertyWidget)
};

#endif // _KARIN_ACTORPROPERTYWIDGET_H
