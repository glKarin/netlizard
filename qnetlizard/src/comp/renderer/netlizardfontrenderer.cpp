#include "netlizardfontrenderer.h"

#include <QDebug>

#include "gl/nl_gl.h"

#include "engine/nlactor.h"
#include "engine/nlscene.h"
#include "qdef.h"
#include "template/nlvariantpointer.h"

NETLizardFontRenderer::NETLizardFontRenderer(NLActor *actor) :
    NLRenderable(actor),
    m_font(0),
      m_paddingWidth(2),
      m_lineSpacing(1)
{
    CLASS_NAME(NETLizardFontRenderer);
    setObjectName("NETLizardFontRenderer");
}

NETLizardFontRenderer::~NETLizardFontRenderer()
{
    m_font = 0;
}

void NETLizardFontRenderer::SetText(const QString &str)
{
    if(m_text != str)
    {
        m_text = str;
        UpdateLayout();
        emit propertyChanged("text", m_text);
    }
}

void NETLizardFontRenderer::SetPaddingWidth(int i)
{
    if(m_paddingWidth != i)
    {
        m_paddingWidth = i;
        UpdateLayout();
        emit propertyChanged("paddingWidth", m_paddingWidth);
    }
}

void NETLizardFontRenderer::SetLineSpacing(int i)
{
    if(m_lineSpacing != i)
    {
        m_lineSpacing = i;
        emit propertyChanged("lineSpacing", m_lineSpacing);
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
    int width = qMax(scene->width() - 2 * m_paddingWidth, (int)m_font->width);
    QByteArray ba = m_text.toLocal8Bit();

    const char *p = ba.constData();
    size_t len = strlen(p);
    GLint count = 0;
    GLint line_count = 0;
    while(count < (GLint)len)
    {
        GLint c = NETLizard_FontGetCharCountOfWidth(m_font, width, p);
        if(c == -1)
            break;
        if(c == 0) c = 1;
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
    if(!IsActived())
        return;
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

void NETLizardFontRenderer::Destroy()
{
    m_font = 0;
    emit propertyChanged("font", FontPtr());
    NLRenderable::Destroy();
}

void NETLizardFontRenderer::SetFont(GL_NETLizard_Font *f)
{
    if(m_font != f)
    {
        m_font = f;
        emit propertyChanged("font", FontPtr());
    }
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
        {
            glRotatef(180, 1, 0, 0);
            NETLizard_FontRenderString(m_font, 0, 0, 1, 1, 1, 1, str);
        }
        glPopMatrix();
        h += m_lineSpacing;
        glTranslatef(0.0, m_lineSpacing, 0.0);
    }
    //glEnable(GL_CULL_FACE);
}

QVariant NETLizardFontRenderer::FontPtr() const { return QVariant::fromValue<NLVariantGeneralPointer>(NLMAKE_VARIANT_VOID_POINTER(struct _GL_NETLizard_Font, m_font)); }
