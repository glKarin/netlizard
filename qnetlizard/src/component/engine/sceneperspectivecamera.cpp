#include "sceneperspectivecamera.h"

#include <QDebug>

#include <GL/gl.h>
#include <GL/glu.h>

ScenePerspectiveCamera::ScenePerspectiveCamera(NLScene *scene)
    : SceneCamera(scene),
      m_fovy(45),
      m_aspect(1),
      m_zNear(0.01),
      m_zFar(999999)
{
}

ScenePerspectiveCamera::~ScenePerspectiveCamera()
{

}

void ScenePerspectiveCamera::Update(float width, float height)
{
    SetWidthAndHeight(width, height);
}

void ScenePerspectiveCamera::SetFovy(float fovy)
{
    if(m_fovy != fovy)
    {
        m_fovy = fovy;
    }
}

void ScenePerspectiveCamera::SetWidthAndHeight(float width, float height)
{
    float h = height == 0 ? 1 : height;
    float a = width / h;
    if(m_aspect != a)
    {
        m_aspect = a;
    }
}

void ScenePerspectiveCamera::SetAspect(float aspect)
{
    if(m_aspect != aspect)
    {
        m_aspect = aspect;
    }
}

void ScenePerspectiveCamera::SetZNear(float near)
{
    if(m_zNear != near)
    {
        m_zNear = near;
    }
}

void ScenePerspectiveCamera::SetZFar(float far)
{
    if(m_zFar != far)
    {
        m_zFar = far;
    }
}

void ScenePerspectiveCamera::Set(float fovy, float width, float height, float near, float far)
{
    SetFovy(fovy);
    SetWidthAndHeight(width, height);
    SetZNear(near);
    SetZFar(far);
}

void ScenePerspectiveCamera::Set(float fovy, float aspect, float near, float far)
{
    SetFovy(fovy);
    SetAspect(aspect);
    SetZNear(near);
    SetZFar(far);
}

void ScenePerspectiveCamera::Projection()
{
    gluPerspective(m_fovy, m_aspect, m_zNear, m_zFar);
    //th = h != 0.0 ? h : 1;
    //Mesa_glLoadIdentity(m_matrix + ProjectionMatrix);
    //Mesa_gluPerspective(m_matrix + ProjectionMatrix, fovy, w / th, n, f);
    //glMatrixMode(GL_PROJECTION);
    //glLoadMatrixf(m_matrix[ProjectionMatrix].m);
}
