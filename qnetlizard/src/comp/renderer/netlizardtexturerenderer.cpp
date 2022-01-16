#include "netlizardtexturerenderer.h"

#include <QDebug>

#include "GL/gl_texture.h"

#include "qdef.h"

NETLizardTextureRenderer::NETLizardTextureRenderer(NLActor *actor) :
    NLRenderable(actor),
    m_tex(0)
{
    SetName("NETLizardTextureRenderer");
}

NETLizardTextureRenderer::~NETLizardTextureRenderer()
{
    m_tex = 0;
    DEBUG_DESTROY(NETLizardTextureRenderer)
}

void NETLizardTextureRenderer::InitRender()
{

}

void NETLizardTextureRenderer::Render()
{
    if(!m_tex)
        return;
    glPushMatrix();
    {
        GLfloat texcoord[] = {
            0.0, 1.0,
            1.0, 1.0,
            0.0, 0.0,
            1.0, 0.0
        };
        GLfloat w = m_tex->width;
        GLfloat h = m_tex->height;
        GLfloat start_x = 0;
        GLfloat start_y = 0;
        GLfloat end_x = w;
        GLfloat end_y = h;
        if(m_align & Qt::AlignLeft)
        {
        }
        else
        {
            start_x = -w / 2;
            end_x = w / 2;
        }

        if(m_align & Qt::AlignBottom)
        {
        }
        else if(m_align & Qt::AlignTop)
        {
            start_y = h;
            end_y = 0;
        }
        else
        {
            start_y = -h / 2;
            end_y = h / 2;
        }
        GLfloat vertex[8] = {
            start_x, start_y,
            end_x, start_y,
            start_x, end_y,
            end_x, end_y,
        };

        glDisable(GL_DEPTH_TEST);
        glDepthMask(GL_FALSE);
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glVertexPointer(2, GL_FLOAT, 0, vertex);
        glTexCoordPointer(2, GL_FLOAT, 0, texcoord);
        glBindTexture(GL_TEXTURE_2D, m_tex->texid);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindTexture(GL_TEXTURE_2D, 0);
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glDepthMask(GL_TRUE);
        glEnable(GL_DEPTH_TEST);
    }
    glPopMatrix();
}

void NETLizardTextureRenderer::DeinitRender()
{
    m_tex = 0;
}

void NETLizardTextureRenderer::SetTexture(texture_s *tex)
{
    if(m_tex != tex)
        m_tex = tex;
}

void NETLizardTextureRenderer::SetAlignment(Qt::Alignment align)
{
    Qt::Alignment a = align;
    a = a & (~Qt::AlignRight);
    if(m_align != a)
    {
        m_align = (Qt::Alignment)a;
    }
}
