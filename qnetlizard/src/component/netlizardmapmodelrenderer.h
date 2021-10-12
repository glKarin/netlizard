#ifndef _KARIN_NETLIZARDMAPMODELRENDERER_H
#define _KARIN_NETLIZARDMAPMODELRENDERER_H

#include "nlrenderable.h"
#include "gl/nl_gl.h"

class NETLizardMapModelRenderer : public NLRenderable
{
public:
    explicit NETLizardMapModelRenderer(NLActor *actor = 0);
    virtual ~NETLizardMapModelRenderer();
    GL_NETLizard_3D_Model * Model();
    void SetModel(GL_NETLizard_3D_Model *model);

protected:
    virtual void InitRender();
    virtual void Render();
    virtual void DeinitRender();

private:
    void RenderSky();

private:
    GL_NETLizard_3D_Model *m_model;

    Q_DISABLE_COPY(NETLizardMapModelRenderer)
    
};

#endif // _KARIN_NETLIZARDMAPMODELRENDERER_H
