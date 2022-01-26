#include "netlizardmapmodeldebugrenderer.h"

#include <QDebug>

#include "gl/nl_gl.h"
#include "gl/nl_util.h"
#include "nl_gl_debug.h"
#include "nl_gl_algo_debug.h"
#include "nl_algo.h"
#include "engine/nlscenecamera.h"
#include "math/matrix.h"
#include "qdef.h"

NETLizardMapModelDebugRenderer::NETLizardMapModelDebugRenderer(NLActor *actor) :
    NLRenderable(actor),
    m_model(0),
    m_cull(false),
    m_scenes(0),
    m_sceneCount(0),
    m_debug(NETLIZARD_DEBUG_RENDER_NONE),
    m_camera(0)
{
    CLASS_NAME(NETLizardMapModelDebugRenderer);
    setObjectName("NETLizardMapModelDebugRenderer");
    NLProperties props = PropertyConfig();
    props.Insert("renderDebug",  NLProperties("option", QVariant::fromValue<NLPropertyPairList>(NLPropertyPairList()
                                                                                            << NLPropertyPair(tr("Render map bound"), NETLIZARD_DEBUG_RENDER_MAP_BOUND)
                                                                                            << NLPropertyPair(tr("Render item vertex and normal"), NETLIZARD_DEBUG_RENDER_ITEM_VERTEX_NORMAL)
                                                                                            << NLPropertyPair(tr("Render scene vertex and normal"), NETLIZARD_DEBUG_RENDER_SCENE_VERTEX_NORMAL)
                                                                                                 << NLPropertyPair(tr("Render item bound"), NETLIZARD_DEBUG_RENDER_ITEM_BOUND)
                                                                                                 << NLPropertyPair(tr("Render scene bound"), NETLIZARD_DEBUG_RENDER_SCENE_BOUND)
                                                                                                 << NLPropertyPair(tr("Render item plane"), NETLIZARD_DEBUG_RENDER_ITEM_PLANE)
                                                                                                 << NLPropertyPair(tr("Render scene plane"), NETLIZARD_DEBUG_RENDER_SCENE_PLANE)
                                                                                                 << NLPropertyPair(tr("Render scene BSP"), NETLIZARD_DEBUG_RENDER_MAP_BSP)
                                                                                                 << NLPropertyPair(tr("Render highlight view scene plane"), NETLIZARD_DEBUG_RENDER_HIGHLIGHT_VIEW_SCENE_PLANE)
                                                                                                 << NLPropertyPair(tr("Render highlight view item"), NETLIZARD_DEBUG_RENDER_HIGHLIGHT_VIEW_ITEM)
                                                                                            )));
    SetPropertyConfig(props);
}

NETLizardMapModelDebugRenderer::~NETLizardMapModelDebugRenderer()
{
    m_model = 0;
    SetupScenes(false);
}

void NETLizardMapModelDebugRenderer::Render()
{
    if(!IsActived())
        return;
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

                int m = 0;
                if(m_debug & NETLIZARD_DEBUG_RENDER_ITEM_VERTEX_NORMAL)
                    m |= NETLIZARD_DEBUG_TYPE_ITEM;
                if(m_debug & NETLIZARD_DEBUG_RENDER_SCENE_VERTEX_NORMAL)
                    m |= NETLIZARD_DEBUG_TYPE_SCENE;
                if(m)
                    NETLizard_DebugRenderGL3DMapModelScene_VertexNormal(m_model, m_scenes, m_sceneCount, m);

                m = 0;
                if(m_debug & NETLIZARD_DEBUG_RENDER_ITEM_BOUND)
                    m |= NETLIZARD_DEBUG_TYPE_ITEM;
                if(m_debug & NETLIZARD_DEBUG_RENDER_SCENE_BOUND)
                    m |= NETLIZARD_DEBUG_TYPE_SCENE;
                if(m)
                    NETLizard_DebugRenderGL3DMapModelScene_Bound(m_model, m_scenes, m_sceneCount, m);

                m = 0;
                if(m_debug & NETLIZARD_DEBUG_RENDER_ITEM_PLANE)
                    m |= NETLIZARD_DEBUG_TYPE_ITEM;
                if(m_debug & NETLIZARD_DEBUG_RENDER_SCENE_PLANE)
                    m |= NETLIZARD_DEBUG_TYPE_SCENE;
                if(m)
                    NETLizard_DebugRenderGL3DMapModelScene_Plane(m_model, m_scenes, m_sceneCount, m);

                if(m_debug & NETLIZARD_DEBUG_RENDER_MAP_BSP)
                    NETLizard_DebugRenderGL3DMapModelBSP(m_model);
            }
        }
        else
        {
            if(m_debug & NETLIZARD_DEBUG_RENDER_MAP_BOUND)
                NETLizard_DebugRenderGL3DMapModelBound(m_model);

            int m = 0;
            if(m_debug & NETLIZARD_DEBUG_RENDER_ITEM_VERTEX_NORMAL)
                m |= NETLIZARD_DEBUG_TYPE_ITEM;
            if(m_debug & NETLIZARD_DEBUG_RENDER_SCENE_VERTEX_NORMAL)
                m |= NETLIZARD_DEBUG_TYPE_SCENE;
            if(m)
                NETLizard_DebugRenderGL3DModel_VertexNormal(m_model, m);

            m = 0;
            if(m_debug & NETLIZARD_DEBUG_RENDER_ITEM_BOUND)
                m |= NETLIZARD_DEBUG_TYPE_ITEM;
            if(m_debug & NETLIZARD_DEBUG_RENDER_SCENE_BOUND)
                m |= NETLIZARD_DEBUG_TYPE_SCENE;
            if(m)
                NETLizard_DebugRenderGL3DModel_Bound(m_model, m);

            m = 0;
            if(m_debug & NETLIZARD_DEBUG_RENDER_ITEM_PLANE)
                m |= NETLIZARD_DEBUG_TYPE_ITEM;
            if(m_debug & NETLIZARD_DEBUG_RENDER_SCENE_PLANE)
                m |= NETLIZARD_DEBUG_TYPE_SCENE;
            if(m)
                NETLizard_DebugRenderGL3DModel_Plane(m_model, m);

            if(m_debug & NETLIZARD_DEBUG_RENDER_MAP_BSP)
                NETLizard_DebugRenderGL3DMapModelBSP(m_model);
        }
    }
    if(m_model->game != NL_RACING_EVOLUTION_3D)
        RenderHighlight();
    glPopMatrix();
}

