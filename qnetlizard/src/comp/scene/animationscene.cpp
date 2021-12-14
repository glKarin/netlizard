#include "animationscene.h"

#include <QDebug>
#include <QByteArray>
#include <QFileInfo>

#include "qdef.h"
#include "simplecameraactor.h"
#include "netlizardanimationmodelrenderer.h"
#include "simplecontrolcomponent.h"
#include "nlsceneorthocamera.h"
#include "settings.h"

AnimationScene::AnimationScene(QWidget *parent)
    : NLScene(parent),
      m_model(0),
      m_renderer(0),
      m_frameAnim(0),
      m_anim(-1),
      m_frame(-1),
      m_playing(false),
      m_animFPS(0),
      m_playSequence(false),
      m_frameInterval(0),
      m_lastFrameTime(0),
      m_control(0)
{
    setObjectName("AnimationScene");
    Settings *settings = SINGLE_INSTANCE_OBJ(Settings);
    SetFPS(settings->GetSetting<int>("ENGINE/fps", 0));
    SetUpdateInterval(settings->GetSetting<int>("ENGINE/update_interval", 10));
    SetClearColor(QColor(settings->GetSetting<QString>("RENDER/clear_color", "#000000")));
    SetAnimFPS(10);

    SimpleCameraActor *camera = new SimpleCameraActor(NLProperties("camera_z_is_up", true));
    camera->setObjectName("main_camera");
    AddActor(camera);
    m_control = static_cast<SimpleControlComponent *>(camera->Control());
    NLActor *actor = new NLActor;
    actor->setObjectName("animation_model_renderer");
    AddActor(actor);
    m_renderer = new NETLizardAnimationModelRenderer;
    actor->SetRenderable(m_renderer);
    SetCurrentCamera(camera->Camera());

    connect(settings, SIGNAL(settingChanged(const QString &, const QVariant &, const QVariant &)), this, SLOT(OnSettingChanged(const QString &, const QVariant &, const QVariant &)));
}

AnimationScene::~AnimationScene()
{
    DEBUG_DESTROY_Q;
}

void AnimationScene::Init()
{
    NLScene::Init();
}

void AnimationScene::Update(float delta)
{
    //qDebug() << delta;
    NLScene::Update(delta);
    if(m_playing)
    {
        qint64 t = UpdateTime();
        qint64 d = t - m_lastFrameTime;
        if(m_frameInterval == 0 || d > m_frameInterval)
        {
            m_lastFrameTime = t;
            if(m_playSequence)
                PrevFrame();
            else
                NextFrame();
        }
    }
}

void AnimationScene::paintGL()
{
    if(!m_model)
        return;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    NLScene::paintGL();

    glFlush();
}

void AnimationScene::Deinit()
{
    Reset();
    NLScene::Deinit();
}

bool AnimationScene::LoadFile(const QString &file, const QString &resourcePath, int game, int index)
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
    case NL_CLONE_3D:
        b = NETLizard_ReadGLClone3DRoleModelFile(path, index, resc_path, m_model);
        break;
    default:
        qDebug() << "Unsupport game";
        break;
    }
    qDebug() << "Load animation model result: " << b;
    if(!b)
    {
        free(m_model);
        m_model = 0;
        return false;
    }

    LoadAnimFrame(index);

    m_renderer->SetModel(m_model, m_frameAnim);

    vector3_t startPos = VECTOR3(0, 150, 1000);
    SimpleCameraActor *camera = GetActor_T<SimpleCameraActor>(0);
    camera->SetPosition(startPos);

    SetAnim(0);

    //GrabMouseCursor(true);

    return true;
}

void AnimationScene::Reset()
{
    Stop();
    m_frameAnim = 0;
    m_renderer->SetModel(0, 0);
    /*m_renderer->*/SetAnim(-1);
    /*m_renderer->*/SetFrame(-1);
    if(m_model)
    {
        delete_GL_NETLizard_3D_Model(m_model);
        free(m_model);
        m_model = 0;
    }

    NLScene::Reset();
}

