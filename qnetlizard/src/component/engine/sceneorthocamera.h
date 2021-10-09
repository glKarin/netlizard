#ifndef _KARIN_SCENEORTHOCAMERA_H
#define _KARIN_SCENEORTHOCAMERA_H

#include "scenecamera.h"

class SceneOrthoCamera : public SceneCamera
{
public:
    SceneOrthoCamera(NLScene *scene = 0);
    virtual ~SceneOrthoCamera();
    virtual void Update(float width, float height);
    void SetLeft(float left);
    void SetRight(float right);
    void SetBottom(float bottom);
    void SetTop(float top);
    void SetZNear(float near);
    void SetZFar(float far);
    void Set2D();
    void Set(float left, float right, float bottom, float top, float near = -1, float far = 1);

protected:
    virtual void Projection();

private:
    float m_left;
    float m_right;
    float m_bottom;
    float m_top;
    float m_zNear;
    float m_zFar;
};

#endif // _KARIN_SCENEORTHOCAMERA_H
