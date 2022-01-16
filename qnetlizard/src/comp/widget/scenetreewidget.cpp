#include "scenetreewidget.h"

#include <QDebug>

#include "engine/nlactor.h"
#include "engine/nlscene.h"
#include "editor/nlscenetreewidget.h"
#include "qdef.h"

SceneTreeWidget::SceneTreeWidget(QWidget *parent) :
    QDockWidget(parent),
    m_scene(0),
    m_treeWidget(0)
{
    setObjectName("SceneTreeWidget");
    Init();
}

SceneTreeWidget::~SceneTreeWidget()
{
    DEBUG_DESTROY_Q;
}


void SceneTreeWidget::SetScene(NLScene *scene)
{
    if(m_scene != scene)
    {
        Reset();
        m_scene = scene;
        m_treeWidget->SetScene(m_scene);
    }
}

void SceneTreeWidget::Reset()
{
    m_scene = 0;
    m_treeWidget->Reset();
}

void SceneTreeWidget::Init()
{
    m_treeWidget = new NLSceneTreeWidget;

    connect(m_treeWidget, SIGNAL(actorSelected(NLActor*)), this, SIGNAL(actorSelected(NLActor*)));

    setWidget(m_treeWidget);
    setWindowTitle(tr("Scene tree"));
    //setMaximumWidth(360);
}
