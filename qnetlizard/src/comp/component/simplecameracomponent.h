#ifndef _KARIN_SIMPLECAMERACOMPONENT_H
#define _KARIN_SIMPLECAMERACOMPONENT_H

#include "nlcomponent.h"

#include "nlactor.h"

class NLSceneCamera;
class CameraNotifyFunc;

NLCOMPONENT(SimpleCameraComponent)
class SimpleCameraComponent : public NLComponent
{
    Q_OBJECT
    Q_PROPERTY(int type READ Type WRITE SetType FINAL)
    Q_PROPERTY(bool render READ IsRender WRITE SetRender FINAL)
    Q_PROPERTY(float fovy READ Fovy WRITE SetFovy FINAL)
    Q_PROPERTY(float aspect READ Aspect WRITE SetAspect FINAL)
    Q_PROPERTY(float zNear READ ZNear WRITE SetZNear FINAL)
    Q_PROPERTY(float zFar READ ZFar WRITE SetZFar FINAL)
    Q_PROPERTY(float left READ Left WRITE SetLeft FINAL)
    Q_PROPERTY(float right READ Right WRITE SetRight FINAL)
    Q_PROPERTY(float bottom READ Bottom WRITE SetBottom FINAL)
    Q_PROPERTY(float top READ Top WRITE SetTop FINAL)
    Q_PROPERTY(int alignment READ Alignment WRITE SetAlignment FINAL)
    Q_PROPERTY(bool zIsUp READ ZIsUp WRITE SetZIsUp FINAL)
public:
    explicit SimpleCameraComponent(const NLProperties &prop = NLProperties(), NLActor *parent = 0);
    virtual ~SimpleCameraComponent();
    virtual void Reset();
    NLINTERFACE void Render();
    NLGETTER(type) int Type() const { return m_type; }
    NLSETTER(type) void SetType(int type);
    Q_INVOKABLE NLSceneCamera * Camera() { return m_camera; }
    void SetScene(NLScene *scene);
    void SetEnabled(bool enabled);
    bool IsEnabled() const;
    void SetRender(bool enabled);
    bool IsRender() const;

    float Left() const;
    float Right() const;
    float Bottom() const;
    float Top() const;
    float ZNear() const;
    float ZFar() const;
    float Fovy() const;
    float Aspect() const;
    int Alignment() const;
    bool ZIsUp() const;

    void SetLeft(float left);
    void SetRight(float right);
    void SetBottom(float bottom);
    void SetTop(float top);
    void SetZNear(float near);
    void SetZFar(float far);
    void SetFovy(float fovy);
    void SetAspect(float aspect);
    void SetAlignment(int align);
    void SetZIsUp(bool b);
    
signals:
    
public slots:
    void UpdateCamera();

protected:
    virtual void Init();
    virtual void Destroy();
    virtual void Update(float delta);
    virtual void InitProperty();

private Q_SLOTS:
    void OnPositionChanged(const NLVector3 &pos);
    void OnRotationChanged(const NLVector3 &rot);

private:
    NLPROPERTY(int, type) int m_type;
    NLSceneCamera *m_camera;
    void *m_cameraNotifyFunc;

    friend class CameraNotifyFunc;
    Q_DISABLE_COPY(SimpleCameraComponent)
};

#endif // _KARIN_SIMPLECAMERACOMPONENT_H
