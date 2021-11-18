#ifndef _KARIN_NLSCENECAMERA_H
#define _KARIN_NLSCENECAMERA_H

#include "nlsceneorthocamera.h"
#include "nlsceneperspectivecamera.h"

class NLSceneCamera : public NLScenePerspectiveCamera, public NLSceneOrthoCamera
{
public:
    NLSceneCamera(NLScene *scene = 0);
    NLSceneCamera(SceneCamera_Type type, NLScene *scene = 0);
    virtual ~NLSceneCamera();
    virtual void Update(float width, float height);
    virtual void SetZNear(float near);
    virtual void SetZFar(float far);
    virtual float ZNear() const;
    virtual float ZFar() const;
    virtual float ZDistance() const;
    void SetType(SceneCamera_Type type);
    SceneCamera_Type Type() const;
    virtual void Reset();

protected:
    virtual void Projection();
    virtual void UpdateProjectionMatrix(NLMatrix4 *mat);

private:
    SceneCamera_Type m_type;

    Q_DISABLE_COPY(NLSceneCamera)
};

#endif // _KARIN_NLSCENECAMERA_H
