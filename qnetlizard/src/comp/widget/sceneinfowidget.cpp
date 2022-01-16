#include "sceneinfowidget.h"

#include <QDebug>

#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>

#include "engine/nlactor.h"
#include "engine/nlscene.h"
#include "editor/nlsceneinfowidget.h"
#include "qdef.h"

SceneInfoWidget::SceneInfoWidget(QWidget *parent) :
    QDockWidget(parent),
    m_scene(0),
    m_infoWidget(0)
{
    setObjectName("SceneInfoWidget");
    Init();
}

SceneInfoWidget::~SceneInfoWidget()
{
    DEBUG_DESTROY_Q;
}


void SceneInfoWidget::SetScene(NLScene *scene)
{
    if(m_scene != scene)
    {
        Reset();
        m_scene = scene;
        m_infoWidget->SetScene(m_scene);
    }
}

void SceneInfoWidget::Reset()
{
    m_scene = 0;
    m_infoWidget->Reset();
}

void SceneInfoWidget::Init()
{
    m_infoWidget = new NLSceneInfoWidget;
    setWidget(m_infoWidget);
    setWindowTitle(tr("Scene infomation"));
    //setMaximumWidth(360);
}
