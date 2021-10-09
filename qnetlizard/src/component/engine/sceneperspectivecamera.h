#ifndef _KARIN_SCENEPERSPECTIVECAMERA_H
#define _KARIN_SCENEPERSPECTIVECAMERA_H

#include "scenecamera.h"

class ScenePerspectiveCamera : public SceneCamera
{
public:
    ScenePerspectiveCamera(NLScene *scene = 0);
    virtual ~ScenePerspectiveCamera();
    virtual void Update(float width, float height);
    void SetFovy(float fovy);
    void SetWidthAndHeight(float width, float height);
    void SetAspect(float aspect);
    void SetZNear(float near);
    void SetZFar(float far);
    void Set(float fovy, float width, float height, float near, float far);
    void Set(float fovy, float aspect, float near, float far);

protected:
    virtual void Projection();

private:
    float m_fovy;
    float m_aspect;
    //float m_width;
    //float m_height;
    float m_zNear;
    float m_zFar;
};

#endif // _KARIN_SCENEPERSPECTIVECAMERA_H
