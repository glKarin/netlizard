#include "netlizardfontrenderer.h"

#include <QDebug>

#include <GL/gl.h>

#include "nlactor.h"
#include "nlscene.h"
#include "qdef.h"

NETLizardFontRenderer::NETLizardFontRenderer(NLActor *actor) :
    NLRenderable(actor),
    m_font(0),
      m_paddingWidth(2),
      m_lineSpacing(1)
{
}

NETLizardFontRenderer::~NETLizardFontRenderer()
{
    m_font = 0;
    DEBUG_DESTROY("NETLizardFontRenderer")
}

void NETLizardFontRenderer::InitRender()
{

}

void NETLizardFontRenderer::SetText(const QString &str)
{
    if(m_text != str)
    {
        m_text = str;
        UpdateLayout();
    }
}

void NETLizardFontRenderer::UpdateLayout()
{
    m_list.clear();
    if(!m_font)
        return;
    NLActor *actor = Actor();
    if(!actor)
        return;
    NLScene *scene = actor->Scene();
    if(!scene)
        return;
    int width = scene->width();
    QByteArray ba = m_text.toLocal8Bit();

    const char *p = ba.constData();
    size_t len = strlen(p);
    GLint count = 0;
    GLint line_count = 0;
    while(count < (GLint)len)
    {
        GLint c = Font_GetCharCountOfWidth(m_font, width - 2 * m_paddingWidth, p);
        if(c == -1)
            break;
        GLint i;
        for(i = 0; i < c; i++)
        {
            if(p[i] == '\n')
            {
                i++;
                break;
            }
        }
        count += i;
        char *str = (char *)calloc(i + 1, sizeof(char));
        strncpy(str, p, i);
        str[i] = '\0';
        m_list.push_back(str);
        free(str);
        p += i;
        line_count++;
    }
    line_count++;
}

void NETLizardFontRenderer::Render()
{
    if(!m_font)
        return;
    if(m_text.isEmpty())
        return;
    glPushMatrix();
    {
        RenderText();
    }
    glPopMatrix();
}

void NETLizardFontRenderer::DeinitRender()
{
    m_font = 0;
}

font_s * NETLizardFontRenderer::Font()
{
    return m_font;
}

void NETLizardFontRenderer::SetFont(font_s *f)
{
    m_font = f;
}

void NETLizardFontRenderer::RenderText()
{
    GLfloat h = 0.0;
    //glDisable(GL_CULL_FACE);
    glTranslatef(m_paddingWidth, m_paddingWidth, 0.01);
    int char_height = m_font->height;
    Q_FOREACH(const QString &text, m_list)
    {
        QByteArray ba = text.toLocal8Bit();
        const char *str = ba.constData();
        h += char_height;
        glTranslatef(0.0, char_height, 0.0);
        glPushMatrix();
        glRotatef(180, 1, 0, 0);
        Font_RenderString(m_font, 0, 0, 1, 1, 1, 1, str);
        glPopMatrix();
        h += m_lineSpacing;
        glTranslatef(0.0, m_lineSpacing, 0.0);
    }
    //glEnable(GL_CULL_FACE);
}

