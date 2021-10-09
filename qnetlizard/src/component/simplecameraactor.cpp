#include "simplecameraactor.h"

#include <GL/gl.h>
#include <GL/glu.h>

#include "simplecontrolcomponent.h"
#include "simplecameracomponent.h"
#include "lib/vector3.h"

SimpleCameraActor::SimpleCameraActor(const NLPropperties &prop, NLActor *parent) :
    NLActor(prop, parent),
    m_camera(0),
    m_control(0)
{
    setObjectName("SimpleCameraActor");
}

SimpleCameraActor::~SimpleCameraActor()
{
}

void SimpleCameraActor::Init()
{
    if(IsInited())
        return;
    m_camera = new SimpleCameraComponent(NLPropperties(), this);
    m_camera->SetScene(Scene());
    m_control = new SimpleControlComponent(NLPropperties(), this);
    m_control->SetScene(Scene());
    AddComponent(m_camera);
    AddComponent(m_control);
    NLActor::Init();
}

void SimpleCameraActor::Destroy()
{
    NLActor::Destroy();
}

void SimpleCameraActor::Update(float delta)
{
    if(!IsInited())
        return;
    vector3_s v = m_control->CurrentMove();
    if(!vector3_iszero(&v))
        m_camera->Move(&v);
    v = m_control->CurrentTurn();
    if(!vector3_iszero(&v))
        m_camera->Turn(&v);
    v = m_control->CurrentRot();
    vector3_scalev(&v, delta * 20);
    if(!vector3_iszero(&v))
        m_camera->Turn(&v);
    NLActor::Update(delta);
}

SceneCamera * SimpleCameraActor::Camera()
{
    return m_camera ? m_camera->Camera() : 0;
}

void SimpleCameraActor::SetPosition(const vector3_s *v)
{
    if(m_camera)
        m_camera->SetPosition(v);
}

void SimpleCameraActor::SetRotation(const vector3_s *v)
{
    if(m_camera)
        m_camera->SetRotation(v);
}
