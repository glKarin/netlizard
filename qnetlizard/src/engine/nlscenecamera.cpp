#include "nlscenecamera.h"

#include <QDebug>

#include <GL/gl.h>
#include <GL/glu.h>

NLSceneCamera::NLSceneCamera(NLScene *scene)
    : NLScenePerspectiveCamera(scene),
      NLSceneOrthoCamera(scene),
      m_type(NLSceneCamera::Type_Perspective)
{
}

NLSceneCamera::NLSceneCamera(NLSceneCameraBase::SceneCamera_Type type, NLScene *scene)
    : NLScenePerspectiveCamera(scene),
      NLSceneOrthoCamera(scene),
      m_type(type)
{

}

NLSceneCamera::~NLSceneCamera()
{

}

void NLSceneCamera::SetZNear(float near)
{
    if(m_type == NLSceneCamera::Type_Ortho)
        NLSceneOrthoCamera::SetZNear(near);
    else
        NLScenePerspectiveCamera::SetZNear(near);
}

void NLSceneCamera::SetZFar(float far)
{
    if(m_type == NLSceneCamera::Type_Ortho)
        NLSceneOrthoCamera::SetZFar(far);
    else
        NLScenePerspectiveCamera::SetZFar(far);
}

float NLSceneCamera::ZNear() const
{
    if(m_type == NLSceneCamera::Type_Ortho)
        return NLSceneOrthoCamera::ZNear();
    else
        return NLScenePerspectiveCamera::ZNear();
}

float NLSceneCamera::ZFar() const
{
    if(m_type == NLSceneCamera::Type_Ortho)
        return NLSceneOrthoCamera::ZFar();
    else
        return NLScenePerspectiveCamera::ZFar();
}

float NLSceneCamera::ZDistance() const
{
    if(m_type == NLSceneCamera::Type_Ortho)
        return NLSceneOrthoCamera::ZDistance();
    else
        return NLScenePerspectiveCamera::ZDistance();
}

void NLSceneCamera::Update(float width, float height)
{
    if(m_type == NLSceneCamera::Type_Ortho)
        return NLSceneOrthoCamera::Update(width, height);
    else
        return NLScenePerspectiveCamera::Update(width, height);
}

void NLSceneCamera::Projection()
{
    if(m_type == NLSceneCamera::Type_Ortho)
        return NLSceneOrthoCamera::Projection();
    else
        return NLScenePerspectiveCamera::Projection();
}

void NLSceneCamera::UpdateProjectionMatrix(NLMatrix4 *mat)
{
    if(m_type == NLSceneCamera::Type_Ortho)
        return NLSceneOrthoCamera::UpdateProjectionMatrix(mat);
    else
        return NLScenePerspectiveCamera::UpdateProjectionMatrix(mat);
}

void NLSceneCamera::SetType(SceneCamera_Type type)
{
    if(m_type != type)
    {
        m_type = type;
        Reset();
    }
}

NLSceneCamera::SceneCamera_Type NLSceneCamera::Type() const
{
    return m_type;
}


void NLSceneCamera::Reset()
{
    if(m_type == NLSceneCamera::Type_Ortho)
        NLSceneOrthoCamera::Reset();
    else
        NLScenePerspectiveCamera::Reset();
}
