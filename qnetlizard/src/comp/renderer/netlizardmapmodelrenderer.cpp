#include "netlizardmapmodelrenderer.h"

#include <QDebug>

#include "qdef.h"

NETLizardMapModelRenderer::NETLizardMapModelRenderer(NLActor *actor) :
    NLRenderable(actor),
    m_model(0)
{
}

NETLizardMapModelRenderer::~NETLizardMapModelRenderer()
{
    m_model = 0;
    DEBUG_DESTROY("NETLizardMapModelRenderer")
}

void NETLizardMapModelRenderer::InitRender()
{

}

void NETLizardMapModelRenderer::Render()
{
    if(!m_model)
        return;
    glPushMatrix();
    {
        NETLizard_RenderGL3DModel(m_model);
    }
    glPopMatrix();
}

void NETLizardMapModelRenderer::DeinitRender()
{
    m_model = 0;
}

GL_NETLizard_3D_Model * NETLizardMapModelRenderer::Model()
{
    return m_model;
}

void NETLizardMapModelRenderer::SetModel(GL_NETLizard_3D_Model *model)
{
    m_model = model;
}

