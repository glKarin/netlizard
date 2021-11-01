#include "simplecameraactor.h"

#include <QDebug>

#include <GL/gl.h>
#include <GL/glu.h>

#include "nlscenecamera.h"
#include "nlsceneperspectivecamera.h"
#include "simplecontrol2dcomponent.h"
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
    NLPropperties prop;
    QVariant type = GetProperty("type");
    if(type.isValid())
        prop.insert("type", type);
    QVariant z_is_up = GetProperty("z_is_up");
    if(z_is_up.isValid())
        prop.insert("z_is_up", z_is_up);
    m_camera = new SimpleCameraComponent(prop, this);
    m_camera->SetScene(Scene());
    if(type == NLSceneCamera::Type_Ortho)
        m_control = new SimpleControl2DComponent(NLPropperties(), this);
    else
        m_control = new SimpleControlComponent(NLPropperties(), this);
    m_control->SetScene(Scene());
    SetEnableControl(GetProperty("enable_control", true));
    AddComponent(m_camera);
    AddComponent(m_control);

    if(type != NLSceneCamera::Type_Ortho)
    {
        SimpleControlComponent *control = static_cast<SimpleControlComponent *>(m_control);
        connect(control, SIGNAL(fovyChanged(float)), this, SLOT(OnFovyChanged(float)));
    }

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
    NLActor::Update(delta);
}

NLSceneCamera * SimpleCameraActor::Camera()
{
    return m_camera ? m_camera->Camera() : 0;
}

NLComponent * SimpleCameraActor::Control()
{
    return m_control;
}

void SimpleCameraActor::SetEnableControl(bool b)
{
    if(m_control)
        m_control->SetEnabled(b);
}

void SimpleCameraActor::OnFovyChanged(float f)
{
    NLScenePerspectiveCamera *camera = dynamic_cast<NLScenePerspectiveCamera *>(Camera());
    if(!camera)
        return;
    if(f == 0)
        camera->ResetFovy();
    else
        camera->SetFovy(camera->Fovy() + f);
}

void SimpleCameraActor::UpdateCamera()
{
    if(!m_camera)
        return;
    m_camera->UpdateCamera();
}
