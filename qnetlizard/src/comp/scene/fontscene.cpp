#include "fontscene.h"

#include <QDebug>
#include <QByteArray>

#include <QColor>

#include "qdef.h"
#include "nlscenecamera.h"
#include "simplecameraactor.h"
#include "nlsceneorthocamera.h"
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
    SetFPS(settings->GetSetting<int>("RENDER/fps", 0));

    SimpleCameraActor *camera = new SimpleCameraActor(NLProperties("type", QVariant::fromValue((int)NLSceneCamera::Type_Ortho))("enable_control", false));
    Qt::Alignment align = Qt::AlignLeft | Qt::AlignTop;
    //align = Qt::AlignCenter;
    AddActor(camera);
    NLActor *actor = new NLActor;
    AddActor(actor);
    m_renderer = new NETLizardFontRenderer;
    actor->SetRenderable(m_renderer);
    NLSceneOrthoCamera *orthoCam = static_cast<NLSceneOrthoCamera *>(camera->Camera());
    orthoCam->SetAlignment(align);
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
    }
    m_text.clear();
    m_lineCount = 0;
    m_renderer->SetFont(0);

    NLScene::Reset();
}

void FontScene::paintGL()
{
    if(!m_font)
        return;
    if(m_text.isEmpty())
        return;

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
    m_font = (GL_NETLizard_Font *)malloc(sizeof(GL_NETLizard_Font));
    memset(m_font, 0, sizeof(GL_NETLizard_Font));
    qDebug() << "Load font: " << path << tex_path;
    GLboolean b = NETLizard_ReadFont(m_font, path, tex_path);
    qDebug() << "Load font result: " << b;
    if(!b)
    {
        free(m_font);
        m_font = 0;
        return false;
    }

    m_renderer->SetFont(m_font);

    return true;
}

bool FontScene::IsValid() const
{
    return m_font != 0;
}

void FontScene::SetText(const QString &str)
{
    if(m_text != str)
    {
        m_text = str;
        m_renderer->SetText(str);
    }
}

void FontScene::resizeEvent(QResizeEvent *event)
{
    NLScene::resizeEvent(event);
    m_renderer->UpdateLayout();
}

void FontScene::OnSettingChanged(const QString &name, const QVariant &value, const QVariant &oldValue)
{
    if(name == "RENDER/fps")
        SetFPS(value.toInt());
}
