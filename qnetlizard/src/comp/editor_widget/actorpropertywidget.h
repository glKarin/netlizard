#ifndef _KARIN_ACTORPROPERTYWIDGET_H
#define _KARIN_ACTORPROPERTYWIDGET_H

#include <QDockWidget>

class NLActorWidget;
class NLActor;

class ActorPropertyWidget : public QDockWidget
{
    Q_OBJECT
public:
    explicit ActorPropertyWidget(QWidget *parent = 0);
    virtual ~ActorPropertyWidget();
    NLActor * Actor();
    void Reset();
    
signals:
    
public slots:
    void SetActor(NLActor *actor);
    
protected Q_SLOTS:

private:
    void Init();

private:
    NLActor *m_actor;
    NLActorWidget *m_actorWidget;

    Q_DISABLE_COPY(ActorPropertyWidget)
};

#endif // _KARIN_ACTORPROPERTYWIDGET_H
