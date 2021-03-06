#ifndef _KARIN_NETLIZARDMAPMODELDEBUGRENDERER_H
#define _KARIN_NETLIZARDMAPMODELDEBUGRENDERER_H

#include "engine/nlrenderable.h"

class NLSceneCamera;
struct _GL_NETLizard_3D_Model;

NLRENDERER(NETLizardMapModelDebugRenderer)
class NETLizardMapModelDebugRenderer : public NLRenderable
{
    Q_OBJECT
    Q_PROPERTY(int renderDebug READ Debug WRITE SetDebug FINAL)
    Q_PROPERTY(QVariant model READ ModelPtr FINAL)
public:
    explicit NETLizardMapModelDebugRenderer(NLActor *actor = 0);
    virtual ~NETLizardMapModelDebugRenderer();
    struct _GL_NETLizard_3D_Model * Model() { return m_model; }
    void SetModel(struct _GL_NETLizard_3D_Model *model);
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
    void SetAllScenes();
    QVariant ModelPtr() const;

protected:
    virtual void Render();
    virtual void Destroy();

    private:
    void RenderHighlight();

private:
    struct _GL_NETLizard_3D_Model *m_model;
    bool m_cull;
    int *m_scenes;
    int m_sceneCount;
    int m_debug;
    NLSceneCamera *m_camera;

    Q_DISABLE_COPY(NETLizardMapModelDebugRenderer)
    
};

#endif // _KARIN_NETLIZARDMAPMODELDEBUGRENDERER_H
