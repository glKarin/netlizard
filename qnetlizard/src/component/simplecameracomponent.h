#ifndef _KARIN_SIMPLECAMERACOMPONENT_H
#define _KARIN_SIMPLECAMERACOMPONENT_H

#include "nlcomponent.h"

#include "lib/vector3.h"

class SceneCamera;

class QWidget;

class SimpleCameraComponent : public NLComponent
{
    Q_OBJECT
public:
    explicit SimpleCameraComponent(const NLPropperties &prop = NLPropperties(), NLActor *parent = 0);
    virtual ~SimpleCameraComponent();
    NLINTERFACE void Render();
    NLINTERFACE void Move(const vector3_s *v);
    NLINTERFACE void Turn(const vector3_s *v);
    NLINTERFACE void Zoom(const vector3_s *v);
    NLINTERFACE void SetPosition(const vector3_s *v);
    NLINTERFACE void SetRotation(const vector3_s *v);
    NLSETTER(type) void SetType(int type);
    SceneCamera * Camera();
    
signals:
    
public slots:

protected:
    virtual void Init();
    virtual void Destroy();
    virtual void Update(float delta);
    virtual void Reset();

private:
    NLPROPERTY(type) int m_type;
    SceneCamera *m_camera;

    Q_DISABLE_COPY(SimpleCameraComponent)
};

#endif // _KARIN_SIMPLECAMERACOMPONENT_H
