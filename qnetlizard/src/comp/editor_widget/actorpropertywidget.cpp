#include "actorpropertywidget.h"

#include <QDebug>

#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>

#include "nlactor.h"
#include "nlscene.h"
#include "nlactorwidget.h"
#include "qdef.h"

ActorPropertyWidget::ActorPropertyWidget(QWidget *parent) :
    QDockWidget(parent),
    m_actor(0),
    m_actorWidget(0)
{
    setObjectName("ActorPropertyWidget");
    Init();
}

ActorPropertyWidget::~ActorPropertyWidget()
{
    DEBUG_DESTROY_Q;
}


void ActorPropertyWidget::SetActor(NLActor *actor)
{
    if(m_actor != actor)
    {
        Reset();
        m_actor = actor;
        m_actorWidget->SetActor(m_actor);
//        if(m_actor)
//        {
//            connect(m_actor, SIGNAL(updated(float)), this, SLOT(UpdateSceneInfo()));
//            connect(m_actor, SIGNAL(destroyed()), this, SLOT(Reset()));
//        }
    }
}

void ActorPropertyWidget::Reset()
{
    m_actorWidget->Reset();
}

void ActorPropertyWidget::Init()
{
    m_actorWidget = new NLActorWidget;
    setWidget(m_actorWidget);
    setWindowTitle("Actor property viewer");
    //setMaximumWidth(360);
}
