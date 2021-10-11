#ifndef _KARIN_SIMPLECAMERACOMPONENT_H
#define _KARIN_SIMPLECAMERACOMPONENT_H

#include "nlcomponent.h"

#include "nlactor.h"

class NLSceneCamera;

class QWidget;

class SimpleCameraComponent : public NLComponent
{
    Q_OBJECT
public:
    explicit SimpleCameraComponent(const NLPropperties &prop = NLPropperties(), NLActor *parent = 0);
    virtual ~SimpleCameraComponent();
    NLINTERFACE void Render();
    NLSETTER(type) void SetType(int type);
    NLSceneCamera * Camera();
    
signals:
    
public slots:

protected:
    virtual void Init();
    virtual void Destroy();
    virtual void Update(float delta);
    virtual void Reset();

private Q_SLOTS:
    void OnPositionChanged(const NLVector3 &pos);
    void OnRotationChanged(const NLVector3 &rot);

private:
    NLPROPERTY(type) int m_type;
    NLSceneCamera *m_camera;

    Q_DISABLE_COPY(SimpleCameraComponent)
};

#endif // _KARIN_SIMPLECAMERACOMPONENT_H
