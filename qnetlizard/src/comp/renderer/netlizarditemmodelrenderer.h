#ifndef _KARIN_NETLIZARDITEMMODELRENDERER_H
#define _KARIN_NETLIZARDITEMMODELRENDERER_H

#include "engine/nlrenderable.h"

struct _GL_NETLizard_3D_Mesh;
struct _texture_s;

NLRENDERER(NETLizardItemModelRenderer)
class NETLizardItemModelRenderer : public NLRenderable
{
    Q_OBJECT
    Q_PROPERTY(int index READ Index WRITE SetIndex FINAL)
    Q_PROPERTY(QVariant meshes READ MeshesPtr FINAL)
    Q_PROPERTY(QVariant textures READ TexturesPtr FINAL)
public:
    explicit NETLizardItemModelRenderer(NLActor *actor = 0);
    virtual ~NETLizardItemModelRenderer();
    struct _GL_NETLizard_3D_Mesh * Model() { return m_itemMesh; }
    void SetModel(struct _GL_NETLizard_3D_Mesh *model, struct _texture_s **tex);
    void SetIndex(int index);
    int Index() const { return m_index; }
    QVariant TexturesPtr() const { return QVariant::fromValue<NLVariantGeneralPointer>(NLMAKE_VARIANT_VOID_POINTER(struct _texture_s *, m_tex)); }
    QVariant MeshesPtr() const { return QVariant::fromValue<NLVariantGeneralPointer>(NLMAKE_VARIANT_VOID_POINTER(struct _GL_NETLizard_3D_Mesh *, m_itemMesh)); }

protected:
    virtual void Render();
    virtual void Destroy();

private:
    struct _GL_NETLizard_3D_Mesh *m_itemMesh;
    struct _texture_s **m_tex;
    int m_index;

    Q_DISABLE_COPY(NETLizardItemModelRenderer)
    
};

#endif // _KARIN_NETLIZARDITEMMODELRENDERER_H
