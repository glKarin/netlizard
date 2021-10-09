#include "scenecamera.h"

#include <GL/gl.h>

#include "nlscene.h"

SceneCamera::SceneCamera(NLScene *widget)
    : m_scene(widget)
{
    newcam(&m_cam);
#if 0
    for(int i = 0; i < TotalMatrix; i++)
    {
        Mesa_AllocGLMatrix(m_matrix + i);
    }
#endif
}

SceneCamera::~SceneCamera()
{
    m_scene = 0;
#if 0
    for(int i = 0; i < TotalMatrix; i++)
    {
        Mesa_FreeGLMatrix(m_matrix + i);
    }
    Mesa_glLoadIdentity(m_matrix + ModelviewMatrix);
    Mesa_glLoadMatrix(m_matrix + ModelviewMatrix, m_matrix[ViewMatrix].m);

    if(mat)
    {
        Mesa_glMultMatrix(m_matrix + ModelviewMatrix, mat->m);
    }

    Mesa_glLoadMatrix(m_matrix + ModelviewProjectionMatrix, m_matrix[ProjectionMatrix].m);
    Mesa_glMultMatrix(m_matrix + ModelviewProjectionMatrix, m_matrix[ModelviewMatrix].m);
#endif
}

void SceneCamera::ModelView()
{
    glRotatef(VECTOR3_X(m_cam.rotation), 1, 0, 0);
    glRotatef(VECTOR3_Y(m_cam.rotation), 0, 1, 0);
    glTranslatef(
                -VECTOR3_X(m_cam.position),
                -VECTOR3_Y(m_cam.position),
                -VECTOR3_Z(m_cam.position)
                );
    glRotatef(-90, 1, 0, 0);
}

void SceneCamera::Render()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    Projection();

    glMatrixMode(GL_MODELVIEW);
    ModelView();

    m_scene->Render();
}

NLScene * SceneCamera::Scene()
{
    return m_scene;
}

void SceneCamera::SetScene(NLScene *scene)
{
    m_scene = scene;
}

void SceneCamera::Move(const vector3_s *v)
{
    cammove(&m_cam, v);
}

void SceneCamera::Turn(const vector3_s *v)
{
    camrot(&m_cam, v);
}

void SceneCamera::Zoom(const vector3_s *v)
{
    camscale(&m_cam, v);
}

void SceneCamera::Reset()
{
    newcam(&m_cam);
}

void SceneCamera::SetPosition(const vector3_s *v)
{
    initcam(&m_cam, v, 0);
}

void SceneCamera::SetRotation(const vector3_s *v)
{
    initcam(&m_cam, 0, v);
}
