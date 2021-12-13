#include "mapscene.h"

#include <QDebug>
#include <QByteArray>

#include "qdef.h"
#include "simplecameraactor.h"
#include "netlizardmapmodelrenderer.h"
#include "netlizardshadowmodelrenderer.h"
#include "netlizardmapmodeldebugrenderer.h"
#include "netlizarditemmodelrenderer.h"
#include "simplecameracomponent.h"
#include "simplecontrolcomponent.h"
#include "simplelightsourceactor.h"
#include "simplelightsourcecomponent.h"
#include "nlsceneorthocamera.h"
#include "nlsceneperspectivecamera.h"
#include "nlscenecamera.h"
#include "netlizardtexturerenderer.h"
#include "mapeventhandlercomponent.h"
#include "settings.h"
#include "matrix.h"
#include "nl_util.h"
#include "nl_shadow_render.h"
#include "nl_algo.h"
#include "nlforce.h"
#include "lib/bound.h"

#define OBJ_RADIUS 50
#define OBJ_HEIGHT 180
#define OBJ_LEG_HEIGHT 50
#define OBJ_HEAD_HEIGHT 20

static const float _g = NL::Physics::EARTH_G * 1000;

MapScene::MapScene(QWidget *parent)
    : NLScene(parent),
      m_model(0),
      m_mainCameraActor(0),
      m_mapActor(0),
      m_shadowActor(0),
      m_debugActor(0),
      m_skyActor(0),
      m_sky3DActor(0),
      m_sky3DCameraActor(0),
      m_renderer(0),
      m_shadowRenderer(0),
      m_debugRenderer(0),
      m_skyRenderer(0),
      m_sky3DRenderer(0),
      m_skyCamera(0),
      m_sky3DCamera(0),
      m_control(0),
      m_eventHandler(0),
      m_noclip(0),
      m_fog(false),
      m_singleScene(0)
{
    setObjectName("MapScene");
    Settings *settings = SINGLE_INSTANCE_OBJ(Settings);
    SetFPS(settings->GetSetting<int>("ENGINE/fps", 0));
    SetUpdateInterval(settings->GetSetting<int>("ENGINE/update_interval", 10));

    // render model
    m_mapActor = new NLActor;
    m_mapActor->setObjectName("map_model_renderer");
    AddActor(m_mapActor);
    m_renderer = new NETLizardMapModelRenderer;
    m_mapActor->SetRenderable(m_renderer);
    m_renderer->SetCull(settings->GetSetting<bool>("RENDER/scene_cull"));
    m_eventHandler = new MapEventHandlerComponent;
    m_eventHandler->setObjectName("map_event_handler");
    m_mapActor->AddComponent(m_eventHandler);

    // 3D camera + 3D control
    m_mainCameraActor = new SimpleCameraActor(NLProperties("camera_z_is_up", true));
    m_mainCameraActor->setObjectName("main_camera");
    AddActor(m_mainCameraActor);
    m_control = static_cast<SimpleControlComponent *>(m_mainCameraActor->Control());
    SetCurrentCamera(m_mainCameraActor->Camera());
    m_eventHandler->SetTeleportActor(m_mainCameraActor);

    // 2D background camera
    SimpleCameraActor *camera_2d = new SimpleCameraActor(NLProperties("type", static_cast<int>(NLSceneCamera::Type_Ortho))("enable_control", false));
    camera_2d->setObjectName("sky_2d_camera");
    AddActor(camera_2d);
    m_skyCamera = static_cast<NLSceneOrthoCamera *>(camera_2d->Camera());
    m_skyCamera->SetAlignment(Qt::AlignCenter);

    // 2D background render
    m_skyActor = new NLActor;
    m_skyActor->setObjectName("sky_texture_renderer");
    AddActor(m_skyActor);
    m_skyRenderer = new NETLizardTextureRenderer;
    m_skyActor->SetRenderable(m_skyRenderer);

    // 3D background camera
    m_sky3DCameraActor = new SimpleCameraActor(NLProperties("camera_z_is_up", true)("enable_control", false));
    m_sky3DCameraActor->setObjectName("skybox_camera");
    AddActor(m_sky3DCameraActor);
    m_sky3DCamera = static_cast<NLScenePerspectiveCamera *>(m_sky3DCameraActor->Camera());

    // 3D background render
    m_sky3DActor = new NLActor;
    m_sky3DActor->setObjectName("skybox_model_renderer");
    AddActor(m_sky3DActor);
    m_sky3DRenderer = new NETLizardItemModelRenderer;
    m_sky3DActor->SetRenderable(m_sky3DRenderer);

    // render shadow model
    m_shadowActor = new NLActor;
    m_shadowActor->setObjectName("stencil_shadow_renderer");
    AddActor(m_shadowActor);
    m_shadowRenderer = new NETLizardShadowModelRenderer;
    m_shadowActor->SetRenderable(m_shadowRenderer);
    //m_shadowRenderer->SetCull(settings->GetSetting<bool>("RENDER/scene_cull"));
    int method = settings->GetSetting<int>("RENDER/shadow", SHADOW_Z_FAIL);
    m_shadowActor->SetEnabled(method > 0);
    m_shadowRenderer->SetStencilShadowMethod(method);
    int shadowObj = settings->GetSetting<int>("RENDER/shadow_object", NETLIZARD_SHADOW_RENDER_ITEM);
    m_shadowRenderer->SetShadowObject(shadowObj);

    // light source
    SimpleLightSourceActor *lightSource = new SimpleLightSourceActor(NLProperties("type", static_cast<int>(SimpleLightSourceComponent::LightSourceType_Point)));
    lightSource->setObjectName("lighting_source");
    AddActor(lightSource);

    // render debug model
    m_debugActor = new NLActor;
    m_debugActor->setObjectName("debug_renderer");
    AddActor(m_debugActor);
    m_debugRenderer = new NETLizardMapModelDebugRenderer;
    m_debugActor->SetRenderable(m_debugRenderer);
    m_debugRenderer->SetCull(settings->GetSetting<bool>("RENDER/scene_cull"));
    m_debugRenderer->SetDebug(settings->GetSetting<int>("DEBUG/render"));
    m_debugRenderer->SetCamera(CurrentCamera());

    m_shadowRenderer->SetLightSourceType(lightSource->LightSource()->IsDirectionLighting());

    SetNoclip(settings->GetSetting<int>("DEBUG/noclip"));
    SetFog(settings->GetSetting<int>("RENDER/fog") > 0);
    SetSingleScene(settings->GetSetting<int>("DEBUG/single_scene"));
    connect(settings, SIGNAL(settingChanged(const QString &, const QVariant &, const QVariant &)), this, SLOT(OnSettingChanged(const QString &, const QVariant &, const QVariant &)));
}

