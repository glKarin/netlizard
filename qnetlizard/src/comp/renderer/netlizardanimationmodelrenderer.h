#ifndef _KARIN_NETLIZARDANIMATIONMODELRENDERER_H
#define _KARIN_NETLIZARDANIMATIONMODELRENDERER_H

#include "nlrenderable.h"
#include "gl/nl_gl.h"

NLRENDERER(NETLizardAnimationModelRenderer)
class NETLizardAnimationModelRenderer : public NLRenderable
{
public:
    explicit NETLizardAnimationModelRenderer(NLActor *actor = 0);
    virtual ~NETLizardAnimationModelRenderer();
    GL_NETLizard_3D_Model * Model();
    const NETLizard_3D_Frame_Animation * Config() const;
    void SetModel(GL_NETLizard_3D_Model *model, const NETLizard_3D_Frame_Animation *config);
    void SetFrame(int frame);
    void SetAnim(int anim, int frame = 0);
    int Anim() const;
    int Frame() const;

protected:
    virtual void InitRender();
    virtual void Render();
    virtual void DeinitRender();

private:
    void RenderSky();

private:
    GL_NETLizard_3D_Model *m_model;
    const NETLizard_3D_Frame_Animation *m_config;
    int m_anim;
    int m_frame;

    Q_DISABLE_COPY(NETLizardAnimationModelRenderer)
    
};

#endif // _KARIN_NETLIZARDANIMATIONMODELRENDERER_H
