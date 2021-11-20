#ifndef _KARIN_NLMODELRENDERER_H
#define _KARIN_NLMODELRENDERER_H

#include "nlrenderable.h"
#include "nlrendermodel.h"

class NLModelRenderer : public NLRenderable
{
public:
    explicit NLModelRenderer(NLActor *actor = 0);
    virtual ~NLModelRenderer();
    NLRenderModelGLGeneral * Model() { return m_model; }
    NLRenderModelGLGeneral * Take();
    void SetModel(NLRenderModelGLGeneral *model);

protected:
    virtual void InitRender();
    virtual void Render();
    virtual void DeinitRender();

private:
    NLRenderModelGLGeneral *m_model;

    Q_DISABLE_COPY(NLModelRenderer)
    
};

#endif // _KARIN_NLMODELRENDERER_H
