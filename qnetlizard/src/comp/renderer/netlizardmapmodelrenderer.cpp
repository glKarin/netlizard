#include "netlizardmapmodelrenderer.h"

#include <QDebug>

#include "gl/nl_gl.h"
#include "qdef.h"

NETLizardMapModelRenderer::NETLizardMapModelRenderer(NLActor *actor) :
    NLRenderable(actor),
    m_model(0),
    m_cull(false),
    m_scenes(0),
    m_sceneCount(0),
    m_items(0),
    m_itemCount(0),
    m_itemRenderMode(NETLizardMapModelRenderer::RenderItem_Cull)
{
    SetName("NETLizardMapModelRenderer");
}

NETLizardMapModelRenderer::~NETLizardMapModelRenderer()
{
    m_model = 0;
    SetupCull(false);
    DEBUG_DESTROY(NETLizardMapModelRenderer)
}

void NETLizardMapModelRenderer::InitRender()
{

}

void NETLizardMapModelRenderer::Render()
{
    if(!m_model)
        return;
    glPushMatrix();
    {
        if(m_cull)
        {
            if(m_itemRenderMode == NETLizardMapModelRenderer::RenderItem_Scene)
            {
                NETLizard_RenderGL3DMapModel(m_model, m_scenes, m_sceneCount);
            }
            else
            {
                if(m_scenes && m_sceneCount > 0)
                {
                    NETLizard_RenderGL3DMapModelScene(m_model, m_scenes, m_sceneCount);
#if 0
                    fprintf(stderr, "Cull scene(%d): ", m_sceneCount); fflush(stderr);
                    for(int i = 0; i < m_sceneCount; i++)
                    {
                        fprintf(stderr, "%d ", m_scenes[i]); fflush(stderr);
                    }
                    fprintf(stderr, "\n"); fflush(stderr);
#endif
                }
                if(m_items && m_itemCount > 0)
                {
                    NETLizard_RenderGL3DMapModelItem(m_model, m_items, m_itemCount);
#if 0
                    fprintf(stderr, "Cull item(%d): ", m_itemCount); fflush(stderr);
                    for(int i = 0; i < m_itemCount; i++)
                    {
                        fprintf(stderr, "%d ", m_items[i]); fflush(stderr);
                    }
                    fprintf(stderr, "\n"); fflush(stderr);
#endif
                }
            }
        }
        else
        {
            NETLizard_RenderGL3DModel(m_model);
        }
    }
    glPopMatrix();
}

void NETLizardMapModelRenderer::DeinitRender()
{
    m_model = 0;
    SetupCull(false);
}

void NETLizardMapModelRenderer::SetModel(GL_NETLizard_3D_Model *model)
{
    m_model = model;
    SetupCull(false);
    if(m_model)
        SetupCull(m_cull);
}

void NETLizardMapModelRenderer::SetCull(bool b)
{
    if(m_cull != b)
    {
        m_cull = b;
        SetupCull(m_cull);
    }
}

void NETLizardMapModelRenderer::SetupCull(bool b)
{
    if(b)
    {
        if(m_model)
        {
            m_sceneCount = 0;
            m_itemCount = 0;
            m_scenes = (int *)calloc(m_model->count, sizeof(int));
            m_items = (int *)calloc(m_model->item_count, sizeof(int));
        }
    }
    else
    {
        free(m_scenes);
        m_scenes = 0;
        m_sceneCount = 0;
        free(m_items);
        m_items = 0;
        m_itemCount = 0;
    }
}

void NETLizardMapModelRenderer::SetSceneCount(int i)
{
    if(!m_cull && i > 0)
        return;
    if(m_sceneCount != i)
        m_sceneCount = i;
}

void NETLizardMapModelRenderer::SetRenderScenes(const int scenes[], int count)
{
    if(!m_scenes)
        return;
    m_sceneCount = count;
    if(count > 0)
        memcpy(m_scenes, scenes, count * sizeof(int));
}

void NETLizardMapModelRenderer::SetAllScene()
{
    if(!m_scenes)
        return;
    for(GLuint i = 0; i < m_model->count; i++)
        m_scenes[i] = i;
    m_sceneCount = m_model->count;
}

void NETLizardMapModelRenderer::SetRenderItems(const int items[], int count)
{
    if(!m_items)
        return;
    m_itemCount = count;
    if(count > 0)
        memcpy(m_items, items, count * sizeof(int));
}

void NETLizardMapModelRenderer::SetAllItems()
{
    if(!m_items)
        return;
    for(GLuint i = 0; i < m_model->item_count; i++)
        m_items[i] = i;
    m_itemCount = m_model->item_count;
}

void NETLizardMapModelRenderer::SetItemCount(int i)
{
    if(!m_cull && i > 0)
        return;
    if(m_itemCount != i)
        m_itemCount = i;
}

void NETLizardMapModelRenderer::SetRenderItemMode(NETLizardMapModelRenderer::RenderItemMode_e mode)
{
    if(m_itemRenderMode != mode)
        m_itemRenderMode = mode;
}
