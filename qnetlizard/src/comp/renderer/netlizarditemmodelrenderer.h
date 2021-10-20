#ifndef _KARIN_NETLIZARDITEMMODELRENDERER_H
#define _KARIN_NETLIZARDITEMMODELRENDERER_H

#include "nlrenderable.h"
#include "gl/nl_gl.h"

NLRENDERER(NETLizardItemModelRenderer)
class NETLizardItemModelRenderer : public NLRenderable
{
public:
    explicit NETLizardItemModelRenderer(NLActor *actor = 0);
    virtual ~NETLizardItemModelRenderer();
    GL_NETLizard_3D_Mesh * Model();
    void SetModel(GL_NETLizard_3D_Mesh *model, texture_s **tex);
    void SetIndex(int index);

protected:
    virtual void InitRender();
    virtual void Render();
    virtual void DeinitRender();

private:
    void RenderSky();

private:
    GL_NETLizard_3D_Mesh *m_itemMesh;
    texture_s **m_tex;
    int m_index;

    Q_DISABLE_COPY(NETLizardItemModelRenderer)
    
};

#endif // _KARIN_NETLIZARDITEMMODELRENDERER_H
