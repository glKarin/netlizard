#ifndef _KARIN_NLSCENEORTHOCAMERA_H
#define _KARIN_NLSCENEORTHOCAMERA_H

#include <Qt>

#include "nlscenecamera.h"

class NLSceneOrthoCamera : public NLSceneCamera
{
public:
    NLSceneOrthoCamera(NLScene *scene = 0);
    virtual ~NLSceneOrthoCamera();
    virtual void Update(float width, float height);
    void SetAlignment(Qt::Alignment align);
    Qt::Alignment Alignment() const;
    void SetLeft(float left);
    void SetRight(float right);
    void SetBottom(float bottom);
    void SetTop(float top);
    void SetZNear(float near);
    void SetZFar(float far);
    void Set2D();
    void Set(float left, float right, float bottom, float top, float near = -1, float far = 1);
    float Left() const;
    float Right() const;
    float Bottom() const;
    float Top() const;
    float ZNear() const;
    float ZFar() const;
    float XDistance() const;
    float YDistance() const;
    float ZDistance() const;

protected:
    virtual void Projection();
    virtual void UpdateProjectionMatrix(NLMatrix4 *mat);

private:
    Qt::Alignment m_align;
    float m_left;
    float m_right;
    float m_bottom;
    float m_top;
    float m_zNear;
    float m_zFar;
};

#endif // _KARIN_NLSCENEORTHOCAMERA_H
