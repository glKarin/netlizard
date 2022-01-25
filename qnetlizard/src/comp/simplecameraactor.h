#ifndef _KARIN_SIMPLECAMERAACTOR_H
#define _KARIN_SIMPLECAMERAACTOR_H

#include "engine/nlrigidbody.h"

class SimpleCameraComponent;
class SimpleControlComponent;
class NLSceneCamera;
class NLComponent;

NLACTOR(SimpleCameraActor)
class SimpleCameraActor : public NLRigidbody
{
    Q_OBJECT
    Q_PROPERTY(NLSceneCamera* camera READ Camera FINAL)
    Q_PROPERTY(NLComponent* control READ Control FINAL)
    Q_PROPERTY(NLComponent* cameraComponent READ CameraComponent FINAL)
public:
    explicit SimpleCameraActor(const NLProperties &prop = NLProperties(), NLActor *parent = 0);
    virtual ~SimpleCameraActor();
    NLSceneCamera * Camera();
    NLComponent * CameraComponent() { return m_camera; }
    NLComponent * Control() { return m_control; }
    NLINTERFACE void SetEnableControl(bool b);
    void SetEnabled(bool enabled);

protected:
    virtual void Init();
    
signals:
    
public slots:
    void UpdateCamera();

private Q_SLOTS:
    void OnFovyChanged(float f);

private:
    NLComponent *m_camera;
    NLComponent *m_control;

    Q_DISABLE_COPY(SimpleCameraActor)
};

#endif // _KARIN_SIMPLECAMERAACTOR_H
