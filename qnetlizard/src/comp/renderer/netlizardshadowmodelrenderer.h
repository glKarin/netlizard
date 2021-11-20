#ifndef _KARIN_NETLIZARDSHADOWMODELRENDERER_H
#define _KARIN_NETLIZARDSHADOWMODELRENDERER_H

#include "nlrenderable.h"
#include "gl/nl_gl.h"
#include "stencil_shadow.h"

NLRENDERER(NETLizardShadowModelRenderer)
class NETLizardShadowModelRenderer : public NLRenderable
{
public:
    explicit NETLizardShadowModelRenderer(int method = SHADOW_Z_FAIL, NLActor *actor = 0);
    virtual ~NETLizardShadowModelRenderer();
    GL_NETLizard_3D_Model * Model() { return m_model; }
    void SetModel(GL_NETLizard_3D_Model *model);
    int * Scenes() { return m_scenes; }
    void SetCull(bool b);
    bool Cull() const { return m_cull; }
    void SetSceneCount(int i);
    int SceneCount() const;
    void SetLightSource(const NLVector3 *pos = 0, bool dirlight = false);
    void SetRenderScenes(int scenes[], int count);
    int StencilShadowMethod() const { return m_stencilShadowMethod; }
    void SetStencilShadowMethod(int method);
    void SetLightSourcePosition(const NLVector3 &pos);
    void SetLightSourceType(bool dirlight);
    int ShadowObject() const { return m_shadowObject; }
    void SetShadowObject(int obj);

protected:
    virtual void InitRender();
    virtual void Render();
    virtual void DeinitRender();

    private:
    void SetupScenes(bool b);

private:
    GL_NETLizard_3D_Model *m_model;
    bool m_cull;
    int *m_scenes;
    int m_sceneCount;
    NLVector3 m_lightPosition;
    bool m_directionLighting;
    int m_stencilShadowMethod; // SHADOW_Z_FAIL | SHADOW_Z_PASS
    int m_shadowObject; // 1, 2, 4

    Q_DISABLE_COPY(NETLizardShadowModelRenderer)
    
};

#endif // _KARIN_NETLIZARDSHADOWMODELRENDERER_H
