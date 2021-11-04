#ifndef _KARIN_SIMPLECAMERAACTOR_H
#define _KARIN_SIMPLECAMERAACTOR_H

#include "nlactor.h"
#include "nlrigidbody.h"

class SimpleCameraComponent;
class SimpleControlComponent;
class NLSceneCamera;
class NLComponent;

NLACTOR(SimpleCameraActor)
class SimpleCameraActor : public NLRigidbody
{
    Q_OBJECT
public:
    explicit SimpleCameraActor(const NLProperties &prop = NLProperties(), NLActor *parent = 0);
    virtual ~SimpleCameraActor();
    NLSceneCamera * Camera();
    NLComponent * Control();
    NLINTERFACE void SetEnableControl(bool b);

protected:
    virtual void Init();
    virtual void Destroy();
    virtual void Update(float delta);
    
signals:
    
public slots:
    void UpdateCamera();

private Q_SLOTS:
    void OnFovyChanged(float f);

private:
    SimpleCameraComponent *m_camera;
    NLComponent *m_control;

    Q_DISABLE_COPY(SimpleCameraActor)
};

#endif // _KARIN_SIMPLECAMERAACTOR_H
