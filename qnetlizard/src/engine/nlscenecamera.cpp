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

void NLSceneCamera::SetType(SceneCamera_Type type)
{
    if(m_type != type)
    {
        float near = ZNear();
        float far = ZFar();
        m_type = type;
        Reset();
        float nnear = ZNear();
        float nfar = ZFar();
        if(near != nnear)
            PropertyChanged("zNear", nnear);
        if(far != nfar)
            PropertyChanged("zFar", nfar);
    }
}
