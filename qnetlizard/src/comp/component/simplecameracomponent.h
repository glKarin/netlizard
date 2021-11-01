#ifndef _KARIN_SIMPLECAMERACOMPONENT_H
#define _KARIN_SIMPLECAMERACOMPONENT_H

#include "nlcomponent.h"

#include "nlactor.h"

class NLSceneCamera;

NLCOMPONENT(SimpleCameraComponent)
class SimpleCameraComponent : public NLComponent
{
    Q_OBJECT
public:
    explicit SimpleCameraComponent(const NLPropperties &prop = NLPropperties(), NLActor *parent = 0);
    virtual ~SimpleCameraComponent();
    virtual void Reset();
    NLINTERFACE void Render();
    NLSETTER(type) void SetType(int type);
    NLSceneCamera * Camera();
    
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

    Q_DISABLE_COPY(SimpleCameraComponent)
};

#endif // _KARIN_SIMPLECAMERACOMPONENT_H
