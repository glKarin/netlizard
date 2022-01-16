#ifndef _KARIN_NETLIZARDMAPMODELRENDERER_H
#define _KARIN_NETLIZARDMAPMODELRENDERER_H

#include "engine/nlrenderable.h"

struct _GL_NETLizard_3D_Model;

NLRENDERER(NETLizardMapModelRenderer)
class NETLizardMapModelRenderer : public NLRenderable
{
    public:
    enum RenderItemMode_e
    {
        RenderItem_Scene = 1,
        RenderItem_Cull = 2
    };

public:
    explicit NETLizardMapModelRenderer(NLActor *actor = 0);
    virtual ~NETLizardMapModelRenderer();
    struct _GL_NETLizard_3D_Model * Model() { return m_model; }
    void SetModel(struct _GL_NETLizard_3D_Model *model);
    void SetupCull(bool b);
    int * Scenes() { return m_scenes; }
    void SetCull(bool b);
    bool Cull() const { return m_cull; }
    void SetSceneCount(int i);
    int SceneCount() const { return m_sceneCount; } 
    void SetRenderScenes(const int scenes[], int count);
    void SetAllScenes();
    void SetRenderItems(const int items[], int count);
    void SetAllItems();
    int * Items() { return m_items; }
    void SetItemCount(int i);
    int ItemCount() const { return m_itemCount; }
    void SetAllScene();
    void SetRenderItemMode(RenderItemMode_e mode);
    RenderItemMode_e RenderItemMode() const { return m_itemRenderMode; }

protected:
    virtual void InitRender();
    virtual void Render();
    virtual void DeinitRender();

private:
    struct _GL_NETLizard_3D_Model *m_model;
    bool m_cull;
    int *m_scenes;
    int m_sceneCount;
    int *m_items;
    int m_itemCount;
    RenderItemMode_e m_itemRenderMode;

    Q_DISABLE_COPY(NETLizardMapModelRenderer)
    
};

#endif // _KARIN_NETLIZARDMAPMODELRENDERER_H
