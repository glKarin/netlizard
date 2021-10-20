#include "rolescene.h"

#include <QDebug>
#include <QByteArray>
#include <QFileInfo>

#include "qdef.h"
#include "simplecameraactor.h"
#include "netlizardanimationmodelrenderer.h"
#include "simplecontrolcomponent.h"
#include "nlsceneorthocamera.h"
#include "settings.h"

RoleScene::RoleScene(QWidget *parent)
    : NLScene(parent),
      m_model(0),
      m_renderer(0),
      m_frameAnim(0),
      m_anim(-1),
      m_frame(-1),
      m_playing(false),
      m_animFPS(0),
      m_frameInterval(0),
      m_lastFrameTime(0)
{
    SetFPS(SINGLE_INSTANCE_OBJ(Settings)->GetSetting<int>("RENDER/fps", 0));
    SetAnimFPS(2);

    setObjectName("RoleScene");

    NLPropperties prop;
    prop.insert("z_is_up", true);
    SimpleCameraActor *camera = new SimpleCameraActor(prop);
    AddActor(camera);
    NLActor *actor = new NLActor;
    AddActor(actor);
    m_renderer = new NETLizardAnimationModelRenderer;
    actor->SetRenderable(m_renderer);
    SetCurrentCamera(camera->Camera());
}

RoleScene::~RoleScene()
{
    DEBUG_DESTROY_Q;
}

void RoleScene::Init()
{
    NLScene::Init();
}

void RoleScene::Update(float delta)
{
    //qDebug() << delta;
    NLScene::Update(delta);
    qint64 t = UpdateTime();
    qint64 d = t - m_lastFrameTime;
    if(m_frameInterval == 0 || d > m_frameInterval)
    {
        m_lastFrameTime = t;
        NextFrame();
    }
}

void RoleScene::paintGL()
{
    if(!m_model)
        return;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    NLScene::paintGL();

    glFlush();
}

void RoleScene::Deinit()
{
    Reset();
    NLScene::Deinit();
}

bool RoleScene::LoadFile(const QString &file, const QString &resourcePath, int game, int index)
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
    case NL_SHADOW_OF_EGYPT_3D:
        b = NETLizard_ReadGLEgypt3DRoleModelFile(path, index, resc_path, m_model);
        break;
    //case NL_CLONE_3D:
       // b = NETLizard_ReadGLCT3DEp3ItemModelFile(path, index, resc_path, m_model);
        //break;
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

    LoadAnimFrame(index);

    m_renderer->SetModel(m_model, m_frameAnim);

    vector3_s startPos = VECTOR3(0, 150, 1000);
    SimpleCameraActor *camera = GetActor_T<SimpleCameraActor>(0);
    camera->SetPosition(startPos);

    GrabMouseCursor(true);

    return true;
}

void RoleScene::Reset()
{
    m_frameAnim = 0;
    m_renderer->SetModel(0, 0);
    m_renderer->SetAnim(-1);
    m_renderer->SetFrame(-1);
    if(m_model)
    {
        delete_GL_NETLizard_3D_Model(m_model);
        free(m_model);
        m_model = 0;
    }

    NLScene::Reset();
}

bool RoleScene::IsValid() const
{
    return m_model != 0;
}

const GL_NETLizard_3D_Model * RoleScene::Model() const
{
    return m_model;
}

void RoleScene::SetAnim(int anim)
{
    if(m_anim != anim)
    {
        m_anim = anim;
        m_renderer->SetAnim(anim);
        emit animChanged(anim);
    }
}

void RoleScene::SetFrame(int frame)
{
    if(m_frame != frame)
    {
        m_frame = frame;
        m_renderer->SetFrame(frame);
        emit frameChanged(m_frame);
    }
}

void RoleScene::LoadAnimFrame(int index)
{
    m_frameAnim = 0;
    if(!m_model)
        return;
    int game = m_model->game;
    m_frameAnim = nlGet3DModelFrameAnimationConfig(static_cast<NETLizard_Game>(game), index);
}

const NETLizard_3D_Frame_Animation * RoleScene::Config() const
{
    return m_frameAnim;
}

const NETLizard_3D_Frame_Animation * RoleScene::CurrentAnimation() const
{
    if(!m_frameAnim || m_anim < 0)
        return 0;
    return m_frameAnim + m_anim;
}

int RoleScene::CurrentAnimationFrames() const
{
    const NETLizard_3D_Frame_Animation *config = CurrentAnimation();
    if(config && config->end_frame != -1 && config->begin_frame != -1)
        return config->count;
    else
        return 0;
}

int RoleScene::Anim() const
{
    return m_anim;
}

int RoleScene::Frame() const
{
    return m_frame;
}

void RoleScene::NextFrame()
{
    int count = CurrentAnimationFrames();
    int frame = m_frame;
    if(m_frame >= count)
        frame = 0;
    else
        frame++;
    SetFrame(frame);
}

void RoleScene::PrevFrame()
{
    int count = CurrentAnimationFrames();
    int frame = m_frame;
    if(m_frame < 0)
        frame = count - 1;
    else
        frame--;
    SetFrame(frame);
}

bool RoleScene::IsPlaying() const
{
    return m_playing;
}

void RoleScene::Play()
{
    if(m_playing)
        return;
    m_playing = true;
    emit playing();
}

void RoleScene::Stop()
{
    if(!m_playing)
        return;
    m_playing = false;
    emit stopped();
}

void RoleScene::Toggle()
{
    if(m_playing)
        Stop();
    else
        Play();
}

int RoleScene::AnimFPS() const
{
    return m_animFPS;
}

void RoleScene::SetAnimFPS(int fps)
{
    if(m_animFPS != fps)
    {
        m_animFPS = fps;
        if(m_animFPS <= 0)
            m_frameInterval = 0;
        else
        {
            m_frameInterval = qRound(1000.0 / (float)m_animFPS);
        }
    }
}
