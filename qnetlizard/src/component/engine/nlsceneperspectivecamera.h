#ifndef NLSCENEPERSPECTIVECAMERA_H
#define NLSCENEPERSPECTIVECAMERA_H

#include "nlscenecamera.h"

class NLScenePerspectiveCamera : public NLSceneCamera
{
public:
    NLScenePerspectiveCamera(NLScene *scene = 0);
    virtual ~NLScenePerspectiveCamera();
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
    virtual void UpdateProjectionMatrix(NLMatrix4 *mat);

private:
    float CaleAspect(float width, float height);

private:
    float m_fovy;
    float m_aspect;
    //float m_width;
    //float m_height;
    float m_zNear;
    float m_zFar;
};

#endif // NLSCENEPERSPECTIVECAMERA_H
