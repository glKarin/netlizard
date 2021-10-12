#ifndef _KARIN_SIMPLECAMERAACTOR_H
#define _KARIN_SIMPLECAMERAACTOR_H

#include "nlactor.h"

#include "lib/vector3.h"

class SimpleCameraComponent;
class SimpleControlComponent;
class NLSceneCamera;
class NLComponent;

class SimpleCameraActor : public NLActor
{
    Q_OBJECT
public:
    explicit SimpleCameraActor(const NLPropperties &prop = QVariantHash(), NLActor *parent = 0);
    virtual ~SimpleCameraActor();
    virtual void Init();
    virtual void Destroy();
    virtual void Update(float delta);
    NLSceneCamera * Camera();
    
signals:
    
public slots:

private:
    SimpleCameraComponent *m_camera;
    NLComponent *m_control;

    Q_DISABLE_COPY(SimpleCameraActor)
};

#endif // _KARIN_SIMPLECAMERAACTOR_H
