#include "testscene.h"

#include <QDebug>
#include <QByteArray>

#include "qdef.h"
#include "simplecameraactor.h"
#include "nlmodelrenderer.h"
#include "settings.h"

TestScene::TestScene(QWidget *parent)
    : NLScene(parent),
      m_renderer(0)
{
    setObjectName("TestScene");

    SimpleCameraActor *camera = new SimpleCameraActor;
    camera->setObjectName("Test_SimpleCameraActor");
    AddActor(camera);
    NLActor *actor = new NLActor;
    actor->setObjectName("Test_RootRenderActor");
    AddActor(actor);
    m_renderer = new NLModelRenderer_coordinate(9999);
    actor->SetRenderable(m_renderer);
    SetCurrentCamera(camera->Camera());
}

TestScene::~TestScene()
{
}

void TestScene::Init()
{
    qDebug() << 11111;
    NLActor *actor = new NLActor;
    actor->setObjectName("Test_ChildRenderActor");
    GetActor(1)->AddChild(actor);
    NLModelRenderer *r = new NLModelRenderer_cube(200);
    actor->SetRenderable(r);

    qDebug() << 22222;
    NLVector3 v = VECTOR3(-800, 600, -200);
    actor->SetPosition(v);
    NLVector3 vr = VECTOR3(45,0,0);
    actor->SetRotation(vr);

    NLVector3 vr2 = VECTOR3(0,45,0);
    GetActor(1)->SetRotation(vr2);

    SetFPS(SINGLE_INSTANCE_OBJ(Settings)->GetSetting<int>("RENDER/fps", 0));
}

void TestScene::Update(float delta)
{
    NLScene::Update(delta);
}

void TestScene::paintGL()
{
    glClearColor(1,1,1,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    glLineWidth(10);
    NLScene::paintGL();
    glLineWidth(1);

    glFlush();
}

void TestScene::Deinit()
{
    NLScene::Deinit();
    Reset();
}

void TestScene::Reset()
{
}
