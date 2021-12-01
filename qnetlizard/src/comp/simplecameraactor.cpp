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

SimpleCameraActor::SimpleCameraActor(const NLProperties &prop, NLActor *parent) :
    NLRigidbody(prop, parent),
    m_camera(0),
    m_control(0)
{
    CLASS_NAME(SimpleCameraActor);
    setObjectName("SimpleCameraActor");
}

SimpleCameraActor::~SimpleCameraActor()
{
}

void SimpleCameraActor::Init()
{
    if(IsInited())
        return;
    NLProperties prop;
    QVariant type = GetInitProperty("type");
    if(type.isValid())
        prop.insert("type", type);
    QVariant z_is_up = GetInitProperty("camera_z_is_up");
    if(z_is_up.isValid())
        prop.insert("camera_z_is_up", z_is_up);
    m_camera = new SimpleCameraComponent(prop, this);
    m_camera->SetScene(Scene());
    if(type == NLSceneCamera::Type_Ortho)
        m_control = new SimpleControl2DComponent(NLProperties(), this);
    else
        m_control = new SimpleControlComponent(NLProperties(), this);
    m_control->SetScene(Scene());
    SetEnableControl(GetInitProperty_T<bool>("enable_control", true));
    AddComponent(m_camera);
    AddComponent(m_control);

    if(type != NLSceneCamera::Type_Ortho)
    {
        SimpleControlComponent *control = static_cast<SimpleControlComponent *>(m_control);
        connect(control, SIGNAL(fovyChanged(float)), this, SLOT(OnFovyChanged(float)));
    }

    NLRigidbody::Init();
}

NLSceneCamera * SimpleCameraActor::Camera()
{
    return m_camera ? m_camera->Camera() : 0;
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

void SimpleCameraActor::SetEnabled(bool enabled)
{
    NLActor::SetEnabled(enabled);
    if(m_camera)
        m_camera->SetRender(enabled);
}
