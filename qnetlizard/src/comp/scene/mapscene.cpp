#include "mapscene.h"

#include <QDebug>
#include <QByteArray>

#include "qdef.h"
#include "simplecameraactor.h"
#include "netlizardmapmodelrenderer.h"
#include "netlizardshadowmodelrenderer.h"
#include "netlizarditemmodelrenderer.h"
#include "simplecameracomponent.h"
#include "simplecontrolcomponent.h"
#include "simplelightsourceactor.h"
#include "simplelightsourcecomponent.h"
#include "nlsceneorthocamera.h"
#include "nlsceneperspectivecamera.h"
#include "netlizardtexturerenderer.h"
#include "settings.h"
#include "matrix.h"
#include "nl_util.h"

MapScene::MapScene(QWidget *parent)
    : NLScene(parent),
      m_model(0),
      m_mapActor(0),
      m_shadowActor(0),
      m_skyActor(0),
      m_sky3DActor(0),
      m_renderer(0),
      m_shadowRenderer(0),
      m_skyRenderer(0),
      m_sky3DRenderer(0),
      m_skyCamera(0),
      m_sky3DCamera(0),
      m_control(0)
{
    setObjectName("MapScene");
    Settings *settings = SINGLE_INSTANCE_OBJ(Settings);
    SetFPS(settings->GetSetting<int>("RENDER/fps", 0));

    NLPropperties prop;

    // 3D camera + 3D control
    prop.insert("z_is_up", true);
    SimpleCameraActor *camera = new SimpleCameraActor(prop);
    AddActor(camera);
    m_control = static_cast<SimpleControlComponent *>(camera->Control());

    // render model
    m_mapActor = new NLActor;
    AddActor(m_mapActor);
    m_renderer = new NETLizardMapModelRenderer;
    m_mapActor->SetRenderable(m_renderer);
    SetCurrentCamera(camera->Camera());
    m_renderer->SetCull(settings->GetSetting<bool>("RENDER/scene_cull"));

    // 2D background camera
    prop.clear();
    prop.insert("type", NLSceneCamera::Type_Ortho);
    prop.insert("enable_control", false);
    SimpleCameraActor *camera_2d = new SimpleCameraActor(prop);
    AddActor(camera_2d);
    m_skyCamera = static_cast<NLSceneOrthoCamera *>(camera_2d->Camera());
    m_skyCamera->SetAlignment(Qt::AlignCenter);

    // 2D background render
    m_skyActor = new NLActor;
    AddActor(m_skyActor);
    m_skyRenderer = new NETLizardTextureRenderer;
    m_skyActor->SetRenderable(m_skyRenderer);

    // 3D background camera
    prop.clear();
    prop.insert("z_is_up", true);
    prop.insert("enable_control", false);
    SimpleCameraActor *camera_3d = new SimpleCameraActor(prop);
    AddActor(camera_3d);
    m_sky3DCamera = static_cast<NLScenePerspectiveCamera *>(camera_3d->Camera());

    // 3D background render
    m_sky3DActor = new NLActor;
    AddActor(m_sky3DActor);
    m_sky3DRenderer = new NETLizardItemModelRenderer;
    m_sky3DActor->SetRenderable(m_sky3DRenderer);

    // render shadow model
    m_shadowActor = new NLActor;
    AddActor(m_shadowActor);
    m_shadowRenderer = new NETLizardShadowModelRenderer;
    m_shadowActor->SetRenderable(m_shadowRenderer);
    m_shadowRenderer->SetCull(settings->GetSetting<bool>("RENDER/scene_cull"));
    int method = settings->GetSetting<int>("RENDER/shadow");
    m_shadowActor->SetEnabled(method > 0);
    m_shadowRenderer->SetStencilShadowMethod(method);

    // light source
    prop.clear();
    prop.insert("type", static_cast<int>(SimpleLightSourceComponent::LightSourceType_Point));
    SimpleLightSourceActor *lightSource = new SimpleLightSourceActor(prop);
    AddActor(lightSource);

    m_shadowRenderer->SetLightSourceType(lightSource->LightSource()->IsDirectionLighting());

    connect(settings, SIGNAL(settingChanged(const QString &, const QVariant &, const QVariant &)), this, SLOT(OnSettingChanged(const QString &, const QVariant &, const QVariant &)));
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
    NLActor *camera_3d = GetActor(4);
    camera_3d->SetRotation(CurrentCamera()->Rotation());
    m_shadowRenderer->SetLightSourcePosition(GetActor(7)->Position());

    if(m_model)
    {
        int *scenes = m_renderer->Scenes();
        if(scenes)
        {
            float frustum[6][4];
            NLSceneCamera *camera = CurrentCamera();
            const GLmatrix *projMat = camera->ProjectionMatrix();
            const GLmatrix *viewMat = camera->ViewMatrix();
            matrix_cale_frustum(projMat, viewMat, frustum);
            int count = NETLizard_GetMapRenderScenes(m_model, scenes, frustum);
            m_renderer->SetSceneCount(count);
            m_shadowRenderer->SetRenderScenes(scenes, count);
        }
    }
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
        glDisable(GL_DEPTH_TEST);
        glDepthMask(GL_FALSE);
        m_sky3DCamera->Render(m_sky3DActor);
        glEnable(GL_DEPTH_TEST);
        glDepthMask(GL_TRUE);
    }

    CurrentCamera()->Render(m_mapActor);
    CurrentCamera()->Render(m_shadowActor);

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
    case NL_CLONE_3D:
        b = NETLizard_ReadGLClone3DMapModelFile(path, resc_path, m_model);
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
    m_shadowRenderer->SetModel(m_model);
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
//                        GLfloat xs = mesh->box.max[0] - mesh->box.min[0];
//                        GLfloat ys = mesh->box.max[1] - mesh->box.min[1];
//                        GLfloat zs = mesh->box.max[2] - mesh->box.min[2];
//                        float d = m_sky3DCamera->ZDistance();
//                        NLVector3 scale = VECTOR3(d / xs, d / ys, d / zs);
//                        NLDEBUG_VECTOR3(scale);
//                        m_sky3DActor->SetScale(scale);
                        NLVector3 skyPos = VECTOR3(mesh->position[0], mesh->position[2], -mesh->position[1]);
                        NLActor *camera_3d = GetActor(4);
                        camera_3d->SetPosition(skyPos);
                        m_sky3DRenderer->SetModel(mesh, m_model->texes);
                        break; // only set 1, some level has many sky box
                    }
                }
            }
        }
    }

    bound_s bound = BOUND(0, 0, 0, 0, 0, 0);
    NETLizard_GetNETLizard3DMapBound(m_model, 0, 0, &bound);
    NLVector3 lp;
    bound_center(&bound, &lp);
    //VECTOR3_Y(lp) = (BOUND_MAX_Y(bound) - BOUND_MIN_Y(bound)) * 2;
    NLVector3 pos = VECTOR3(VECTOR3_X(lp), VECTOR3_Z(lp), VECTOR3_Y(lp));
    GetActor(7)->SetPosition(pos);

    GrabMouseCursor(true);

    return true;
}

