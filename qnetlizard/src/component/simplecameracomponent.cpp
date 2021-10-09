#include "simplecameracomponent.h"

#include <QDebug>

#include "nlscene.h"

#include "sceneperspectivecamera.h"
#include "sceneorthocamera.h"

SimpleCameraComponent::SimpleCameraComponent(const NLPropperties &prop, NLActor *parent) :
    NLComponent(prop, parent),
      m_type(0),
      m_camera(0)
{
    setObjectName("SimpleCameraComponent");
}

SimpleCameraComponent::~SimpleCameraComponent()
{

}

void SimpleCameraComponent::Init()
{
    SetType(GetProperty<int>("type", static_cast<int>(SceneCamera::Type_Perspective)));
    NLComponent::Init();
}

void SimpleCameraComponent::Update(float delta)
{
    if(m_camera)
    {
        NLScene *scene = Scene();
        if(scene)
            m_camera->Update(scene->width(), scene->height());
    }
    NLComponent::Update(delta);
}

void SimpleCameraComponent::Reset()
{
    if(m_camera)
        m_camera->Reset();
}

void SimpleCameraComponent::Destroy()
{
    m_type = 0;
    if(m_camera)
    {
        delete m_camera;
        m_camera = 0;
    }
    NLComponent::Destroy();
}

void SimpleCameraComponent::Render()
{
    if(m_camera)
        m_camera->Render();
}

void SimpleCameraComponent::Move(const vector3_s *v)
{
    if(m_camera)
        m_camera->Move(v);
}

void SimpleCameraComponent::Turn(const vector3_s *v)
{
    if(m_camera)
        m_camera->Turn(v);
}

void SimpleCameraComponent::Zoom(const vector3_s *v)
{
    if(m_camera)
        m_camera->Zoom(v);
}

void SimpleCameraComponent::SetPosition(const vector3_s *v)
{
    if(m_camera)
        m_camera->SetPosition(v);
}

void SimpleCameraComponent::SetRotation(const vector3_s *v)
{
    if(m_camera)
        m_camera->SetRotation(v);
}

void SimpleCameraComponent::SetType(int type)
{
    if(m_type != type)
    {
        m_type = type;
        if(m_camera)
        {
            delete m_camera;
            m_camera = 0;
        }
        m_camera = m_type == SceneCamera::Type_Ortho ? (SceneCamera *)new SceneOrthoCamera : (SceneCamera *)new ScenePerspectiveCamera;
        m_camera->SetScene(Scene());
    }
}

SceneCamera * SimpleCameraComponent::Camera()
{
    return m_camera;
}
