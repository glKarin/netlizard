#ifndef _KARIN_NLSCENEORTHOCAMERA_H
#define _KARIN_NLSCENEORTHOCAMERA_H

#include <QtGlobal>

#include "nlscenecamerabase.h"

class NLSceneOrthoCamera : virtual public NLSceneCameraBase
{
public:
    NLSceneOrthoCamera(NLScene *scene = 0);
    virtual ~NLSceneOrthoCamera();
    virtual void Update(float width, float height);
    void SetAlignment(Qt::Alignment align);
    Qt::Alignment Alignment() const { return m_align; }
    void SetLeft(float left);
    void SetRight(float right);
    void SetBottom(float bottom);
    void SetTop(float top);
    virtual void SetZNear(float near);
    virtual void SetZFar(float far);
    void Setup2D();
    void Setup3D(float near, float far);
    void SetOrtho2D(float left, float right, float bottom, float top) { SetOrtho(left, right, bottom, top, -1, 1); }
    void SetOrtho(float left, float right, float bottom, float top, float near, float far);
    void SetOrtho(float left, float right, float bottom, float top) { SetOrtho(left, right, bottom, top, m_zNear, m_zFar); }
    float Left() const { return m_left; }
    float Right() const { return m_right; }
    float Bottom() const { return m_bottom; }
    float Top() const { return m_top; }
    virtual float ZNear() const { return m_zNear; }
    virtual float ZFar() const { return m_zFar; }
    float XDistance() const { return m_right - m_left; }
    float YDistance() const { return m_top - m_bottom; }
    virtual float ZDistance() const { return m_zFar - m_zNear; }
    virtual void Reset();

protected:
    virtual void UpdateProjectionMatrix(NLMatrix4 *mat);

private:
    void EmitPropertyChanged(int mask);

private:
    Qt::Alignment m_align;
    float m_left;
    float m_right;
    float m_bottom;
    float m_top;
    float m_zNear;
    float m_zFar;

    Q_DISABLE_COPY(NLSceneOrthoCamera)
};

#endif // _KARIN_NLSCENEORTHOCAMERA_H
