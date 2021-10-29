#include "netlizardmapmodelrenderer.h"

#include <QDebug>

#include "nl_gl_debug.h"
#include "qdef.h"

NETLizardMapModelRenderer::NETLizardMapModelRenderer(NLActor *actor) :
    NLRenderable(actor),
    m_model(0),
    m_cull(false),
    m_scenes(0),
    m_sceneCount(0),
    m_debug(NETLIZARD_DEBUG_RENDER_NONE)
{
}

NETLizardMapModelRenderer::~NETLizardMapModelRenderer()
{
    m_model = 0;
    SetScenes(false);
    DEBUG_DESTROY("NETLizardMapModelRenderer")
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
            if(m_scenes && m_sceneCount > 0)
            {
                NETLizard_RenderGL3DMapModelScene(m_model, m_scenes, m_sceneCount);
                if(m_debug)
                {
                    if(m_debug & NETLIZARD_DEBUG_RENDER_MAP_BOUND)
                        NETLizard_DebugRenderGL3DMapModelBound(m_model);
                    if(m_debug & NETLIZARD_DEBUG_RENDER_VERTEX_NORMAL)
                        NETLizard_DebugRenderGL3DMapModelScene_VertexNormal(m_model, m_scenes, m_sceneCount);
                    if(m_debug & NETLIZARD_DEBUG_RENDER_SCENE_BOUND)
                        NETLizard_DebugRenderGL3DMapModelScene_Bound(m_model, m_scenes, m_sceneCount);
                }
            }
        }
        else
        {
            NETLizard_RenderGL3DModel(m_model);
        }
    }
    glPopMatrix();
    RenderDebug();
}

void NETLizardMapModelRenderer::DeinitRender()
{
    m_model = 0;
    SetScenes(false);
}

void NETLizardMapModelRenderer::RenderDebug()
{
    if(!m_model)
        return;
    if(!m_debug)
        return;
    glPushMatrix();
    {
        if(m_cull)
        {
#if 0
            if(m_scenes)
            {
                qDebug() << "Render scene count: " << m_sceneCount;
                QString str;
                for(int i = 0; i < m_sceneCount; i++)
                {
                    str += QString::number(m_scenes[i]) + ", ";
                }
                qDebug() << str;
            }
#endif

            if(m_scenes && m_sceneCount > 0)
            {
                if(m_debug & NETLIZARD_DEBUG_RENDER_MAP_BOUND)
                    NETLizard_DebugRenderGL3DMapModelBound(m_model);
                if(m_debug & NETLIZARD_DEBUG_RENDER_VERTEX_NORMAL)
                    NETLizard_DebugRenderGL3DMapModelScene_VertexNormal(m_model, m_scenes, m_sceneCount);
                if(m_debug & NETLIZARD_DEBUG_RENDER_SCENE_BOUND)
                    NETLizard_DebugRenderGL3DMapModelScene_Bound(m_model, m_scenes, m_sceneCount);
            }
        }
        else
        {
            if(m_debug & NETLIZARD_DEBUG_RENDER_MAP_BOUND)
                NETLizard_DebugRenderGL3DMapModelBound(m_model);
            if(m_debug & NETLIZARD_DEBUG_RENDER_VERTEX_NORMAL)
                NETLizard_DebugRenderGL3DModel_VertexNormal(m_model);
            if(m_debug & NETLIZARD_DEBUG_RENDER_SCENE_BOUND)
                NETLizard_DebugRenderGL3DModel_Bound(m_model);
        }
    }
    glPopMatrix();
}

GL_NETLizard_3D_Model * NETLizardMapModelRenderer::Model()
{
    return m_model;
}

void NETLizardMapModelRenderer::SetModel(GL_NETLizard_3D_Model *model)
{
    m_model = model;
    SetScenes(false);
    if(m_model)
        SetScenes(m_cull);
}

void NETLizardMapModelRenderer::SetCull(bool b)
{
    if(m_cull != b)
    {
        m_cull = b;
        SetScenes(m_cull);
    }
}

void NETLizardMapModelRenderer::SetScenes(bool b)
{
    if(b)
    {
        if(m_model)
        {
            m_sceneCount = 0;
            m_scenes = (int *)calloc(m_model->count, sizeof(int));
        }
    }
    else
    {
        free(m_scenes);
        m_scenes = 0;
        m_sceneCount = 0;
    }
}

bool NETLizardMapModelRenderer::Cull() const
{
    return m_cull;
}

int * NETLizardMapModelRenderer::Scenes()
{
    return m_scenes;
}

void NETLizardMapModelRenderer::SetSceneCount(int i)
{
    if(!m_cull && i > 0)
        return;
    if(m_sceneCount != i)
        m_sceneCount = i;
}

int NETLizardMapModelRenderer::SceneCount() const
{
    return m_sceneCount;
}

void NETLizardMapModelRenderer::SetDebug(int i)
{
    if(m_debug != i)
        m_debug = i;
}

int NETLizardMapModelRenderer::Debug() const
{
    return m_debug;
}
