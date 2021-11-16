#include "netlizarditemmodelrenderer.h"

#include <QDebug>

#include "qdef.h"

NETLizardItemModelRenderer::NETLizardItemModelRenderer(NLActor *actor) :
    NLRenderable(actor),
    m_itemMesh(0),
    m_tex(0),
    m_index(0)
{
    SetName("NETLizardItemModelRenderer");
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
    if(!m_index < 0)
        return;
    if(!m_itemMesh || !m_tex)
        return;
    if(!m_itemMesh->materials)
        return;
    glPushMatrix();
    {
        NETLizard_RenderGL3DMesh(m_itemMesh + m_index, m_tex);
    }
    glPopMatrix();
}

void NETLizardItemModelRenderer::DeinitRender()
{
    m_itemMesh = 0;
    m_tex = 0;
    m_index = 0;
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

void NETLizardItemModelRenderer::SetIndex(int index)
{
    if(m_index != index)
        m_index = index;
}