void NETLizardMapModelDebugRenderer::Destroy()
{
    SetModel(0);
    NLRenderable::Destroy();
}

void NETLizardMapModelDebugRenderer::SetModel(GL_NETLizard_3D_Model *model)
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

void NETLizardMapModelDebugRenderer::SetCull(bool b)
{
    if(m_cull != b)
    {
        m_cull = b;
        SetupScenes(m_cull);
    }
}

void NETLizardMapModelDebugRenderer::SetupScenes(bool b)
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

void NETLizardMapModelDebugRenderer::SetSceneCount(int i)
{
    if(!m_cull && i > 0)
        return;
    if(m_sceneCount != i)
        m_sceneCount = i;
}

void NETLizardMapModelDebugRenderer::SetDebug(int i)
{
    if(m_debug != i)
    {
        m_debug = i;
        emit propertyChanged("renderDebug", m_debug);
    }
}

void NETLizardMapModelDebugRenderer::SetRenderScenes(const int scenes[], int count)
{
    if(!m_scenes)
        return;
    m_sceneCount = count;
    if(count > 0)
        memcpy(m_scenes, scenes, count * sizeof(int));
}

void NETLizardMapModelDebugRenderer::SetCamera(NLSceneCamera *camera)
{
    m_camera = camera;
}

void NETLizardMapModelDebugRenderer::RenderHighlight()
{
    if(!m_camera || !m_model)
        return;
    int collision_object = 0;
    if(m_debug & NETLIZARD_DEBUG_RENDER_HIGHLIGHT_VIEW_SCENE_PLANE)
        collision_object |= 1;
    if(m_debug & NETLIZARD_DEBUG_RENDER_HIGHLIGHT_VIEW_ITEM)
        collision_object |= 2;
    if(!collision_object)
        return;

    float distance = 0;
    int scene = -1;
    int collision_id = -1;
    int collision_type = 0;

    vector3_t direction = m_camera->Direction();
    vector3_t p = m_camera->Position();
    const NLMatrix4 *mat = m_camera->InitialFixedViewMatrix();
    matrix_transformv_self_row(mat, &p);

    nl_vector3_t cpoint = VECTOR3(0, 0, 0);
    int res = NETLizard_RayIntersect(m_model, &p, &direction, collision_object, &scene, &collision_id, &collision_type, &cpoint, &distance);
    if(res)
    {
        if(collision_type == 1)
            NETLizard_DebugHighlightRenderGL3DModelPlane(m_model, scene, collision_id);
        else
        {
            NETLizard_DebugHighlightRenderGL3DItemModelEdge(m_model, scene, collision_id, &p, NULL/*&direction*/);
        }

        //fprintf(stderr, "%d: scene %d, id %d, type %d, %f | %f %f %f\n", res, scene, collision_id, collision_type, distance, cpoint.v[0], cpoint.v[1], cpoint.v[2]);
    }
}

void NETLizardMapModelDebugRenderer::SetAllScenes()
{
    if(!m_scenes)
        return;
    for(GLuint i = 0; i < m_model->count; i++)
        m_scenes[i] = i;
    m_sceneCount = m_model->count;
}
