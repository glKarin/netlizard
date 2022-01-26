#ifndef _KARIN_NETLIZARDSHADOWMODELRENDERER_H
#define _KARIN_NETLIZARDSHADOWMODELRENDERER_H

#include "engine/nlrenderable.h"

struct _GL_NETLizard_3D_Model;

NLRENDERER(NETLizardShadowModelRenderer)
class NETLizardShadowModelRenderer : public NLRenderable
{
    Q_OBJECT
    Q_PROPERTY(int shadowMethod READ StencilShadowMethod WRITE SetStencilShadowMethod FINAL)
    Q_PROPERTY(int shadowObject READ ShadowObject WRITE SetShadowObject FINAL)
    Q_PROPERTY(QVariant model READ ModelPtr FINAL)
public:
    explicit NETLizardShadowModelRenderer(int method = 2, NLActor *actor = 0);
    virtual ~NETLizardShadowModelRenderer();
    struct _GL_NETLizard_3D_Model * Model() { return m_model; }
    void SetModel(struct _GL_NETLizard_3D_Model *model);
    int * Scenes() { return m_scenes; }
    void SetCull(bool b);
    bool Cull() const { return m_cull; }
    void SetSceneCount(int i);
    int SceneCount() const;
    void SetLightSource(const NLVector3 *pos = 0, bool dirlight = false);
    void SetRenderScenes(const int scenes[], int count);
    int StencilShadowMethod() const { return m_stencilShadowMethod; }
    void SetStencilShadowMethod(int method);
    void SetLightSourcePosition(const NLVector3 &pos);
    void SetLightSourceType(bool dirlight);
    int ShadowObject() const { return m_shadowObject; }
    void SetShadowObject(int obj);
    void SetAllScenes();
    QVariant ModelPtr() const { return QVariant::fromValue<NLVariantGeneralPointer>(NLMAKE_VARIANT_VOID_POINTER(struct _GL_NETLizard_3D_Model, m_model)); }

protected:
    virtual void Render();
    virtual void Destroy();

    private:
    void SetupScenes(bool b);

private:
    struct _GL_NETLizard_3D_Model *m_model;
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
