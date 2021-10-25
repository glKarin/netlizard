#include "netlizardshadowmodelrenderer.h"

#include <QDebug>

#include "qdef.h"
#include "nl_shadow.h"

NETLizardShadowModelRenderer::NETLizardShadowModelRenderer(NLActor *actor) :
    NLRenderable(actor),
    m_model(0),
    m_cull(false),
    m_scenes(0),
    m_sceneCount(0),
    m_directionLighting(false)
{
    SetLightSource();
}

NETLizardShadowModelRenderer::~NETLizardShadowModelRenderer()
{
    m_model = 0;
    SetupScenes(false);
    DEBUG_DESTROY("NETLizardShadowModelRenderer")
}

void NETLizardShadowModelRenderer::InitRender()
{

}

void NETLizardShadowModelRenderer::Render()
{
    if(!m_model)
        return;
    if(vector3_iszero(&m_lightPosition))
        return;
    glPushMatrix();
    {
        if(m_cull)
        {
            if(m_scenes && m_sceneCount > 0)
            {
                Shadow_RenderNETLizardModelScene(m_model, m_scenes, m_sceneCount, &m_lightPosition, m_directionLighting, SHADOW_Z_PASS);
                Shadow_RenderMask();
            }
        }
        else
        {
            Shadow_RenderNETLizardModel(m_model, &m_lightPosition, m_directionLighting, SHADOW_Z_PASS);
            Shadow_RenderMask();
        }
    }
    glPopMatrix();
}

void NETLizardShadowModelRenderer::DeinitRender()
{
    m_model = 0;
    SetupScenes(false);
}

GL_NETLizard_3D_Model * NETLizardShadowModelRenderer::Model()
{
    return m_model;
}

void NETLizardShadowModelRenderer::SetModel(GL_NETLizard_3D_Model *model)
{
    m_model = model;
    SetupScenes(false);
    if(m_model)
        SetupScenes(m_cull);
}

void NETLizardShadowModelRenderer::SetCull(bool b)
{
    if(m_cull != b)
    {
        m_cull = b;
        SetupScenes(m_cull);
    }
}

void NETLizardShadowModelRenderer::SetupScenes(bool b)
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

bool NETLizardShadowModelRenderer::Cull() const
{
    return m_cull;
}

int * NETLizardShadowModelRenderer::Scenes()
{
    return m_scenes;
}

void NETLizardShadowModelRenderer::SetSceneCount(int i)
{
    if(!m_cull && i > 0)
        return;
    if(m_sceneCount != i)
        m_sceneCount = i;
}

int NETLizardShadowModelRenderer::SceneCount() const
{
    return m_sceneCount;
}

void NETLizardShadowModelRenderer::SetLightSource(const NLVector3 *pos, bool dirlight)
{
    if(pos)
    {
        vector3_assignv(&m_lightPosition, pos);
    }
    else
    {
        vector3_identityv(&m_lightPosition);
    }
    if(!vector3_iszero(&m_lightPosition))
    {
        if(dirlight)
            vector3_normalizev(&m_lightPosition);
    }
    m_directionLighting = dirlight;
}

void NETLizardShadowModelRenderer::SetRenderScenes(int scenes[], int count)
{
    if(!m_scenes)
        return;
    m_sceneCount = count;
    if(count > 0)
        memcpy(m_scenes, scenes, count * sizeof(int));
}
