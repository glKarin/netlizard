#include "nlsceneperspectivecamera.h"

#include <QDebug>

#include <GL/gl.h>
#include <GL/glu.h>

#include "nlfuncs.h"

#define NLSCENEPERSPECTIVECAMERA_DEFAULT_FOVY 45
#define NLSCENEPERSPECTIVECAMERA_DEFAULT_ASPECT 1
#define NLSCENEPERSPECTIVECAMERA_DEFAULT_Z_NEAR 1 // 0.01
#define NLSCENEPERSPECTIVECAMERA_DEFAULT_Z_FAR 99999

#define PERSPECTIVE_FOVY 1
#define PERSPECTIVE_ASPECT (1 << 1)
#define PERSPECTIVE_ZNEAR (1 << 2)
#define PERSPECTIVE_ZFAR (1 << 3)

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
        PropertyChanged("fovy", m_fovy);
    }
}
#undef FOVY_P

void NLScenePerspectiveCamera::SetAspect(float aspect)
{
    if(m_aspect != aspect)
    {
        m_aspect = aspect;
        UpdateMatrix();
        PropertyChanged("aspect", m_aspect);
    }
}

void NLScenePerspectiveCamera::SetZNear(float near)
{
    if(m_zNear != near)
    {
        m_zNear = near;
        UpdateMatrix();
        PropertyChanged("zNear", m_zNear);
    }
}

void NLScenePerspectiveCamera::SetZFar(float far)
{
    if(m_zFar != far)
    {
        m_zFar = far;
        UpdateMatrix();
        PropertyChanged("zFar", m_zFar);
    }
}

void NLScenePerspectiveCamera::Set(float fovy, float width, float height, float near, float far)
{
    float aspect = CaleAspect(width, height);
    int b = 0;
    if(m_fovy != fovy)
    {
        m_fovy = fovy;
        b |= PERSPECTIVE_FOVY;
    }
    if(m_aspect != aspect)
    {
        m_aspect = aspect;
        b |= PERSPECTIVE_ASPECT;
    }
    if(m_zNear != near)
    {
        m_zNear = near;
        b |= PERSPECTIVE_ZNEAR;
    }
    if(m_zFar != far)
    {
        m_zFar = far;
        b |= PERSPECTIVE_ZFAR;
    }
    if(b)
    {
        UpdateMatrix();
        EmitPropertyChanged(b);
    }
}

void NLScenePerspectiveCamera::Set(float fovy, float aspect, float near, float far)
{
    int b = 0;
    if(m_fovy != fovy)
    {
        m_fovy = fovy;
        b |= PERSPECTIVE_FOVY;
    }
    if(m_aspect != aspect)
    {
        m_aspect = aspect;
        b |= PERSPECTIVE_ASPECT;
    }
    if(m_zNear != near)
    {
        m_zNear = near;
        b |= PERSPECTIVE_ZNEAR;
    }
    if(m_zFar != far)
    {
        m_zFar = far;
        b |= PERSPECTIVE_ZFAR;
    }
    if(b)
    {
        UpdateMatrix();
        EmitPropertyChanged(b);
    }
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
    Set(NLSCENEPERSPECTIVECAMERA_DEFAULT_FOVY, NLSCENEPERSPECTIVECAMERA_DEFAULT_ASPECT, NLSCENEPERSPECTIVECAMERA_DEFAULT_Z_NEAR, NLSCENEPERSPECTIVECAMERA_DEFAULT_Z_FAR);
}

void NLScenePerspectiveCamera::EmitPropertyChanged(int b)
{
    if(b & PERSPECTIVE_FOVY)
        PropertyChanged("fovy", m_fovy);
    if(b & PERSPECTIVE_ASPECT)
        PropertyChanged("aspect", m_aspect);
    if(b & PERSPECTIVE_ZNEAR)
        PropertyChanged("zNear", m_zNear);
    if(b & PERSPECTIVE_ZFAR)
        PropertyChanged("zFar", m_zFar);
}
