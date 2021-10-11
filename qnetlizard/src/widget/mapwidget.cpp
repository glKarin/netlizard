#include "mapwidget.h"

#include <QDebug>
#include <QByteArray>

#include "qdef.h"
#include "nlactor.h"
#include "simplecameraactor.h"
#include "netlizardmapmodelrenderer.h"

MapWidget::MapWidget(QWidget *parent)
    : NLScene(parent),
      m_model(0),
      m_renderer(0)
{
    setObjectName("MapWidget");
    SimpleCameraActor *camera = new SimpleCameraActor;
    m_actors.Add(camera);
    NLActor *actor = new NLActor;
    m_actors.Add(actor);
    m_renderer = new NETLizardMapModelRenderer;
    actor->SetRenderable(m_renderer);
    SetCurrentCamera(camera->Camera());
}

MapWidget::~MapWidget()
{
    DEBUG_DESTROY_Q;
}

void MapWidget::Init()
{
    NLScene::Init();
}

void MapWidget::Update(float delta)
{
    //qDebug() << delta;
    NLScene::Update(delta);
}

void MapWidget::paintGL()
{
    if(!m_model)
        return;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    NLScene::paintGL();

    glFlush();
}

void MapWidget::Deinit()
{
    NLScene::Deinit();
    Reset();
}

bool MapWidget::LoadFile(const QString &file, const QString &resourcePath, int game, int level)
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
        b = NETLizard_ReadGLCT3DMapModelFile(path, level, resc_path, m_model);
        break;
    case NL_CONTR_TERRORISM_3D_EPISODE_2:
        b = NETLizard_ReadGLCT3DEp2MapModelFile(path, level, resc_path, m_model);
        break;
    case NL_ARMY_RANGER_3D:
        b = NETLizard_ReadGLSpecnaz3DMapModelFile(path, level, resc_path, m_model);
        break;
    case NL_SHADOW_OF_EGYPT_3D:
        b = NETLizard_ReadGLEgypt3DMapModelFile(path, resc_path, m_model);
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

    //vector3_s startPos = {{m_model->start_pos[0], m_model->start_pos[2], -m_model->start_pos[1]}};
    //vector3_s startRotate = {{m_model->start_angle[0] + 90.0, m_model->start_angle[1] - 180.0, 0}};
    vector3_s startPos = {{m_model->start_pos[0], m_model->start_pos[1], m_model->start_pos[2]}}; // z_is_up
    vector3_s startRotate = {{m_model->start_angle[0] - 90.0, m_model->start_angle[1] - 180.0, 0}}; // z_is_up

    SimpleCameraActor *camera = static_cast<SimpleCameraActor *>(m_actors[0]);
    camera->SetPosition(startPos);
    camera->SetRotation(startRotate);

    GrabMouseCursor(true);

    return true;
}

void MapWidget::Reset()
{
    if(m_model)
    {
        delete_GL_NETLizard_3D_Model(m_model);
        free(m_model);
        m_model = 0;
    }
    m_renderer->SetModel(0);
    //SetHideMouse(false);
}


bool MapWidget::IsValid() const
{
    return m_model != 0;
}

const GL_NETLizard_3D_Model * MapWidget::Model() const
{
    return m_model;
}
