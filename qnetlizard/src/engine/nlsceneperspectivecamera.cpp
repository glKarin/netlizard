#include "nlsceneperspectivecamera.h"

#include <QDebug>

#include <GL/gl.h>
#include <GL/glu.h>

NLScenePerspectiveCamera::NLScenePerspectiveCamera(NLScene *scene)
    : NLSceneCamera(scene),
      m_fovy(45),
      m_aspect(1),
      m_zNear(0.01),
      m_zFar(999999)
{
}

NLScenePerspectiveCamera::~NLScenePerspectiveCamera()
{

}

void NLScenePerspectiveCamera::Update(float width, float height)
{
    SetWidthAndHeight(width, height);
}

void NLScenePerspectiveCamera::SetFovy(float fovy)
{
    if(m_fovy != fovy)
    {
        m_fovy = fovy;
        UpdateMatrix();
    }
}

float NLScenePerspectiveCamera::CaleAspect(float width, float height)
{
    float h = height == 0 ? 1 : height;
    float a = width / h;
    return a;
}

void NLScenePerspectiveCamera::SetWidthAndHeight(float width, float height)
{
    SetAspect(CaleAspect(width, height));
}

void NLScenePerspectiveCamera::SetAspect(float aspect)
{
    if(m_aspect != aspect)
    {
        m_aspect = aspect;
        UpdateMatrix();
    }
}

void NLScenePerspectiveCamera::SetZNear(float near)
{
    if(m_zNear != near)
    {
        m_zNear = near;
        UpdateMatrix();
    }
}

void NLScenePerspectiveCamera::SetZFar(float far)
{
    if(m_zFar != far)
    {
        m_zFar = far;
        UpdateMatrix();
    }
}

void NLScenePerspectiveCamera::Set(float fovy, float width, float height, float near, float far)
{
    float aspect = CaleAspect(width, height);
    bool b = false;
    if(m_fovy != fovy)
    {
        m_fovy = fovy;
        b = true;
    }
    if(m_aspect != aspect)
    {
        m_aspect = aspect;
        b = true;
    }
    if(m_zNear != near)
    {
        m_zNear = near;
        b = true;
    }
    if(m_zFar != far)
    {
        m_zFar = far;
        b = true;
    }
    if(b)
        UpdateMatrix();
}

void NLScenePerspectiveCamera::Set(float fovy, float aspect, float near, float far)
{
    bool b = false;
    if(m_fovy != fovy)
    {
        m_fovy = fovy;
        b = true;
    }
    if(m_aspect != aspect)
    {
        m_aspect = aspect;
        b = true;
    }
    if(m_zNear != near)
    {
        m_zNear = near;
        b = true;
    }
    if(m_zFar != far)
    {
        m_zFar = far;
        b = true;
    }
    if(b)
        UpdateMatrix();
}

void NLScenePerspectiveCamera::Projection()
{
    gluPerspective(m_fovy, m_aspect, m_zNear, m_zFar);
}

void NLScenePerspectiveCamera::UpdateProjectionMatrix(NLMatrix4 *mat)
{
    Mesa_glLoadIdentity(mat);
    Mesa_gluPerspective(mat, m_fovy, m_aspect, m_zNear, m_zFar);
}
