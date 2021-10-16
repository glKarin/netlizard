#include "netlizarditemmodelrenderer.h"

#include <QDebug>

#include "qdef.h"

NETLizardItemModelRenderer::NETLizardItemModelRenderer(NLActor *actor) :
    NLRenderable(actor),
    m_itemMesh(0),
    m_tex(0)
{
}

NETLizardItemModelRenderer::~NETLizardItemModelRenderer()
{
    m_itemMesh = 0;
    m_tex = 0;
    DEBUG_DESTROY("NETLizardItemModelRenderer")
}

void NETLizardItemModelRenderer::InitRender()
{

}

void NETLizardItemModelRenderer::Render()
{
    if(!m_itemMesh || !m_tex)
        return;
    if(!m_itemMesh->materials)
        return;
    glPushMatrix();
    {

        glDepthMask(GL_FALSE);
        glDisable(GL_DEPTH_TEST);
        NETLizard_RenderGL3DMesh(m_itemMesh, m_tex);
        glDepthMask(GL_TRUE);
        glEnable(GL_DEPTH_TEST);
    }
    glPopMatrix();
}

void NETLizardItemModelRenderer::DeinitRender()
{
    m_itemMesh = 0;
    m_tex = 0;
}

GL_NETLizard_3D_Mesh * NETLizardItemModelRenderer::Model()
{
    return m_itemMesh;
}

void NETLizardItemModelRenderer::SetModel(GL_NETLizard_3D_Mesh *model, texture_s **tex)
{
    m_itemMesh = model;
    m_tex = tex;
}
