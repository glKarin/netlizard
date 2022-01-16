#ifndef _KARIN_NETLIZARDITEMMODELRENDERER_H
#define _KARIN_NETLIZARDITEMMODELRENDERER_H

#include "engine/nlrenderable.h"

struct _GL_NETLizard_3D_Mesh;
struct _texture_s;

NLRENDERER(NETLizardItemModelRenderer)
class NETLizardItemModelRenderer : public NLRenderable
{
public:
    explicit NETLizardItemModelRenderer(NLActor *actor = 0);
    virtual ~NETLizardItemModelRenderer();
    struct _GL_NETLizard_3D_Mesh * Model() { return m_itemMesh; }
    void SetModel(struct _GL_NETLizard_3D_Mesh *model, struct _texture_s **tex);
    void SetIndex(int index);

protected:
    virtual void InitRender();
    virtual void Render();
    virtual void DeinitRender();

private:
    struct _GL_NETLizard_3D_Mesh *m_itemMesh;
    struct _texture_s **m_tex;
    int m_index;

    Q_DISABLE_COPY(NETLizardItemModelRenderer)
    
};

#endif // _KARIN_NETLIZARDITEMMODELRENDERER_H
