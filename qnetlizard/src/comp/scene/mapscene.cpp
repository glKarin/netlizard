#include "mapscene.h"

#include <QDebug>
#include <QByteArray>

#include "qdef.h"
#include "simplecameraactor.h"
#include "netlizardmapmodelrenderer.h"
#include "netlizarditemmodelrenderer.h"
#include "simplecameracomponent.h"
#include "simplecontrolcomponent.h"
#include "nlsceneorthocamera.h"
#include "nlsceneperspectivecamera.h"
#include "netlizardtexturerenderer.h"

MapScene::MapScene(QWidget *parent)
    : NLScene(parent),
      m_model(0),
      m_mapActor(0),
      m_skyActor(0),
      m_sky3DActor(0),
      m_renderer(0),
      m_skyRenderer(0),
      m_sky3DRenderer(0),
      m_skyCamera(0),
      m_sky3DCamera(0)
{
    setObjectName("MapScene");
    NLActor *actor;

    NLPropperties prop;
    // 3D camera + 3D control
    prop.insert("z_is_up", true);
    SimpleCameraActor *camera = new SimpleCameraActor(prop);
    AddActor(camera);

    // render model
    m_mapActor = new NLActor;
    AddActor(m_mapActor);
    m_renderer = new NETLizardMapModelRenderer;
    m_mapActor->SetRenderable(m_renderer);
    SetCurrentCamera(camera->Camera());

    // 2D background camera
    actor = new NLActor;
    AddActor(actor);
    prop.clear();
    prop.insert("type", NLSceneCamera::Type_Ortho);
    SimpleCameraComponent *camera_2d = new SimpleCameraComponent(prop, actor);
    actor->AddComponent(camera_2d);
    m_skyCamera = static_cast<NLSceneOrthoCamera *>(camera_2d->Camera());
    m_skyCamera->SetAlignment(Qt::AlignCenter);

    // 2D background render
    m_skyActor = new NLActor;
    AddActor(m_skyActor);
    m_skyRenderer = new NETLizardTextureRenderer;
    m_skyActor->SetRenderable(m_skyRenderer);

    // 3D background camera
    actor = new NLActor;
    AddActor(actor);
    prop.clear();
    prop.insert("z_is_up", true);
    SimpleCameraComponent *camera_3d = new SimpleCameraComponent(prop, actor);
    actor->AddComponent(camera_3d);
    m_sky3DCamera = static_cast<NLScenePerspectiveCamera *>(camera_3d->Camera());

    // 3D background render
    m_sky3DActor = new NLActor;
    AddActor(m_sky3DActor);
    m_sky3DRenderer = new NETLizardItemModelRenderer;
    m_sky3DActor->SetRenderable(m_sky3DRenderer);
}

MapScene::~MapScene()
{
}

void MapScene::Init()
{
    NLScene::Init();
}

void MapScene::Update(float delta)
{
    NLScene::Update(delta);
}

void MapScene::paintGL()
{
    if(!m_model)
        return;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    if(m_skyRenderer->Texture())
    {
        float factorX = (float)width() / (float)m_model->bg_tex->width;
        float factorY = (float)height() / (float)m_model->bg_tex->height;
        float factor = qMax(factorX, factorY);
        NLVector3 scale = VECTOR3(
                    factor,
                    factor,
                    1
                    );
        m_skyActor->SetScale(scale);
        m_skyCamera->Render(m_skyActor);
    }
    else if(m_sky3DRenderer->Model())
    {
        m_sky3DCamera->SetRotation(CurrentCamera()->Rotation());
        m_sky3DCamera->Render(m_sky3DActor);
    }

    CurrentCamera()->Render(m_mapActor);

    glFlush();
}

void MapScene::Deinit()
{
    Reset();
    NLScene::Deinit();
}

bool MapScene::LoadFile(const QString &file, const QString &resourcePath, int game, int level)
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
    case NL_CONTR_TERRORISM_3D_EPISODE_3:
        b = NETLizard_ReadGLCT3DEp3MapModelFile(path, level, resc_path, m_model);
        break;
    case NL_RACING_EVOLUTION_3D:
        b = NETLizard_ReadGLRE3DMapModelFile(path, resc_path, m_model);
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
    if(m_model->bg_tex && m_model->bg_tex)
        m_skyRenderer->SetTexture(m_model->bg_tex);

    // RE3D using java MSG 3D, like OpenGL, y is up
    if(game == NL_RACING_EVOLUTION_3D)
    {
        CurrentCamera()->SetZIsUp(false);
        m_sky3DCamera->SetZIsUp(false);
        SimpleCameraActor *cameraActor = GetActor_T<SimpleCameraActor>(0);
        SimpleControlComponent *control = cameraActor->GetComponent_T<SimpleControlComponent>(1);
        control->SetMoveSens(control->MoveSens() / 20);
    }
    else
    {
        CurrentCamera()->SetZIsUp(true);
        m_sky3DCamera->SetZIsUp(true);
        NLVector3 startPos = VECTOR3(m_model->start_pos[0], m_model->start_pos[2], -m_model->start_pos[1]);
        NLVector3 startRotate = VECTOR3(m_model->start_angle[0] + 90.0, m_model->start_angle[1] - 180.0, 0);
        // NLVector3 startPos = VECTOR3(m_model->start_pos[0], m_model->start_pos[1], m_model->start_pos[2]); // z_is_up
        // NLVector3 startRotate = VECTOR3(m_model->start_angle[0] - 90.0, m_model->start_angle[1] - 180.0, 0); // z_is_up

        SimpleCameraActor *camera = GetActor_T<SimpleCameraActor>(0);
        camera->SetPosition(startPos);
        camera->SetRotation(startRotate);
        // Egypt 3D level 0(main menu) 8 9 10 12 has a cube sky model
        if(game == NL_SHADOW_OF_EGYPT_3D)
        {
            if(level == 0 || level == 8 || level == 9 || level == 10 || level == 12)
            {
                for(int i = 0; i < m_model->item_count; i++)
                {
                    GL_NETLizard_3D_Item_Mesh *mesh = m_model->item_meshes + i;
                    if(mesh->item_type == Item_Box_Type)
                    {
                        GLfloat xs = mesh->item_mesh.ortho[0] - mesh->item_mesh.ortho[3];
                        GLfloat ys = mesh->item_mesh.ortho[1] - mesh->item_mesh.ortho[4];
                        GLfloat zs = mesh->item_mesh.ortho[2] - mesh->item_mesh.ortho[5];
                        float d = m_sky3DCamera->ZDistance();
                        NLVector3 scale = VECTOR3(d / xs, d / ys, d / zs);
                        NLDEBUG_VECTOR3(scale);
                        m_sky3DActor->SetScale(scale);
                        m_sky3DRenderer->SetModel(&mesh->item_mesh, m_model->texes);
                        break;
                    }
                }
            }
            CurrentCamera()->SetZIsUp(true);
            m_sky3DCamera->SetZIsUp(true);
        }
    }

    GrabMouseCursor(true);

    return true;
}

void MapScene::Reset()
{
    if(m_model)
    {
        delete_GL_NETLizard_3D_Model(m_model);
        free(m_model);
        m_model = 0;
    }
    m_renderer->SetModel(0);
    m_skyRenderer->SetTexture(0);
    m_sky3DRenderer->SetModel(0, 0);

    NLScene::Reset();
}


bool MapScene::IsValid() const
{
    return m_model != 0;
}

const GL_NETLizard_3D_Model * MapScene::Model() const
{
    return m_model;
}
