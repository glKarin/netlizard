#include "netlizardspriterenderer.h"

#include <QDebug>

#include "gl/nl_gl.h"
#include "engine/nlactor.h"
#include "engine/nlscene.h"
#include "qdef.h"
#include "template/nlvariantpointer.h"

NETLizardSpriteRenderer::NETLizardSpriteRenderer(NLActor *actor) :
    NLRenderable(actor),
    m_sprite(0),
    m_index(-1)
{
    CLASS_NAME(NETLizardSpriteRenderer);
    setObjectName("NETLizardSpriteRenderer");
}

NETLizardSpriteRenderer::~NETLizardSpriteRenderer()
{
    m_sprite = 0;
}

void NETLizardSpriteRenderer::SetIndex(int i)
{
    if(m_index != i)
    {
        m_index = i;
        emit propertyChanged("index", m_index);
    }
}
void NETLizardSpriteRenderer::Render()
{
    if(!IsActived())
        return;
    if(!m_sprite)
        return;
    if(m_index < 0)
        return;
    RenderSprite();
}

void NETLizardSpriteRenderer::Destroy()
{
    SetSprite(0);
    NLRenderable::Destroy();
}

void NETLizardSpriteRenderer::SetSprite(GL_NETLizard_Sprite *f)
{
    if(m_sprite != f)
    {
        m_sprite = f;
        emit propertyChanged("sprite", SpritePtr());
    }
}

void NETLizardSpriteRenderer::RenderSprite()
{
    glPushMatrix();
    {
        //glRotatef(180, 1, 0, 0);
        NETLizard_RenderSpirit(m_sprite, m_index);
    }
    glPopMatrix();
}

QVariant NETLizardSpriteRenderer::SpritePtr() const { return QVariant::fromValue<NLVariantGeneralPointer>(NLMAKE_VARIANT_VOID_POINTER(struct _GL_NETLizard_Sprite, m_sprite)); }
