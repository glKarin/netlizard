#include "spritescene.h"

#include <QDebug>
#include <QByteArray>

#include <QColor>

#include "gl/nl_gl.h"
#include "qdef.h"
#include "engine/nlscenecamera.h"
#include "simplecameraactor.h"
#include "netlizardspriterenderer.h"
#include "simpleimagecontrolcomponent.h"
#include "settings.h"

SpriteScene::SpriteScene(QWidget *parent) :
    NLScene(parent),
    m_renderer(0),
    m_sprite(0),
    m_control(0)
{
    setObjectName("SpriteScene");

    Settings *settings = SINGLE_INSTANCE_OBJ(Settings);
    SetFPS(settings->GetSetting<int>("ENGINE/fps", 0));
    SetUpdateInterval(settings->GetSetting<int>("ENGINE/update_interval", 10));
    SetClearColor(QColor(settings->GetSetting<QString>("RENDER/clear_color", "#000000")));

    SimpleCameraActor *camera = new SimpleCameraActor(NLProperties("type", QVariant::fromValue((int)NLSceneCamera::Type_Ortho))("enable_control", false));
    Qt::Alignment align = Qt::AlignLeft | Qt::AlignTop;
    align = Qt::AlignCenter;
    camera->setObjectName("main_camera_2d");
    AddActor(camera);
    NLActor *actor = new NLActor;
    actor->setObjectName("sprite_renderer");
    AddActor(actor);
    m_renderer = new NETLizardSpriteRenderer;
    actor->SetRenderable(m_renderer);
    m_control = new SimpleImageControlComponent(NLProperties(), actor);
    actor->AddComponent(m_control);
    NLSceneCamera *orthoCam = camera->Camera();
    orthoCam->SetAlignment(align);
    SetCurrentCamera(orthoCam);
    connect(settings, SIGNAL(settingChanged(const QString &, const QVariant &, const QVariant &)), this, SLOT(OnSettingChanged(const QString &, const QVariant &, const QVariant &)));
}

SpriteScene::~SpriteScene()
{
    DEBUG_DESTROY_Q;
}

void SpriteScene::Init()
{
    NLScene::Init();
}

void SpriteScene::Update(float delta)
{
    NLScene::Update(delta);
}

void SpriteScene::Deinit()
{
    Reset();
    NLScene::Deinit();
}

void SpriteScene::Reset()
{
    if(m_sprite)
    {
        delete_GL_NETLizard_Sprite(m_sprite);
        free(m_sprite);
        m_sprite = 0;
    }
    m_index = -1;
    m_renderer->SetSprite(0);
    m_renderer->SetIndex(-1);

    NLScene::Reset();
}

void SpriteScene::paintGL()
{
    if(!m_sprite)
        return;
    if(m_index < 0)
        return;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    NLScene::paintGL();

    glFlush();
}

bool SpriteScene::LoadFile(const QString &file, const QString &texFile)
{
    Reset();
    QByteArray ba1 = file.toLocal8Bit();
    const char *path = ba1.data();
    QByteArray ba2 = texFile.toLocal8Bit();
    const char *tex_path = ba2.data();
    m_sprite = (GL_NETLizard_Sprite *)malloc(sizeof(GL_NETLizard_Sprite));
    memset(m_sprite, 0, sizeof(GL_NETLizard_Sprite));
    qDebug() << "Load sprite: " << path << tex_path;
    GLboolean b = NETLizard_ReadSpirit(m_sprite, path, tex_path);
    qDebug() << "Load sprite result: " << b;
    if(!b)
    {
        free(m_sprite);
        m_sprite = 0;
        return false;
    }

    m_renderer->SetSprite(m_sprite);

    return true;
}

int SpriteScene::Count() const
{
    if(!IsValid())
        return -1;
    return m_sprite->sprite_count;
}

void SpriteScene::SetIndex(int i)
{
    if(m_index != i)
    {
        m_index = i;
        NLScene::Reset();
        m_renderer->SetIndex(i);
    }
}

void SpriteScene::OnSettingChanged(const QString &name, const QVariant &value, const QVariant &oldValue)
{
    if(name == "ENGINE/fps")
        SetFPS(value.toInt());
    else if(name == "ENGINE/update_interval")
        SetUpdateInterval(value.toInt());
    else if(name == "CONTROL_2D/trans_sens")
        m_control->SetTransSens(value.toFloat());
    else if(name == "CONTROL_2D/rot_sens")
        m_control->SetRotSens(value.toFloat());
    else if(name == "RENDER/clear_color")
        SetClearColor(QColor(value.toString()));
}
