#include "netlizardspriterenderer.h"

#include <QDebug>

#include <GL/gl.h>

#include "nlactor.h"
#include "nlscene.h"
#include "qdef.h"

NETLizardSpriteRenderer::NETLizardSpriteRenderer(NLActor *actor) :
    NLRenderable(actor),
    m_sprite(0),
    m_index(-1)
{
    SetName("NETLizardSpriteRenderer");
}

NETLizardSpriteRenderer::~NETLizardSpriteRenderer()
{
    m_sprite = 0;
    DEBUG_DESTROY(NETLizardSpriteRenderer)
}

void NETLizardSpriteRenderer::InitRender()
{

}

void NETLizardSpriteRenderer::SetIndex(int i)
{
    if(m_index != i)
    {
        m_index = i;
    }
}
void NETLizardSpriteRenderer::Render()
{
    if(!m_sprite)
        return;
    if(m_index < 0)
        return;
    RenderSprite();
}

void NETLizardSpriteRenderer::DeinitRender()
{
    m_sprite = 0;
}

void NETLizardSpriteRenderer::SetSprite(GL_NETLizard_Sprite *f)
{
    if(m_sprite != f)
        m_sprite = f;
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

