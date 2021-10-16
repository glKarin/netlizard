#include "fontscene.h"

#include <QDebug>
#include <QByteArray>

#include <QColor>

#include "qdef.h"
#include "nlscenecamera.h"
#include "simplecameraactor.h"
#include "nlsceneorthocamera.h"
#include "netlizardfontrenderer.h"

FontScene::FontScene(QWidget *parent) :
    NLScene(parent),
    m_renderer(0),
    m_font(0),
    m_lineCount(0),
    m_paddingWidth(2),
    m_lineSpacing(1)
{
    setObjectName("FontScene");

    NLPropperties prop;
    prop.insert("type", QVariant::fromValue((int)NLSceneCamera::Type_Ortho));
    SimpleCameraActor *camera = new SimpleCameraActor(prop);
    Qt::Alignment align = Qt::AlignLeft | Qt::AlignTop;
    AddActor(camera);
    NLActor *actor = new NLActor;
    AddActor(actor);
    m_renderer = new NETLizardFontRenderer;
    actor->SetRenderable(m_renderer);
    NLSceneOrthoCamera *orthoCam = static_cast<NLSceneOrthoCamera *>(camera->Camera());
    orthoCam->SetAlignment(align);
    SetCurrentCamera(orthoCam);
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
        delete_font(m_font);
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
    m_font = (font_s *)malloc(sizeof(font_s));
    memset(m_font, 0, sizeof(font_s));
    qDebug() << "Load font: " << path << tex_path;
    GLboolean b = new_netlizard_font(m_font, path, tex_path);
    qDebug() << b;
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
