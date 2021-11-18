#include "simplecameracomponent.h"

#include <QDebug>
#include <QMetaObject>
#include <QMetaProperty>

#include "nlscene.h"
#include "nlfuncs.h"

#include "nlsceneperspectivecamera.h"
#include "nlsceneorthocamera.h"
#include "nlscenecamera.h"

SimpleCameraComponent::SimpleCameraComponent(const NLProperties &prop, NLActor *parent) :
    NLComponent(prop, parent),
      m_type(NLSceneCamera::Type_Perspective),
      m_camera(new NLSceneCamera)
{
    setObjectName("SimpleCameraComponent");
}

SimpleCameraComponent::~SimpleCameraComponent()
{

}

void SimpleCameraComponent::InitProperty()
{
    SetType(GetInitProperty_T<int>("type", static_cast<int>(NLSceneCamera::Type_Perspective)));
}

void SimpleCameraComponent::Init()
{
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
    UpdateCamera();
    NLComponent::Update(delta);
}

void SimpleCameraComponent::Reset()
{
    m_camera->Reset();
}

void SimpleCameraComponent::Destroy()
{
    m_type = 0;
    delete m_camera;
    NLComponent::Destroy();
}

void SimpleCameraComponent::Render()
{
    m_camera->Render();
}

void SimpleCameraComponent::OnPositionChanged(const NLVector3 &pos)
{
    //NLActor *actor = Actor();
    //if(actor)
        m_camera->SetPosition(pos);
}

void SimpleCameraComponent::OnRotationChanged(const NLVector3 &rot)
{
    //NLActor *actor = Actor();
    //if(actor)
        m_camera->SetRotation(rot);
}

void SimpleCameraComponent::SetType(int type)
{
    if(m_type != type)
    {
        m_type = type;
        m_camera->SetType(static_cast<NLSceneCamera::SceneCamera_Type>(m_type));
        if(GetProperty_T<bool>("camera_z_is_up", GetInitProperty_T<bool>("camera_z_is_up")))
            m_camera->SetZIsUp(true); // z_is_up
        //m_camera->SetScene(Scene());
        emit propertyChanged("type", m_type);
    }
}

int SimpleCameraComponent::Type() const
{
    return m_type;
}

NLSceneCamera * SimpleCameraComponent::Camera()
{
    return m_camera;
}

void SimpleCameraComponent::UpdateCamera()
{
    NLScene *scene = Scene();
    if(scene)
        m_camera->Update(scene->width(), scene->height());
    NLActor *actor = Actor();
    if(actor)
    {
        NLActor *pa = actor->ParentActor();
        if(pa)
            m_camera->SetGlobalMatrix(pa->GlobalMatrix());
        m_camera->SetPosition(actor->Position());
        m_camera->SetRotation(actor->Rotation());
    }
}

void SimpleCameraComponent::SetScene(NLScene *scene)
{
    NLComponent::SetScene(scene);
    m_camera->SetScene(scene);
}