MapScene::~MapScene()
{
}

void MapScene::Init()
{
    NLScene::Init();

    GLfloat color[4];
    glGetFloatv(GL_CURRENT_COLOR, color);
    color[3] = 0.2;
    int fog = SINGLE_INSTANCE_OBJ(Settings)->GetSetting<int>("RENDER/fog");
    {
        glFogf(GL_FOG_START, 2000);
        glFogf(GL_FOG_END, 5000);
    }
    {
        glFogf(GL_FOG_DENSITY, 0.00035);
    }
    if(fog == 2)
        glFogf(GL_FOG_MODE, GL_EXP);
    else
        glFogf(GL_FOG_MODE, GL_LINEAR);
    glFogfv(GL_FOG_COLOR, color);
    //glFogi(GL_FOG_COORD_SRC, GL_FRAGMENT_DEPTH);
    //glHint(GL_FOG_HINT, GL_FASTEST);
}

void MapScene::Update(float delta)
{
    nl_vector3_t oldPos = m_mainCameraActor->Position();

    NLScene::Update(delta);
    if(!m_model)
        return;
    VECTOR3_Y(oldPos) = VECTOR3_Y(m_mainCameraActor->Position()); // if has gravity effect

    m_sky3DCameraActor->SetRotation(CurrentCamera()->Rotation());
    m_sky3DCameraActor->UpdateCamera();
    m_shadowRenderer->SetLightSourcePosition(GetActor(7)->Position());

    int scene = -1;
    int item = -1;
    if(m_noclip != 0 && m_model->game != NL_RACING_EVOLUTION_3D)
    {
        nl_vector3_t pos = m_mainCameraActor->Position();
        unsigned include_item = m_noclip == 2;

        ConvToAlgoVector3(oldPos);
        ConvToAlgoVector3(pos);

        //fprintf(stderr,"Old pos : %f %f %f\n", oldPos.v[0], oldPos.v[1], oldPos.v[2]);fflush(stderr);
        //fprintf(stderr,"Tmp pos : %f %f %f\n", pos.v[0], pos.v[1], pos.v[2]);fflush(stderr);
        collision_object_t obj = {oldPos, OBJ_RADIUS, OBJ_HEIGHT, OBJ_LEG_HEIGHT, OBJ_HEAD_HEIGHT};
        int res = NETLizard_MapCollisionTesting(m_model, &obj, &pos, &scene, include_item, &item);
        //fprintf(stderr,"new_pos : %f %f %f\n", pos.v[0], pos.v[1], pos.v[2]);fflush(stderr);
        vector3_t p;
        bool clear = false;
        if(res == NETLizard_Collision_Testing_Scene_Movement)
        {
            p = pos;
            //clear = true;
        }
        else if(res == NETLizard_Collision_Testing_Scene_Pass)
        {
            p = pos;
        }
        else if(res == NETLizard_Collision_Testing_Scene_Missing_Plane)
        {
            p = pos;
        }
        else
        {
            //fprintf(stderr,"<>  : %d : %d| \n\n", res, item);fflush(stderr);
            p = oldPos;
            NLForce_gravity *gravity = m_mainCameraActor->GetTypeForce<NLForce_gravity>();
            if(gravity && gravity->GetProperty_T("force", 0) != 0) // is jump
                clear = true;
        }
        //fprintf(stderr,"NETLizard_MapCollisionTesting : %d - scene(%d), item(%d): %f %f %f\n", res, scene, item, pos.v[0], pos.v[1], pos.v[2]);fflush(stderr);
        float rglz = 0;
        obj.position = p;
        res = NETLizard_GetScenePointZCoord(m_model, &obj, scene, include_item, &scene, &rglz);
        //fprintf(stderr,"NETLizard_GetScenePointZCoord : %d - scene(%d): %f <> %f\n\n", res, scene, VECTOR3_Z(p), rglz);fflush(stderr);
        if(clear)
            m_mainCameraActor->Collision();

        if(res)
        {
            if(VECTOR3_Z(p) > OBJ_HEIGHT + rglz)
            {
                if(!m_mainCameraActor->HasTypeForce<NLForce_gravity>())
                {
                    // pre cale
                    vector3_t de;
                    vector3_subtractv(&de, &p, &oldPos);
                    VECTOR3_Z(de) = 0;
                    float d0 = vector3_length(&de) / 2.0;
                    //fprintf(stderr,"res222 : %f %f %f\n\n", d0, VECTOR3_Z(p), OBJ_HEIGHT + rglz);fflush(stderr);
                    if(VECTOR3_Z(p) - d0 > OBJ_HEIGHT + rglz)
                        m_mainCameraActor->AddForce(new NLForce_gravity(NLProperties("g", _g), m_mainCameraActor));
                    else
                        VECTOR3_Z(p) = OBJ_HEIGHT + rglz;
                }
            }
            else
            {
                if(m_mainCameraActor->HasTypeForce<NLForce_gravity>())
                    m_mainCameraActor->RemoveTypeForces<NLForce_gravity>();
                VECTOR3_Z(p) = OBJ_HEIGHT + rglz;
            }
        }
        ConvToRenderVector3(p);
        m_mainCameraActor->SetPosition(p);
        m_mainCameraActor->UpdateCamera();

        if(item >= 0)
            m_eventHandler->Collision(item);
    }

    // cull map scenes
    int *scenes = m_renderer->Scenes();
    if(scenes)
    {
        int count;
        if(m_noclip != 0 && m_singleScene)
        {
            if(scene >= 0)
            {
                scenes[0] = scene;
                count = 1;
                if(m_singleScene == 2)
                {
                    count += NETLizard_GetNETLizard3DMapNeighboringScenes(m_model, scene, scenes + 1);
                }
            }
            else
                count = 0;
        }
        else
        {
            float frustum[6][4];
            NLSceneCamera *camera = CurrentCamera();
            const GLmatrix *projMat = camera->ProjectionMatrix();
            const GLmatrix *viewMat = camera->ViewMatrix();
            matrix_cale_frustum(projMat, viewMat, frustum);
            count = NETLizard_GetMapRenderScenes(m_model, scenes, frustum);
        }
        m_renderer->SetSceneCount(count);
        m_shadowRenderer->SetRenderScenes(scenes, count);
        m_debugRenderer->SetRenderScenes(scenes, count);
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
        NLVector3 scale = VECTOR3(factor, factor, 1);
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

    NLSceneCamera *mainCamera = CurrentCamera();
    if(m_fog)
        glEnable(GL_FOG);
    mainCamera->Render(m_mapActor);
    if(m_fog)
        glDisable(GL_FOG);
    mainCamera->Render(m_shadowActor);
    mainCamera->Render(m_debugActor);
    //mainCamera->Render(rrrrr);

    glFlush();
}

void MapScene::Deinit()
{
    Reset();
    glDisable(GL_FOG);
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
    qDebug() << "Load result: " << b;
    if(!b)
    {
        free(m_model);
        m_model = 0;
        return false;
    }

    m_renderer->SetModel(m_model);
    m_shadowRenderer->SetModel(m_model);
    m_debugRenderer->SetModel(m_model);
    m_eventHandler->SetModel(m_model, level);
    bool hasSky = m_model->bg_tex && glIsTexture(m_model->bg_tex->texid);
    if(hasSky)
        m_skyRenderer->SetTexture(m_model->bg_tex);

    bound_t bound = BOUND(0, 0, 0, 0, 0, 0);
    NETLizard_GetNETLizard3DMapBound(m_model, 0, 0, &bound);
    NLVector3 lp;
    bound_center(&bound, &lp);

    glColor4f(1, 1, 1, 1);
    if(hasSky)
    {
        GLfloat color[4] = {1, 1, 1, 0.2};
        glFogfv(GL_FOG_COLOR, color);
    }
    else
    {
        GLfloat color[4] = {0, 0, 0, 0.2};
        glFogfv(GL_FOG_COLOR, color);
    }
    // RE3D using java MSG 3D, like OpenGL, y is up
    if(game == NL_RACING_EVOLUTION_3D)
    {
        CurrentCamera()->SetZIsUp(false);
        m_sky3DCamera->SetZIsUp(false);
        SimpleControlComponent *control = m_mainCameraActor->GetComponent_T<SimpleControlComponent>(1);
        control->SetMoveSens(control->MoveSens() / 20);
        NLVector3 pos = VECTOR3(VECTOR3_X(lp), BOUND_MAX_Y(bound) + (VECTOR3_Y(lp) - BOUND_MIN_Y(bound)) * 2, VECTOR3_Z(lp));
        GetActor(7)->SetPosition(pos);
        //lpos = pos;
#if 0
        NLVector3 scale = VECTOR3(20, 20, 20);
        m_renderer->Actor()->SetScale(scale);
        m_shadowRenderer->Actor()->SetScale(scale);
#endif
    }
    else
    {
#if 0
        NLVector3 scale = VECTOR3(1, 1, 1);
        m_renderer->Actor()->SetScale(scale);
        m_shadowRenderer->Actor()->SetScale(scale);
#endif
        NLVector3 pos = VECTOR3(VECTOR3_X(lp), VECTOR3_Y(lp), BOUND_MAX_Z(bound) + (VECTOR3_Z(lp) - BOUND_MIN_Z(bound)) * 2);
        //lpos = pos;
        GetActor(7)->SetPosition(pos);
        CurrentCamera()->SetZIsUp(true);
        m_sky3DCamera->SetZIsUp(true);
        //NLVector3 startPos = VECTOR3(m_model->start_pos[0], m_model->start_pos[2], -m_model->start_pos[1]);
        NLVector3 startRotate = VECTOR3(m_model->start_angle[0] + 90.0, m_model->start_angle[1] - 180.0, 0);

        NLVector3 startPos = VECTOR3(m_model->start_pos[0], m_model->start_pos[1], m_model->start_pos[2]);
        conv_gl_vector3(&startPos);

        m_mainCameraActor->SetPosition(startPos);
        m_mainCameraActor->SetRotation(startRotate);
        m_mainCameraActor->UpdateCamera();
        // Egypt 3D level 0(main menu) 8 9 10 12 has a cube sky model
        if(game == NL_SHADOW_OF_EGYPT_3D)
        {
            glColor4f(0, 0, 0, 1);
            GLfloat color[4] = {0, 0, 0, 0.2};
            glFogfv(GL_FOG_COLOR, color);
            if(level == 0 || level == 8 || level == 9 || level == 10 || level == 12)
            {
                for(uint i = 0; i < m_model->item_count; i++)
                {
                    GL_NETLizard_3D_Item_Mesh *mesh = m_model->item_meshes + i;
                    if(mesh->item_type & NL_3D_ITEM_TYPE_SKYBOX)
                    {
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

    GrabMouseCursor(true);

    return true;
}
bool MapScene::KeyEventHandler(int key, bool pressed, int modifier)
{
    if(!pressed)
        return false;
    switch(key)
    {
        case Qt::Key_U:
        if(m_noclip != 0)
        {
            bound_t bound = BOUND(0, 0, 0, 0, 0, 0);
            NETLizard_GetNETLizard3DMapBound(m_model, 0, 0, &bound);

            // RE3D using java MSG 3D, like OpenGL, y is up
            NLVector3 pos = m_mainCameraActor->Position();
            if(m_model->game == NL_RACING_EVOLUTION_3D)
            {
                VECTOR3_Y(pos) = BOUND_MAX_Y(bound);
            }
            else
            {
                conv_nl_vector3(&pos);
                VECTOR3_Z(pos) = BOUND_MAX_Z(bound);
                conv_gl_vector3(&pos);
            }
            m_mainCameraActor->SetPosition(pos);
            m_mainCameraActor->UpdateCamera();
            return true;
        }
                //NLVector3 dir = m_mainCameraActor->MoveDirection();
                //m_mainCameraActor->AddForce(new NLForce_push(NLProperties("drag_force", -3000)("force", 2000)("direction_x", VECTOR3_X(dir))("direction_y", VECTOR3_Y(dir))("direction_z", VECTOR3_Z(dir)), m_mainCameraActor));
            break;
        case Qt::Key_Space:
        if(m_noclip != 0)
        {
            if(!m_mainCameraActor->HasTypeForce<NLForce_gravity>())
            {
                m_mainCameraActor->AddForce(new NLForce_gravity(NLProperties("g", _g)("force", -2000), m_mainCameraActor));
            }
            return true;
        }
            break;
    case Qt::Key_P:
        SetNoclip(m_noclip ? 0 : 2);
        return true;
        break;
    case Qt::Key_G:
        SetSingleScene(m_singleScene ? 0 : 1);
        return true;
        break;
    }
    return false;
}

void MapScene::Reset()
{
    m_renderer->SetModel(0);
    m_shadowRenderer->SetModel(0);
    m_debugRenderer->SetModel(0);
    m_skyRenderer->SetTexture(0);
    m_sky3DRenderer->SetModel(0, 0);
    m_eventHandler->SetModel(0, -1);
    if(m_model)
    {
        delete_GL_NETLizard_3D_Model(m_model);
        free(m_model);
        m_model = 0;
    }

    NLScene::Reset();
}

void MapScene::OnSettingChanged(const QString &name, const QVariant &value, const QVariant &oldValue)
{
    if(name == "ENGINE/fps")
        SetFPS(value.toInt());
    else if(name == "ENGINE/update_interval")
        SetUpdateInterval(value.toInt());
    else if(name == "RENDER/scene_cull")
    {
        m_renderer->SetCull(value.toBool());
        //m_shadowRenderer->SetCull(m_renderer->Cull());
        m_debugRenderer->SetCull(m_renderer->Cull());
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
    else if(name == "RENDER/shadow_object")
    {
        int shadowObj = value.toInt();
        m_shadowRenderer->SetShadowObject(shadowObj);
    }
    else if(name == "DEBUG/render")
        m_debugRenderer->SetDebug(value.toInt());
    else if(name == "DEBUG/noclip")
        SetNoclip(value.toInt());
    else if(name == "RENDER/fog")
    {
        int fog = value.toInt();
        if(fog == 2)
            glFogf(GL_FOG_MODE, GL_EXP);
        else
            glFogf(GL_FOG_MODE, GL_LINEAR);
        SetFog(fog > 0);
    }
    else if(name == "DEBUG/single_scene")
        SetSingleScene(value.toInt());
}

void MapScene::ConvToAlgoVector3(vector3_t &v)
{
    const NLSceneCamera *camera = CurrentCamera();
    if(!camera)
        return;

    const NLMatrix4 *mat = camera->RenderMatrix();
//    NLMatrix4 normat;
//    Mesa_InitGLMatrix(&normat, mat);
//    Mesa_NormalMatrix(&normat);
//    matrix_transformv_self_row(&normat, &v); // if has translate and scale
//    Mesa_FreeGLMatrix(&normat);
    matrix_transformv_self_row(mat, &v); // if no translate and no scale
}

void MapScene::ConvToRenderVector3(vector3_t &v)
{
    const NLSceneCamera *camera = CurrentCamera();
    if(!camera)
        return;
    const NLMatrix4 *mat = camera->RenderMatrix(); // no translate and no scale
//    NLMatrix4 normat;
//    Mesa_InitGLMatrix(&normat, mat);
//    Mesa_NormalMatrix(&normat);
//    matrix_transformv_self(&normat, &v); // if has translate and scale
//    Mesa_FreeGLMatrix(&normat);
    matrix_transformv_self(mat, &v); // if no translate and no scale
}

void MapScene::SetNoclip(int b)
{
    if(m_noclip != b)
    {
        m_noclip = b;
        m_mainCameraActor->SetFree(m_noclip == 0);
        if(m_noclip == 0)
            m_mainCameraActor->ClearAllForces();
    }
}

void MapScene::SetFog(bool b)
{
    if(m_fog != b)
    {
        m_fog = b;
    }
}

void MapScene::SetSingleScene(int b)
{
    if(m_singleScene != b)
    {
        m_singleScene = b;
    }
}

int MapScene::RayIntersect()
{
    if(!m_model)
        return -1;
    float distance = 0;
    int scene = -1;
    int collision_id = -1;
    int collision_type = 0;
    NLSceneCamera *camera = CurrentCamera();

    vector3_t direction = camera->Direction();
    vector3_t p = camera->Position();
    const NLMatrix4 *mat = camera->RenderMatrix();
    matrix_transformv_self_row(mat, &p);

    nl_vector3_t cpoint = VECTOR3(0, 0, 0);
    int res = NETLizard_RayIntersect(m_model, &p, &direction, 2, &scene, &collision_id, &collision_type, &cpoint, &distance);
    if(res)
    {
       const GL_NETLizard_3D_Mesh *mesh = m_model->item_meshes + collision_id;
       fprintf(stderr, "item -> %d: obj_index: %d, item_type: %d, count: %d, plane_count: %d\n",
                collision_id, mesh->obj_index, mesh->item_type, mesh->count, mesh->plane_count); fflush(stderr);

        return collision_id;
    }
    return -1;
}

bool MapScene::MouseEventHandler(int mouse, bool pressed, int x, int y, int modifier)
{
    if(!pressed)
        return false;
    if((mouse & Qt::LeftButton) == 0)
        return false;
    int item = RayIntersect();
    if(item < 0)
        return false;
    return m_eventHandler->Trigger(item);
}
