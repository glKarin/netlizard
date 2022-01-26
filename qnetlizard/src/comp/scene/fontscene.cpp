#include "fontscene.h"

#include <QDebug>
#include <QByteArray>

#include <QColor>

#include "gl/nl_gl.h"
#include "qdef.h"
#include "engine/nlscenecamera.h"
#include "simplecameraactor.h"
#include "netlizardfontrenderer.h"
#include "settings.h"

FontScene::FontScene(QWidget *parent) :
    NLScene(parent),
    m_renderer(0),
    m_font(0),
    m_lineCount(0),
    m_paddingWidth(2),
    m_lineSpacing(1)
{
    setObjectName("FontScene");
    Settings *settings = SINGLE_INSTANCE_OBJ(Settings);
    SetFPS(settings->GetSetting<int>("ENGINE/fps", 0));
    SetUpdateInterval(settings->GetSetting<int>("ENGINE/update_interval", 10));
    SetClearColor(QColor(settings->GetSetting<QString>("RENDER/clear_color", "#000000")));

    SimpleCameraActor *camera = new SimpleCameraActor(NLProperties("type", QVariant::fromValue((int)NLSceneCamera::Type_Ortho))("enable_control", false));
    camera->setObjectName("main_camera_2d");
    AddActor(camera);
    NLActor *actor = new NLActor;
    actor->setObjectName("font_renderer");
    AddActor(actor);
    m_renderer = new NETLizardFontRenderer;
    actor->SetRenderable(m_renderer);
    NLSceneCamera *orthoCam = camera->Camera();
    orthoCam->SetAlignment(Qt::AlignLeft | Qt::AlignTop);
    SetCurrentCamera(orthoCam);
    connect(settings, SIGNAL(settingChanged(const QString &, const QVariant &, const QVariant &)), this, SLOT(OnSettingChanged(const QString &, const QVariant &, const QVariant &)));
}

FontScene::~FontScene()
{
    DEBUG_DESTROY_Q;
}

void FontScene::Init()
{
    NLScene::Init();
}

void FontScene::Update(float delta)
{
    NLScene::Update(delta);
}

void FontScene::Deinit()
{
    Reset();
    NLScene::Deinit();
}

void FontScene::Reset()
{
    if(m_font)
    {
        delete_GL_NETLizard_Font(m_font);
        free(m_font);
        m_font = 0;
        emit propertyChanged("font", FontPtr());
    }
    m_text.clear();
    emit propertyChanged("text", m_text);
    m_lineCount = 0;
    m_renderer->SetFont(0);

    NLScene::Reset();

    CurrentCamera()->SetAlignment(Qt::AlignLeft | Qt::AlignTop);
}

void FontScene::paintGL()
{
    if(!m_font)
        return;
//    if(m_text.isEmpty())
//        return;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    NLScene::paintGL();

    glFlush();
}

bool FontScene::LoadFile(const QString &file, const QString &texFile)
{
    Reset();
    QByteArray ba1 = file.toLocal8Bit();
    const char *path = ba1.data();
    QByteArray ba2 = texFile.toLocal8Bit();
    const char *tex_path = ba2.data();
    GL_NETLizard_Font font;
    qDebug() << "Load font: " << path << tex_path;
    GLboolean b = NETLizard_ReadFont(&font, path, tex_path);
    qDebug() << "Load font result: " << b;
    if(!b)
    {
        return false;
    }
    m_font = (GL_NETLizard_Font *)malloc(sizeof(GL_NETLizard_Font));
    *m_font = font;
    m_renderer->SetFont(m_font);
    emit propertyChanged("font", FontPtr());

    return true;
}

void FontScene::SetText(const QString &str)
{
    if(m_text != str)
    {
        m_text = str;
        m_renderer->SetText(str);
        emit propertyChanged("text", m_text);
    }
}

void FontScene::resizeEvent(QResizeEvent *event)
{
    NLScene::resizeEvent(event);
    m_renderer->UpdateLayout();
}

void FontScene::OnSettingChanged(const QString &name, const QVariant &value, const QVariant &oldValue)
{
    if(name == "ENGINE/fps")
        SetFPS(value.toInt());
    else if(name == "ENGINE/update_interval")
        SetUpdateInterval(value.toInt());
    else if(name == "RENDER/clear_color")
        SetClearColor(QColor(value.toString()));
}
