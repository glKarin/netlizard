#ifndef _KARIN_NLSCENEPERSPECTIVECAMERA_H
#define _KARIN_NLSCENEPERSPECTIVECAMERA_H

#include "nlscenecamerabase.h"

class NLScenePerspectiveCamera : virtual public NLSceneCameraBase
{
public:
    NLScenePerspectiveCamera(NLScene *scene = 0);
    virtual ~NLScenePerspectiveCamera();
    virtual void Update(float width, float height) { SetWidthAndHeight(width, height); }
    void SetFovy(float fovy);
    void SetAspect(float aspect);
    void SetWidthAndHeight(float width, float height) { SetAspect(CaleAspect(width, height)); }
    virtual void SetZNear(float near);
    virtual void SetZFar(float far);
    void Set(float fovy, float width, float height, float near, float far);
    void Set(float fovy, float aspect, float near, float far);
    float Fovy() const { return m_fovy; }
    float Aspect() const { return m_aspect; }
    virtual float ZNear() const { return m_zNear; }
    virtual float ZFar() const { return m_zFar; }
    virtual float ZDistance() const { return m_zFar - m_zNear; }
    void ResetFovy() { SetFovy(45); }
    virtual void Reset();

protected:
    virtual void Projection();
    virtual void UpdateProjectionMatrix(NLMatrix4 *mat);

private:
    float CaleAspect(float width, float height) {
        float h = height == 0 ? 1 : height;
        float a = width / h;
        return a;
    }

private:
    void EmitPropertyChanged(int mask);

private:
    float m_fovy;
    float m_aspect;
    //float m_width;
    //float m_height;
    float m_zNear;
    float m_zFar;

    Q_DISABLE_COPY(NLScenePerspectiveCamera)
};

#endif // _KARIN_NLSCENEPERSPECTIVECAMERA_H
