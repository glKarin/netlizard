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
    m_renderer = new NLModelRenderer;
    actor->SetRenderable(m_renderer);
    SetCurrentCamera(camera->Camera());
}

TestScene::~TestScene()
{
}

void TestScene::Init()
{
#define VE4(arr, start, x, y, z, w) \
    arr[start] = x; \
    arr[start + 1] = y; \
    arr[start + 2] = z; \
    arr[start + 3] = w;

#define VE3(arr, start, x, y, z) \
    arr[start] = x; \
    arr[start + 1] = y; \
    arr[start + 2] = z;

    NLScene::Init();
    NLRenderModelGLGeneral *model = m_renderer->Model();

    GLfloat *vertex;
    GLfloat *color;

    vertex = new GLfloat[6 * 4];
    int L = 9999;
    VE4(vertex, 0, -L, 0, 0, 1);
    VE4(vertex, 4, L, 0, 0, 1);
    VE4(vertex, 8, 0, -L, 0, 1);
    VE4(vertex, 12, 0, L, 0, 1);
    VE4(vertex, 16, 0, 0, -L, 1);
    VE4(vertex, 20, 0, 0, L, 1);

    color = new GLfloat[6 * 4];
    VE4(color, 0, 0.2, 0, 0, 0.5);
    VE4(color, 4, 1, 0, 0, 1);
    VE4(color, 8, 0, 0.2, 0, 0.5);
    VE4(color, 12, 0, 1, 0, 1);
    VE4(color, 16, 0, 0, 0.2, 0.5);
    VE4(color, 20, 0, 0, 1, 1);

    for(int i = 0; i < 6; i++)
    {
        model->Vertex().Add(vertex + 4 * i, 0, 0, color + 4 * i);
    }


    NLRenderModelGLGeneral::NLRenderModelPrimitiveData primitive;
    primitive.SetMode(GL_LINES);
    for(int i = 0; i < 6; i++)
    {
        primitive.Index().Add(i);
    }
    model->Primitives().push_back(primitive);

//    GLfloat _vertex[32];
//    L = 200;
//    VE4(_vertex, 0, 0, 0, 0, 1);
//    VE4(_vertex, 4, 0, L, 0, 1);
//    VE4(_vertex, 8, L, 0, 0, 1);
//    VE4(_vertex, 12, L, L, 0, 1);
//    VE4(_vertex, 16, 0, 0, L, 1);
//    VE4(_vertex, 20, 0, L, L, 1);
//    VE4(_vertex, 24, L, 0, L, 1);
//    VE4(_vertex, 28, L, L, L, 1);
//    GLfloat _color[24] = {
//        0, 0 ,0.5, 0.5,
//        0, 0, 1, 1,
//        0.5, 0, 0, 0.5,
//        1, 0, 0, 1,
//        0 ,1, 0, 1,
//        0, 0.5, 1, 0.5
//    };

//    vertex = new GLfloat[4 * 36];
//    color = new GLfloat[4 * 36];

//    static const GLushort CubePlane4Index[] =
//    {
//        // Õý
//        0, 2, 1, 1, 2, 3,
//        // ±³
//        4, 5, 6, 6, 5, 7,
//        // ×ó
//        4, 0, 5, 5, 0, 1,
//        // ÓÒ
//        2, 6, 3, 3, 6, 7,
//        // ÉÏ
//        1, 3, 5, 5, 3, 7,
//        // ÏÂ
//        0, 4, 2, 2, 4, 6
//    };
//    for(int i = 0; i < 36; i += 3)
//    {
//        GLfloat *p, *c;
//        c = _color + (i % 6) * 4;

//        p = _vertex + CubePlane4Index[i] * 4;
//        VE4(vertex, i * 4, p[0], p[1], p[2], p[3]);
//        VE4(color, i * 4, c[0], c[1], c[2], c[3]);

//        p = _vertex + CubePlane4Index[i + 1] * 4;
//        VE4(vertex, (i + 1) * 4, p[0], p[1], p[2], p[3]);
//        VE4(color, (i + 1) * 4, c[0], c[1], c[2], c[3]);

//        p = _vertex + CubePlane4Index[i + 2] * 4;
//        VE4(vertex, (i + 2) * 4, p[0], p[1], p[2], p[3]);
//        VE4(color, (i + 2) * 4, c[0], c[1], c[2], c[3]);
//    }
//    TestRenderer::RenderItem itemc(GL_TRIANGLES, vertex, (GLuint)0, 36, 0, 0, 0, color);

//    TestRenderer *renderer = new TestRenderer;
//    (*renderer)<<(itemc);
    //NLActor *actor = new NLActor;
    //actor->setObjectName("[[[ChildRenderActor");
    //actor->SetRenderable(renderer);
    //NLVector3 v = VECTOR3(-800, 200, -200);
    //NLVector3 vr = VECTOR3(45,0,0);
    //actor->SetPosition(v);
    //actor->SetRotation(vr);
    //m_actors.Get(1)->SetRotation(vr);
    //m_actors.Get(1)->SetPosition(v);
    //GetActor(1)->AddChild(actor);

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
