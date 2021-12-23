#ifndef _KARIN_NETLIZARDMAPMODELRENDERER_H
#define _KARIN_NETLIZARDMAPMODELRENDERER_H

#include "nlrenderable.h"
#include "gl/nl_gl.h"

NLRENDERER(NETLizardMapModelRenderer)
class NETLizardMapModelRenderer : public NLRenderable
{
public:
    explicit NETLizardMapModelRenderer(NLActor *actor = 0);
    virtual ~NETLizardMapModelRenderer();
    GL_NETLizard_3D_Model * Model() { return m_model; }
    void SetModel(GL_NETLizard_3D_Model *model);
    void SetupScenes(bool b);
    int * Scenes() { return m_scenes; }
    void SetCull(bool b);
    bool Cull() const { return m_cull; }
    void SetSceneCount(int i);
    int SceneCount() const { return m_sceneCount; } 
    void SetRenderScenes(const int scenes[], int count);
    void SetAllScene();

protected:
    virtual void InitRender();
    virtual void Render();
    virtual void DeinitRender();

private:
    GL_NETLizard_3D_Model *m_model;
    bool m_cull;
    int *m_scenes;
    int m_sceneCount;

    Q_DISABLE_COPY(NETLizardMapModelRenderer)
    
};

#endif // _KARIN_NETLIZARDMAPMODELRENDERER_H
