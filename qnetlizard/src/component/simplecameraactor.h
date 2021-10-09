#ifndef _KARIN_SIMPLECAMERAACTOR_H
#define _KARIN_SIMPLECAMERAACTOR_H

#include "nlactor.h"

#include "lib/vector3.h"

class SimpleCameraComponent;
class SimpleControlComponent;
class SceneCamera;

class SimpleCameraActor : public NLActor
{
    Q_OBJECT
public:
    explicit SimpleCameraActor(const NLPropperties &prop = QVariantHash(), NLActor *parent = 0);
    virtual ~SimpleCameraActor();
    virtual void Init();
    virtual void Destroy();
    virtual void Update(float delta);
    NLINTERFACE void SetPosition(const vector3_s *v);
    NLINTERFACE void SetRotation(const vector3_s *v);
    SceneCamera * Camera();
    
signals:
    
public slots:

private:
    SimpleCameraComponent *m_camera;
    SimpleControlComponent *m_control;

    Q_DISABLE_COPY(SimpleCameraActor)
};

#endif // _KARIN_SIMPLECAMERAACTOR_H
