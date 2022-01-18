#ifndef _KARIN_NETLIZARDANIMATIONMODELRENDERER_H
#define _KARIN_NETLIZARDANIMATIONMODELRENDERER_H

#include "engine/nlrenderable.h"

struct _GL_NETLizard_3D_Model;
struct NETLizard_3D_Frame_Animation_s;

NLRENDERER(NETLizardAnimationModelRenderer)
class NETLizardAnimationModelRenderer : public NLRenderable
{
public:
    explicit NETLizardAnimationModelRenderer(NLActor *actor = 0);
    virtual ~NETLizardAnimationModelRenderer();
    struct _GL_NETLizard_3D_Model * Model() { return m_model; }
    const struct NETLizard_3D_Frame_Animation_s * Config() const { return m_config; }
    void SetModel(struct _GL_NETLizard_3D_Model *model, const struct NETLizard_3D_Frame_Animation_s *config);
    void SetFrame(int frame);
    void SetAnim(int anim, int frame = 0);
    int Anim() const { return m_anim; }
    int Frame() const { return m_frame; }

protected:
    virtual void InitRender();
    virtual void Render();
    virtual void DeinitRender();

private:
    struct _GL_NETLizard_3D_Model *m_model;
    const struct NETLizard_3D_Frame_Animation_s *m_config;
    int m_anim;
    int m_frame;

    Q_DISABLE_COPY(NETLizardAnimationModelRenderer)
    
};

#endif // _KARIN_NETLIZARDANIMATIONMODELRENDERER_H
