#include "nlscenecamera.h"

#include <QDebug>

#include <GL/gl.h>
#include <GL/glu.h>

#include "nlscene.h"

NLSceneCamera::NLSceneCamera(NLScene *scene)
    : NLSceneCameraBase(scene),
      NLScenePerspectiveCamera(scene),
      NLSceneOrthoCamera(scene),
      m_type(NLSceneCamera::Type_Perspective)
{
}

NLSceneCamera::NLSceneCamera(NLSceneCameraBase::SceneCamera_Type type, NLScene *scene)
    : NLSceneCameraBase(scene),
      NLScenePerspectiveCamera(scene),
      NLSceneOrthoCamera(scene),
      m_type(type)
{
}

NLSceneCamera::~NLSceneCamera()
{
    NLScene *scene = Scene();
    if(scene && scene->CurrentCamera() == this)
    {
        scene->SetCurrentCamera(0);
    }
}

void NLSceneCamera::SetType(SceneCamera_Type type)
{
    if(m_type != type)
    {
        float onear = ZNear();
        float ofar = ZFar();
        m_type = type;
        Reset();
        float nnear = ZNear();
        float nfar = ZFar();
        if(onear != nnear)
            PropertyChanged("zNear", nnear);
        if(ofar != nfar)
            PropertyChanged("zFar", nfar);
    }
}
