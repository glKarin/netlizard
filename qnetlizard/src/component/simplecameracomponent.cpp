#include "simplecameracomponent.h"

#include <QDebug>

#include "nlscene.h"

#include "nlsceneperspectivecamera.h"
#include "nlsceneorthocamera.h"

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
    SetType(GetProperty<int>("type", static_cast<int>(NLSceneCamera::Type_Perspective)));
    NLActor *actor = Actor();
    if(actor)
    {
        connect(actor, SIGNAL(positionChanged(const NLVector3 &)), this, SLOT(OnPositionChanged(const NLVector3 &)));
        connect(actor, SIGNAL(rotationChanged(const NLVector3 &)), this, SLOT(OnRotationChanged(const NLVector3 &)));
    }
    NLComponent::Init();
}

void SimpleCameraComponent::Update(float delta)
{
    if(m_camera)
    {
        NLScene *scene = Scene();
        if(scene)
            m_camera->Update(scene->width(), scene->height());
        NLActor *actor = Actor();
        if(actor)
            m_camera->SetModelViewMatrix(actor->GlobalMatrix());
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

void SimpleCameraComponent::OnPositionChanged(const NLVector3 &pos)
{
    if(m_camera)
    {
        NLActor *actor = Actor();
        if(actor)
            m_camera->SetModelViewMatrix(actor->GlobalMatrix());
    }
}

void SimpleCameraComponent::OnRotationChanged(const NLVector3 &rot)
{
    if(m_camera)
    {
        NLActor *actor = Actor();
        if(actor)
            m_camera->SetModelViewMatrix(actor->GlobalMatrix());
    }
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
        m_camera = m_type == NLSceneCamera::Type_Ortho ? (NLSceneCamera *)new NLSceneOrthoCamera : (NLSceneCamera *)new NLScenePerspectiveCamera;
        m_camera->SetScene(Scene());
    }
}

NLSceneCamera * SimpleCameraComponent::Camera()
{
    return m_camera;
}
