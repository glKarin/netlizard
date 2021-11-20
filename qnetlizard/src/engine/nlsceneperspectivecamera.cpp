#include "nlsceneperspectivecamera.h"

#include <QDebug>

#include <GL/gl.h>
#include <GL/glu.h>

#include "nlfuncs.h"

#define NLSCENEPERSPECTIVECAMERA_DEFAULT_FOVY 45
#define NLSCENEPERSPECTIVECAMERA_DEFAULT_ASPECT 1
#define NLSCENEPERSPECTIVECAMERA_DEFAULT_Z_NEAR 0.01
#define NLSCENEPERSPECTIVECAMERA_DEFAULT_Z_FAR 99999

NLScenePerspectiveCamera::NLScenePerspectiveCamera(NLScene *scene)
    : NLSceneCameraBase(scene),
      m_fovy(NLSCENEPERSPECTIVECAMERA_DEFAULT_FOVY),
      m_aspect(NLSCENEPERSPECTIVECAMERA_DEFAULT_ASPECT),
      m_zNear(NLSCENEPERSPECTIVECAMERA_DEFAULT_Z_NEAR),
      m_zFar(NLSCENEPERSPECTIVECAMERA_DEFAULT_Z_FAR)
{
}

NLScenePerspectiveCamera::~NLScenePerspectiveCamera()
{

}

#define FOVY_P 0.1
void NLScenePerspectiveCamera::SetFovy(float fovy)
{
    float f = NL::clamp<float>(fovy, 0.0 + FOVY_P, 180.0 - FOVY_P);
    if(m_fovy != f)
    {
        m_fovy = f;
        UpdateMatrix();
    }
}
#undef FOVY_P

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
    //gluPerspective(m_fovy, m_aspect, m_zNear, m_zFar);
    //glMultMatrixf(GL_MATRIXV_M(ProjectionMatrix()));
    NLSceneCameraBase::Projection();
}

void NLScenePerspectiveCamera::UpdateProjectionMatrix(NLMatrix4 *mat)
{
    Mesa_glLoadIdentity(mat);
    Mesa_gluPerspective(mat, m_fovy, m_aspect, m_zNear, m_zFar);
}

void NLScenePerspectiveCamera::Reset()
{
    NLSceneCameraBase::Reset();
    m_fovy = NLSCENEPERSPECTIVECAMERA_DEFAULT_FOVY;
    m_aspect = NLSCENEPERSPECTIVECAMERA_DEFAULT_ASPECT;
    m_zNear = NLSCENEPERSPECTIVECAMERA_DEFAULT_Z_NEAR;
    m_zFar = NLSCENEPERSPECTIVECAMERA_DEFAULT_Z_FAR;
    UpdateMatrix();
}