void MapScene::Reset()
{
    m_renderer->SetModel(0);
    m_shadowRenderer->SetModel(0);
    m_skyRenderer->SetTexture(0);
    m_sky3DRenderer->SetModel(0, 0);
    if(m_model)
    {
        delete_GL_NETLizard_3D_Model(m_model);
        free(m_model);
        m_model = 0;
    }

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

void MapScene::OnSettingChanged(const QString &name, const QVariant &value, const QVariant &oldValue)
{
    if(name == "RENDER/fps")
        SetFPS(value.toInt());
    else if(name == "RENDER/scene_cull")
    {
        m_renderer->SetCull(value.toBool());
        m_shadowRenderer->SetCull(m_renderer->Cull());
    }
    else if(name == "CONTROL_3D/move_sens")
        m_control->SetMoveSens(value.toInt());
    else if(name == "CONTROL_3D/turn_sens")
        m_control->SetTurnSens(value.toInt());
    else if(name == "CONTROL_3D/freelook_sens")
        m_control->SetFreelookSens(value.toFloat());
    else if(name == "CONTROL_3D/fovy_sens")
        m_control->SetFovySens(value.toFloat());
    else if(name == "RENDER/shadow")
    {
        int m = value.toInt();
        m_shadowActor->SetEnabled(m > 0);
        m_shadowRenderer->SetStencilShadowMethod(m);
    }
}
