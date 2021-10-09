#include "sceneorthocamera.h"

#include <GL/gl.h>
#include <GL/glu.h>

SceneOrthoCamera::SceneOrthoCamera(NLScene *scene)
    : SceneCamera(scene),
      m_left(-1),
      m_right(1),
      m_bottom(-1),
      m_top(1),
      m_zNear(-1),
      m_zFar(1)
{
}

SceneOrthoCamera::~SceneOrthoCamera()
{

}

void SceneOrthoCamera::Update(float width, float height)
{
    SetRight(width);
    SetTop(height);
}

void SceneOrthoCamera::SetLeft(float left)
{
    if(m_left != left)
    {
        m_left = left;
    }
}

void SceneOrthoCamera::SetRight(float right)
{
    if(m_right != right)
    {
        m_right = right;
    }
}

void SceneOrthoCamera::SetTop(float top)
{
    if(m_top != top)
    {
        m_top = top;
    }
}

void SceneOrthoCamera::SetBottom(float bottom)
{
    if(m_bottom != bottom)
    {
        m_bottom = bottom;
    }
}

void SceneOrthoCamera::SetZNear(float near)
{
    if(m_zNear != near)
    {
        m_zNear = near;
    }
}

void SceneOrthoCamera::SetZFar(float far)
{
    if(m_zFar != far)
    {
        m_zFar = far;
    }
}

void SceneOrthoCamera::Set2D()
{
    SetZNear(-1);
    SetZFar(1);
}

void SceneOrthoCamera::Set(float left, float right, float bottom, float top, float near, float far)
{
    SetLeft(left);
    SetRight(right);
    SetBottom(bottom);
    SetTop(top);
    SetZNear(near);
    SetZFar(far);
}

void SceneOrthoCamera::Projection()
{
    glOrtho(m_left, m_right, m_bottom, m_top, m_zNear, m_zFar);
    //Mesa_glLoadIdentity(m_matrix + ProjectionMatrix);
    //Mesa_gluOrtho2D(m_matrix + ProjectionMatrix, left, right, bottom, top);
    //glMatrixMode(GL_PROJECTION);
    //glLoadMatrixf(m_matrix[ProjectionMatrix].m);
}
