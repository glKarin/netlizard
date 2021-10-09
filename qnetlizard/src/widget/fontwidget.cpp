#include "fontwidget.h"

#include <QDebug>
#include <QByteArray>

#include <QColor>

#include "qdef.h"

FontWidget::FontWidget(QWidget *parent) :
    NLScene(parent),
    m_font(0),
    m_lineCount(0),
    m_paddingWidth(2),
    m_lineSpacing(1)
{
    setObjectName("FontWidget");
    SetEnableDefaultKeyHandler(false);
    SetEnableDefaultMouseHandler(false);
    SetEnableDefaultWheelHandler(false);
}

FontWidget::~FontWidget()
{
    DEBUG_DESTROY_Q;
}

void FontWidget::Init()
{
    NLScene::Init();
    glDisable(GL_DEPTH_TEST);
}

void FontWidget::Update(float delta)
{
    //qDebug() << delta;
    NLScene::Update(delta);
}

void FontWidget::Deinit()
{
    NLScene::Deinit();
    Reset();
}

void FontWidget::Reset()
{
    if(m_font)
    {
        delete_font(m_font);
        free(m_font);
        m_font = 0;
    }
    m_text.clear();
    m_lineCount = 0;
}

void FontWidget::paintGL()
{
    if(!m_font)
        return;
    if(m_text.isEmpty())
        return;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, width(), 0, height());
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glPushMatrix();
    {
        RenderText();
    }
    glPopMatrix();

    glFlush();
}

bool FontWidget::LoadFile(const QString &file, const QString &texFile)
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

    return true;
}

void FontWidget::SetText(const QString &str)
{
    if(!m_font)
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
            GLint c = Font_GetCharCountOfWidth(m_font, width() - 2 * m_paddingWidth, p);
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

void FontWidget::RenderText()
{
    if(m_text.isEmpty())
        return;

    GLfloat h = 0.0;
    GLfloat p = height();
    glPushMatrix();
    {
        glTranslatef(m_paddingWidth, height() - m_paddingWidth, 0.01);
        QByteArray ba = m_text.toLocal8Bit();
        const char *ptr = ba.constData();
        GLint count = 0;
        GLint i;
        int char_height = m_font->height;
        int width = NLScene::width();
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

bool FontWidget::IsValid() const
{
    return m_font != 0;
}
