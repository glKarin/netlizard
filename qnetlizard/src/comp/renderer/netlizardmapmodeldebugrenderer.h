#ifndef _KARIN_NETLIZARDMAPMODELDEBUGRENDERER_H
#define _KARIN_NETLIZARDMAPMODELDEBUGRENDERER_H

#include "nlrenderable.h"
#include "gl/nl_gl.h"

class NLSceneCamera;

NLRENDERER(NETLizardMapModelDebugRenderer)
class NETLizardMapModelDebugRenderer : public NLRenderable
{
public:
    explicit NETLizardMapModelDebugRenderer(NLActor *actor = 0);
    virtual ~NETLizardMapModelDebugRenderer();
    GL_NETLizard_3D_Model * Model() { return m_model; }
    void SetModel(GL_NETLizard_3D_Model *model);
    void SetupScenes(bool b);
    int * Scenes() { return m_scenes; }
    void SetCull(bool b);
    bool Cull() const { return m_cull; }
    void SetSceneCount(int i);
    int SceneCount() const { return m_sceneCount; }
    void SetDebug(int i);
    int Debug() const { return m_debug; }
    void SetRenderScenes(const int scenes[], int count);
    void SetCamera(NLSceneCamera *camera);
    void SetAllScene();

protected:
    virtual void InitRender();
    virtual void Render();
    virtual void DeinitRender();

    private:
    void RenderHighlight();

private:
    GL_NETLizard_3D_Model *m_model;
    bool m_cull;
    int *m_scenes;
    int m_sceneCount;
    int m_debug;
    NLSceneCamera *m_camera;

    Q_DISABLE_COPY(NETLizardMapModelDebugRenderer)
    
};

#endif // _KARIN_NETLIZARDMAPMODELDEBUGRENDERER_H
