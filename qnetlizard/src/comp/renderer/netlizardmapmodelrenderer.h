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
    GL_NETLizard_3D_Model * Model();
    void SetModel(GL_NETLizard_3D_Model *model);
    void SetScenes(bool b);
    int * Scenes();
    void SetCull(bool b);
    bool Cull() const;
    void SetSceneCount(int i);
    int SceneCount() const;

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
