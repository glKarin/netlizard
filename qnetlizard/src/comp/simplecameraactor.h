#ifndef _KARIN_SIMPLECAMERAACTOR_H
#define _KARIN_SIMPLECAMERAACTOR_H

#include "nlactor.h"

class SimpleCameraComponent;
class SimpleControlComponent;
class NLSceneCamera;
class NLComponent;

NLACTOR(SimpleCameraActor)
class SimpleCameraActor : public NLActor
{
    Q_OBJECT
public:
    explicit SimpleCameraActor(const NLPropperties &prop = QVariantHash(), NLActor *parent = 0);
    virtual ~SimpleCameraActor();
    NLSceneCamera * Camera();
    NLINTERFACE void SetEnableControl(bool b);

protected:
    virtual void Init();
    virtual void Destroy();
    virtual void Update(float delta);
    
signals:
    
public slots:

private Q_SLOTS:
    void OnFovyChanged(float f);

private:
    SimpleCameraComponent *m_camera;
    NLComponent *m_control;

    Q_DISABLE_COPY(SimpleCameraActor)
};

#endif // _KARIN_SIMPLECAMERAACTOR_H
