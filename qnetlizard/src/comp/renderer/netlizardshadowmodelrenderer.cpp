#include "netlizardshadowmodelrenderer.h"

#include <QDebug>

#include "stencil_shadow.h"
#include "nl_shadow_render.h"
#include "gl/nl_gl.h"
#include "qdef.h"

NETLizardShadowModelRenderer::NETLizardShadowModelRenderer(int method, NLActor *actor) :
    NLRenderable(actor),
    m_model(0),
    m_cull(false),
    m_scenes(0),
    m_sceneCount(0),
    m_directionLighting(false),
    m_stencilShadowMethod(method),
    m_shadowObject(NETLIZARD_SHADOW_RENDER_ITEM)
{
    CLASS_NAME(NETLizardShadowModelRenderer);
    setObjectName("NETLizardShadowModelRenderer");
    SetLightSource();
    NLProperties props = PropertyConfig();
    props.Insert("shadowObject",  NLProperties("enum", QVariant::fromValue<NLPropertyPairList>(NLPropertyPairList()
                                                                                            << NLPropertyPair(tr("Only render item shadow"), NETLIZARD_SHADOW_RENDER_ITEM)
                                                                                            << NLPropertyPair(tr("Only render scene wall shadow"), NETLIZARD_SHADOW_RENDER_SCENE_WALL)
                                                                                            << NLPropertyPair(tr("Only render scene shadow"), NETLIZARD_SHADOW_RENDER_SCENE)
                                                                                               << NLPropertyPair(tr("Render item and scene wall shadow"), NETLIZARD_SHADOW_RENDER_ITEM | NETLIZARD_SHADOW_RENDER_SCENE_WALL)
                                                                                               << NLPropertyPair(tr("Render all shadow"), NETLIZARD_SHADOW_RENDER_ALL)
                                                                                            )));
    props.Insert("shadowMethod",  NLProperties("enum", QVariant::fromValue<NLPropertyPairList>(NLPropertyPairList()
                                                                                            << NLPropertyPair(tr("No shadow"), 0)
                                                                                            << NLPropertyPair(tr("Stencil shadow(Z-FAIL)"), SHADOW_Z_FAIL)
                                                                                            << NLPropertyPair(tr("Stencil shadow(Z-PASS)"), SHADOW_Z_PASS)
                                                                                            )));
    SetPropertyConfig(props);
}

NETLizardShadowModelRenderer::~NETLizardShadowModelRenderer()
{
    m_model = 0;
    SetupScenes(false);
}

void NETLizardShadowModelRenderer::Render()
{
    if(!IsActived())
        return;
    if(!m_model)
        return;
    if(m_stencilShadowMethod == 0)
        return;
    if(vector3_iszero(&m_lightPosition))
        return;
    glPushMatrix();
    {
        if(m_cull)
        {
            if(m_scenes && m_sceneCount > 0)
            {
                NETLizard_RenderNETLizardModelSceneShadow(m_model, m_scenes, m_sceneCount, &m_lightPosition, m_directionLighting, m_stencilShadowMethod, m_shadowObject);
            }
        }
        else
        {
            NETLizard_RenderNETLizardModelShadow(m_model, &m_lightPosition, m_directionLighting, m_stencilShadowMethod, m_shadowObject);
        }
    }
    glPopMatrix();
}

void NETLizardShadowModelRenderer::Destroy()
{
    SetModel(0);
    NLRenderable::Destroy();
}

void NETLizardShadowModelRenderer::SetModel(GL_NETLizard_3D_Model *model)
{
    if(m_model != model)
    {
        m_model = model;
        SetupScenes(false);
        if(m_model)
            SetupScenes(m_cull);
        emit propertyChanged("model", ModelPtr());
    }
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

void NETLizardShadowModelRenderer::SetLightSourcePosition(const NLVector3 &pos)
{
    if(!vector3_equals(&m_lightPosition, &pos))
    {
        vector3_assignv(&m_lightPosition, &pos);
        if(!vector3_iszero(&m_lightPosition))
        {
            if(m_directionLighting)
                vector3_normalizev(&m_lightPosition);
        }
    }
}

void NETLizardShadowModelRenderer::SetLightSourceType(bool dirlight)
{
    if(m_directionLighting != dirlight)
    {
        m_directionLighting = dirlight;
        if(!vector3_iszero(&m_lightPosition))
        {
            if(m_directionLighting)
                vector3_normalizev(&m_lightPosition);
        }
    }
}

void NETLizardShadowModelRenderer::SetRenderScenes(const int scenes[], int count)
{
    if(!m_scenes)
        return;
    m_sceneCount = count;
    if(count > 0)
        memcpy(m_scenes, scenes, count * sizeof(int));
}

void NETLizardShadowModelRenderer::SetStencilShadowMethod(int method)
{
    if(m_stencilShadowMethod != method)
    {
        m_stencilShadowMethod = method;
        emit propertyChanged("shadowMethod", m_stencilShadowMethod);
    }
}

void NETLizardShadowModelRenderer::SetShadowObject(int obj)
{
    if(m_shadowObject != obj)
    {
        m_shadowObject = obj;
        emit propertyChanged("shadowObject", m_shadowObject);
    }
}

void NETLizardShadowModelRenderer::SetAllScenes()
{
    if(!m_scenes)
        return;
    for(GLuint i = 0; i < m_model->count; i++)
        m_scenes[i] = i;
    m_sceneCount = m_model->count;
}
