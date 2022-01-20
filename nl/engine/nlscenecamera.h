#ifndef _KARIN_NLSCENECAMERA_H
#define _KARIN_NLSCENECAMERA_H

#include "nlsceneorthocamera.h"
#include "nlsceneperspectivecamera.h"

class NLLIB_EXPORT NLSceneCamera : public NLScenePerspectiveCamera, public NLSceneOrthoCamera
{
public:
    NLSceneCamera(NLScene *scene = 0);
    NLSceneCamera(SceneCamera_Type type, NLScene *scene = 0);
    virtual ~NLSceneCamera();
    virtual void Update(float width, float height) {
        if(m_type == NLSceneCamera::Type_Ortho)
            NLSceneOrthoCamera::Update(width, height);
        else
            NLScenePerspectiveCamera::Update(width, height);
    }
    void SetZNear(float znear) {
        if(m_type == NLSceneCamera::Type_Ortho)
            NLSceneOrthoCamera::SetZNear(znear);
        else
            NLScenePerspectiveCamera::SetZNear(znear);
    }
    void SetZFar(float zfar) {
        if(m_type == NLSceneCamera::Type_Ortho)
            NLSceneOrthoCamera::SetZFar(zfar);
        else
            NLScenePerspectiveCamera::SetZFar(zfar);
    }
    float ZNear() const {
        return m_type == NLSceneCamera::Type_Ortho ? NLSceneOrthoCamera::ZNear() : NLScenePerspectiveCamera::ZNear();
    }
    float ZFar() const {
        return m_type == NLSceneCamera::Type_Ortho ? NLSceneOrthoCamera::ZFar() : NLScenePerspectiveCamera::ZFar();
    }
    float ZDistance() const {
        return m_type == NLSceneCamera::Type_Ortho ? NLSceneOrthoCamera::ZDistance() : NLScenePerspectiveCamera::ZDistance();
    }
    void SetType(SceneCamera_Type type);
    SceneCamera_Type Type() const { return m_type; }
    void Reset() {
        if(m_type == NLSceneCamera::Type_Ortho)
            NLSceneOrthoCamera::Reset();
        else
            NLScenePerspectiveCamera::Reset();
    }

protected:
    virtual void UpdateProjectionMatrix(NLMatrix4 *mat) {
        if(m_type == NLSceneCamera::Type_Ortho)
            NLSceneOrthoCamera::UpdateProjectionMatrix(mat);
        else
            NLScenePerspectiveCamera::UpdateProjectionMatrix(mat);
    }

private:
    SceneCamera_Type m_type;

    Q_DISABLE_COPY(NLSceneCamera)
};

Q_DECLARE_METATYPE(NLSceneCamera*)

#endif // _KARIN_NLSCENECAMERA_H
