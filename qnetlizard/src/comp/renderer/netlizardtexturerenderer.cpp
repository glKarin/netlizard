#include "netlizardtexturerenderer.h"

#include <QDebug>

#include "GL/gl_texture.h"

#include "qdef.h"
#include "template/nlvariantpointer.h"

NETLizardTextureRenderer::NETLizardTextureRenderer(NLActor *actor) :
    NLRenderable(actor),
    m_tex(0)
{
    CLASS_NAME(NETLizardTextureRenderer);
    setObjectName("NETLizardTextureRenderer");

    NLProperties props = PropertyConfig();

    props.Insert("alignment",  NLProperties("enum", QVariant::fromValue<NLPropertyPairList>(NLPropertyPairList()
                                                                                            << NLPropertyPair(tr("Center"), static_cast<int>(Qt::AlignCenter))
                                                                                            << NLPropertyPair(tr("Left-Top"), static_cast<int>(Qt::AlignLeft | Qt::AlignTop))
                                                                                            << NLPropertyPair(tr("Left-Bottom"), static_cast<int>(Qt::AlignLeft | Qt::AlignBottom))
                                                                                            << NLPropertyPair(tr("Left-Center"), static_cast<int>(Qt::AlignLeft | Qt::AlignVCenter))
                                                                                            << NLPropertyPair(tr("Center-Top"), static_cast<int>(Qt::AlignHCenter | Qt::AlignTop))
                                                                                            << NLPropertyPair(tr("Center-Bottom"), static_cast<int>(Qt::AlignHCenter | Qt::AlignBottom))
                                                                                            )));
    SetPropertyConfig(props);
}

NETLizardTextureRenderer::~NETLizardTextureRenderer()
{
    m_tex = 0;
}

void NETLizardTextureRenderer::Render()
{
    if(!IsActived())
        return;
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

void NETLizardTextureRenderer::Destroy()
{
    SetTexture(0);
    NLRenderable::Destroy();
}

void NETLizardTextureRenderer::SetTexture(texture_s *tex)
{
    if(m_tex != tex)
    {
        m_tex = tex;
        emit propertyChanged("texture", TexturePtr());
    }
}

void NETLizardTextureRenderer::SetAlignment(Qt::Alignment align)
{
    Qt::Alignment a = align;
    a = a & (~Qt::AlignRight);
    if(m_align != a)
    {
        m_align = (Qt::Alignment)a;
        emit propertyChanged("alignment", static_cast<int>(m_align));
    }
}

QVariant NETLizardTextureRenderer::TexturePtr() const { return QVariant::fromValue<NLVariantGeneralPointer>(NLMAKE_VARIANT_VOID_POINTER(struct _texture_s, m_tex)); }
