#include "netlizarditemmodelrenderer.h"

#include <QDebug>

#include "gl/nl_gl.h"
#include "qdef.h"
#include "template/nlvariantpointer.h"

NETLizardItemModelRenderer::NETLizardItemModelRenderer(NLActor *actor) :
    NLRenderable(actor),
    m_itemMesh(0),
    m_tex(0),
    m_index(0)
{
    CLASS_NAME(NETLizardItemModelRenderer);
    setObjectName("NETLizardItemModelRenderer");
}

NETLizardItemModelRenderer::~NETLizardItemModelRenderer()
{
    m_itemMesh = 0;
    m_tex = 0;
}

void NETLizardItemModelRenderer::Render()
{
    if(!IsActived())
        return;
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

void NETLizardItemModelRenderer::Destroy()
{
    SetModel(0, 0);
    SetIndex(0);
    NLRenderable::Destroy();
}

void NETLizardItemModelRenderer::SetModel(GL_NETLizard_3D_Mesh *model, texture_s **tex)
{
    if(m_itemMesh != model)
    {
        m_itemMesh = model;
        emit propertyChanged("meshes", MeshesPtr());
    }
    if(m_tex != tex)
    {
        m_tex = tex;
        emit propertyChanged("textures", TexturesPtr());
    }
}

void NETLizardItemModelRenderer::SetIndex(int index)
{
    if(m_index != index)
    {
        m_index = index;
        emit propertyChanged("index", m_index);
    }
}

QVariant NETLizardItemModelRenderer::TexturesPtr() const { return QVariant::fromValue<NLVariantGeneralPointer>(NLMAKE_VARIANT_VOID_POINTER(struct _texture_s *, m_tex)); }

QVariant NETLizardItemModelRenderer::MeshesPtr() const { return QVariant::fromValue<NLVariantGeneralPointer>(NLMAKE_VARIANT_VOID_POINTER(struct _GL_NETLizard_3D_Mesh *, m_itemMesh)); }
