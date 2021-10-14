#include "itemscene.h"

#include <QDebug>
#include <QByteArray>

#include "qdef.h"
#include "simplecameraactor.h"
#include "netlizardmapmodelrenderer.h"

ItemScene::ItemScene(QWidget *parent)
    : NLScene(parent),
      m_model(0),
      m_renderer(0)
{
    setObjectName("ItemScene");

    NLPropperties prop;
    prop.insert("z_is_up", true);
    SimpleCameraActor *camera = new SimpleCameraActor(prop);
    m_actors.Add(camera);
    NLActor *actor = new NLActor;
    m_actors.Add(actor);
    m_renderer = new NETLizardMapModelRenderer;
    actor->SetRenderable(m_renderer);
    SetCurrentCamera(camera->Camera());
}

ItemScene::~ItemScene()
{
    DEBUG_DESTROY_Q;
}

void ItemScene::Init()
{
    NLScene::Init();
}

void ItemScene::Update(float delta)
{
    //qDebug() << delta;
    NLScene::Update(delta);
}

void ItemScene::paintGL()
{
    if(!m_model)
        return;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    NLScene::paintGL();

    glFlush();
}

void ItemScene::Deinit()
{
    NLScene::Deinit();
    Reset();
}

bool ItemScene::LoadFile(const QString &file, const QString &resourcePath, int game, int index)
{
    Reset();
    QByteArray ba1 = file.toLocal8Bit();
    const char *path = ba1.data();
    QByteArray ba2 = resourcePath.toLocal8Bit();
    const char *resc_path = ba2.data();
    m_model = (GL_NETLizard_3D_Model *)malloc(sizeof(GL_NETLizard_3D_Model));
    memset(m_model, 0, sizeof(GL_NETLizard_3D_Model));
    GLboolean b = GL_FALSE;
    qDebug() << "Load game: " << path << resc_path;
    switch(game)
    {
    case NL_CONTR_TERRORISM_3D:
        b = NETLizard_ReadGLCT3DItemModelFile(path, index, resc_path, m_model);
        break;
    case NL_CONTR_TERRORISM_3D_EPISODE_2:
        b = NETLizard_ReadGLCT3DEp2ItemModelFile(path, index, resc_path, m_model);
        break;
    case NL_ARMY_RANGER_3D:
        b = NETLizard_ReadGLSpecnaz3DItemModelFile(path, index, resc_path, m_model);
        break;
    case NL_SHADOW_OF_EGYPT_3D:
        b = NETLizard_ReadGLEgypt3DItemModelFile(path, index, resc_path, m_model);
        break;
    default:
        qDebug() << "Unsupport game";
        break;
    }
    qDebug() << b;
    if(!b)
    {
        free(m_model);
        m_model = 0;
        return false;
    }

    m_renderer->SetModel(m_model);

    GrabMouseCursor(true);

    return true;
}

void ItemScene::Reset()
{
    if(m_model)
    {
        delete_GL_NETLizard_3D_Model(m_model);
        free(m_model);
        m_model = 0;
    }

    NLScene::Reset();

    vector3_s startPos = VECTOR3(0, 150, 1000);
    vector3_s startRotate = VECTOR3(0, 0, 0);
    SimpleCameraActor *camera = static_cast<SimpleCameraActor *>(m_actors[0]);
    camera->SetPosition(startPos);
    camera->SetRotation(startRotate);
}

bool ItemScene::IsValid() const
{
    return m_model != 0;
}

const GL_NETLizard_3D_Model * ItemScene::Model() const
{
    return m_model;
}
