#include "scenetreeinfowidget.h"

#include <QDebug>

#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>

#include "nlactor.h"
#include "nlscene.h"
#include "nlsceneinfowidget.h"
#include "nlscenetreewidget.h"
#include "qdef.h"

SceneTreeInfoWidget::SceneTreeInfoWidget(QWidget *parent) :
    QDockWidget(parent),
    m_scene(0),
    m_infoWidget(0),
    m_treeWidget(0)
{
    setObjectName("SceneTreeInfoWidget");
    Init();
}

SceneTreeInfoWidget::~SceneTreeInfoWidget()
{
    DEBUG_DESTROY_Q;
}


void SceneTreeInfoWidget::SetScene(NLScene *scene)
{
    if(m_scene != scene)
    {
        Reset();
        m_scene = scene;
        m_treeWidget->SetScene(m_scene);
        m_infoWidget->SetScene(m_scene);
//        if(m_scene)
//        {
//            connect(m_scene, SIGNAL(updated(float)), this, SLOT(UpdateSceneInfo()));
//            connect(m_scene, SIGNAL(destroyed()), this, SLOT(Reset()));
//        }
    }
}

void SceneTreeInfoWidget::Reset()
{
    m_scene = 0;
    m_treeWidget->Reset();
    m_infoWidget->Reset();
}

void SceneTreeInfoWidget::Init()
{
    m_treeWidget = new NLSceneTreeWidget;
    m_infoWidget = new NLSceneInfoWidget;
    QVBoxLayout *mainLayout = new QVBoxLayout;
    QGroupBox *box;
    QVBoxLayout *layout;
    QWidget *root = new QWidget;

    box = new QGroupBox;
    layout = new QVBoxLayout;
    box->setCheckable(false);
    box->setTitle("Scene actors");
    layout->addWidget(m_treeWidget, 1);
    box->setLayout(layout);
    mainLayout->addWidget(box, 1);

    box = new QGroupBox;
    layout = new QVBoxLayout;
    box->setCheckable(false);
    box->setTitle("Scene info");
    layout->addWidget(m_infoWidget, 1);
    box->setLayout(layout);
    mainLayout->addWidget(box);

    connect(m_treeWidget, SIGNAL(actorSelected(NLActor*)), this, SIGNAL(actorSelected(NLActor*)));

    root->setLayout(mainLayout);
    setWidget(root);
    setWindowTitle("Scene tree viewer");
    //setMaximumWidth(360);
}
