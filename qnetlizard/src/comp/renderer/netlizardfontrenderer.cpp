#include "netlizardfontrenderer.h"

#include <QDebug>

#include <GL/gl.h>

#include "nlactor.h"
#include "nlscene.h"
#include "qdef.h"

NETLizardFontRenderer::NETLizardFontRenderer(NLActor *actor) :
    NLRenderable(actor),
    m_font(0),
      m_lineCount(0),
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
    if(!m_font)
        return;
    NLActor *actor = Actor();
    if(!actor)
        return;
    NLScene *scene = actor->Scene();
    if(!scene)
        return;
    if(m_text != str)
    {
        m_text = str;
        QByteArray ba = m_text.toLocal8Bit();

        const char *p = ba.constData();
        size_t len = strlen(p);
        GLint count = 0;
        GLint line_count = 0;
        while(count < (GLint)len)
        {
            GLint c = Font_GetCharCountOfWidth(m_font, scene->width() - 2 * m_paddingWidth, p);
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
            line_count++;
            p += i;
        }
        line_count++;

        m_lineCount = line_count;
    }
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

void NETLizardFontRenderer::SetFont(font_s *tex)
{
    m_font = m_font;
}

void NETLizardFontRenderer::RenderText()
{
    if(m_text.isEmpty())
        return;
    NLActor *actor = Actor();
    if(!actor)
        return;
    NLScene *scene = actor->Scene();
    if(!scene)
        return;

    GLfloat h = 0.0;
    //GLfloat p = height();
    glPushMatrix();
    {
        glTranslatef(m_paddingWidth, scene->height() - m_paddingWidth, 0.01);
        QByteArray ba = m_text.toLocal8Bit();
        const char *ptr = ba.constData();
        GLint count = 0;
        GLint i;
        int char_height = m_font->height;
        int width = scene->width();
        for(i = 0; i < m_lineCount; i++)
        {
            h += char_height;
            GLint c = Font_GetCharCountOfWidth(m_font, width - m_paddingWidth * 2, ptr);
            if(c == -1)
                break;
            GLint j;
            for(j = 0; j < c; j++)
            {
                if(ptr[j] == '\n')
                {
                    j++;
                    break;
                }
            }
            count += j;
            glTranslatef(0.0, -char_height, 0.0);
            char *str = (char *)calloc(j + 1, sizeof(char));
            strncpy(str, ptr, j);
            str[j] = '\0';
            Font_RenderString(m_font, 0, 0, 1, 1, 1, 1, str);
            free(str);
            ptr += j;
            h += m_lineSpacing;
            glTranslatef(0.0, -m_lineSpacing, 0.0);
        }
    }
    glPopMatrix();
}

