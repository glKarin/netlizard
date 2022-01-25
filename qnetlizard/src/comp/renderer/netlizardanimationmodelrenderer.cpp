#include "netlizardanimationmodelrenderer.h"

#include <QDebug>

#include "netlizard.h"
#include "gl/nl_gl.h"
#include "qdef.h"

NETLizardAnimationModelRenderer::NETLizardAnimationModelRenderer(NLActor *actor) :
    NLRenderable(actor),
    m_model(0),
    m_config(0),
    m_anim(-1),
    m_frame(-1)
{
    CLASS_NAME(NETLizardAnimationModelRenderer);
    setObjectName("NETLizardAnimationModelRenderer");
}

NETLizardAnimationModelRenderer::~NETLizardAnimationModelRenderer()
{
    m_model = 0;
    m_config = 0;
}

void NETLizardAnimationModelRenderer::Render()
{
    if(!IsActived())
        return;
    if(!m_model || !m_config)
        return;
    if(!m_frame < 0 || m_anim < 0)
        return;
    glPushMatrix();
    {
        NETLizard_RenderGL3DModelFrameAnimation(m_model, m_config, m_anim, m_frame);
    }
    glPopMatrix();
}

void NETLizardAnimationModelRenderer::Destroy()
{
    m_model = 0;
    m_config = 0;
    m_anim = -1;
    m_frame = -1;
    NLRenderable::Destroy();
}

void NETLizardAnimationModelRenderer::SetModel(GL_NETLizard_3D_Model *model, const NETLizard_3D_Frame_Animation *config)
{
    m_model = model;
    m_config = config;
}

void NETLizardAnimationModelRenderer::SetFrame(int frame)
{
    if(m_frame != frame)
        m_frame = frame;
}

void NETLizardAnimationModelRenderer::SetAnim(int anim, int frame)
{
    if(m_anim != anim)
    {
        m_anim = anim;
        SetFrame(frame);
    }
}