bool AnimationScene::IsValid() const
{
    return m_model && m_frameAnim;
}

void AnimationScene::SetAnim(int anim)
{
    if(anim >= 0 && !m_frameAnim)
        return;
    if(m_anim != anim)
    {
        m_anim = anim;
        SetFrame(m_anim < 0 ? -1 : 0);
        m_renderer->SetAnim(anim);
        emit animChanged(anim);
    }
}

void AnimationScene::SetFrame(int frame)
{
    if(frame >= 0 && !m_frameAnim)
        return;
    if(m_frame != frame)
    {
        m_frame = frame;
        m_renderer->SetFrame(frame);
        emit frameChanged(m_frame);
    }
}

void AnimationScene::LoadAnimFrame(int index)
{
    m_frameAnim = 0;
    if(!m_model)
        return;
    int game = m_model->game;
    m_frameAnim = nlGet3DModelFrameAnimationConfig(static_cast<NETLizard_Game>(game), index);
}

const NETLizard_3D_Frame_Animation * AnimationScene::CurrentAnimation() const
{
    if(!m_frameAnim)
        return 0;
    if(!m_frameAnim || m_anim < 0)
        return 0;
    return m_frameAnim + m_anim;
}

int AnimationScene::CurrentAnimationFrames() const
{
    const NETLizard_3D_Frame_Animation *config = CurrentAnimation();
    if(config && config->end_frame != -1 && config->begin_frame != -1)
        return config->count;
    else
        return 0;
}

void AnimationScene::NextFrame()
{
    if(!IsValid())
        return;
    int count = CurrentAnimationFrames();
    if(count == 0)
        return;
    int frame = m_frame;
    if(m_frame >= count - 1)
        frame = 0;
    else
        frame++;
    SetFrame(frame);
}

void AnimationScene::PrevFrame()
{
    if(!IsValid())
        return;
    int count = CurrentAnimationFrames();
    if(count == 0)
        return;
    int frame = m_frame;
    if(m_frame <= 0)
        frame = count - 1;
    else
        frame--;
    SetFrame(frame);
}

void AnimationScene::Play()
{
    if(!IsValid())
        return;
    if(m_playing)
        return;
    m_playing = true;
    m_lastFrameTime = UpdateTime();
    emit playing();
}

void AnimationScene::Stop()
{
    if(!IsValid())
        return;
    if(!m_playing)
        return;
    m_playing = false;
    emit stopped();
}

void AnimationScene::Toggle()
{
    if(!IsValid())
        return;
    if(m_playing)
        Stop();
    else
        Play();
}

void AnimationScene::SetPlaying(bool playing)
{
    if(!IsValid())
        return;
    if(m_playing != playing)
    {
        Toggle();
    }
}

void AnimationScene::SetAnimFPS(int fps)
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

void AnimationScene::SetPlaySequence(bool invert)
{
    if(m_playSequence != invert)
        m_playSequence = invert;
}

void AnimationScene::OnSettingChanged(const QString &name, const QVariant &value, const QVariant &oldValue)
{
    if(name == "ENGINE/fps")
        SetFPS(value.toInt());
    else if(name == "ENGINE/update_interval")
        SetUpdateInterval(value.toInt());
    else if(name == "CONTROL_3D/move_sens")
        m_control->SetMoveSens(value.toInt());
    else if(name == "CONTROL_3D/turn_sens")
        m_control->SetTurnSens(value.toInt());
    else if(name == "CONTROL_3D/freelook_sens")
        m_control->SetFreelookSens(value.toFloat());
    else if(name == "CONTROL_3D/fovy_sens")
        m_control->SetFovySens(value.toFloat());
    else if(name == "RENDER/clear_color")
        SetClearColor(QColor(value.toString()));
}
