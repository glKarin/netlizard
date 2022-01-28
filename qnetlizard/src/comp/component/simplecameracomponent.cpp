#include "simplecameracomponent.h"

#include <QDebug>
#include <QMetaObject>
#include <QMetaProperty>

#include "engine/nlactor.h"
#include "engine/nlscene.h"
#include "engine/nlscenecamera.h"
#include "utils/nlfuncs.h"
#include "qdef.h"

class CameraNotifyFunc : public NLSceneCamera::NLSceneCameraChangedNotify
{
public:
    explicit CameraNotifyFunc(SimpleCameraComponent *comp);
    virtual ~CameraNotifyFunc();
    virtual void PropertyChanged(const QString &name, const QVariant &value);
    virtual void ValueChanged(const QString &name, const QVariant &value) { Q_UNUSED(name); Q_UNUSED(value); }

private:
    SimpleCameraComponent *m_comp;
};

CameraNotifyFunc::CameraNotifyFunc(SimpleCameraComponent *comp)
    : NLSceneCameraBase::NLSceneCameraChangedNotify(NLSceneCamera::NLSceneCameraChangedNotify::Notify_PropertyChanged),
    m_comp(comp)
{
}

CameraNotifyFunc::~CameraNotifyFunc()
{
    m_comp = 0;
    DEBUG_DESTROY(CameraNotifyFunc)
}

void CameraNotifyFunc::PropertyChanged(const QString &name, const QVariant &value)
{
    if(m_comp)
    {
        m_comp->propertyChanged(name, value);
    }
}



SimpleCameraComponent::SimpleCameraComponent(const NLProperties &prop, NLActor *parent) :
    NLComponent(prop, parent),
      m_type(NLSceneCamera::Type_Perspective),
      m_camera(new NLSceneCamera),
      m_cameraNotifyFunc(new CameraNotifyFunc(this))
{
    CLASS_NAME(SimpleCameraComponent);
    setObjectName("SimpleCameraComponent");
    m_camera->SetChangedNotifyFunc(static_cast<CameraNotifyFunc *>(m_cameraNotifyFunc));

    NLProperties props = PropertyConfig();

    props.Insert("type", NLProperties("enum",
                                      QVariant::fromValue<NLPropertyPairList>(NLPropertyPairList()
                                                                              << NLPropertyPair(tr("Perspective"), static_cast<int>(NLSceneCamera::Type_Perspective))
                                                                              << NLPropertyPair(tr("Ortho"),  static_cast<int>(NLSceneCamera::Type_Ortho))
                                                                                      )));

    props.Insert("alignment",  NLProperties("enum", QVariant::fromValue<NLPropertyPairList>(NLPropertyPairList()
                                                                                            << NLPropertyPair(tr("Center"), static_cast<int>(Qt::AlignCenter))
                                                                                            << NLPropertyPair(tr("Left-Top"), static_cast<int>(Qt::AlignLeft | Qt::AlignTop))
                                                                                            << NLPropertyPair(tr("Left-Bottom"), static_cast<int>(Qt::AlignLeft | Qt::AlignBottom))
                                                                                            << NLPropertyPair(tr("Left-Center"), static_cast<int>(Qt::AlignLeft | Qt::AlignVCenter))
                                                                                            << NLPropertyPair(tr("Center-Top"), static_cast<int>(Qt::AlignHCenter | Qt::AlignTop))
                                                                                            << NLPropertyPair(tr("Center-Bottom"), static_cast<int>(Qt::AlignHCenter | Qt::AlignBottom))
                                                                                            )));

    SetPropertyConfig(props);
}

SimpleCameraComponent::~SimpleCameraComponent()
{

}

void SimpleCameraComponent::InitProperty()
{
    SetType(GetInitProperty_T<int>("type", static_cast<int>(NLSceneCamera::Type_Perspective)));
    if(GetInitProperty_T<bool>("camera_z_is_up", GetInitProperty_T<bool>("camera_z_is_up")))
        m_camera->SetZIsUp(true); // z_is_up
}

void SimpleCameraComponent::Init()
{
#if 0
    NLActor *actor = Actor();
    if(actor)
    {
        connect(actor, SIGNAL(positionChanged(const NLVector3 &)), this, SLOT(OnPositionChanged(const NLVector3 &)));
        connect(actor, SIGNAL(rotationChanged(const NLVector3 &)), this, SLOT(OnRotationChanged(const NLVector3 &)));
    }
#endif
    NLComponent::Init();
}

void SimpleCameraComponent::Update(float delta)
{
    if(!IsActived())
        return;
    UpdateCamera();
    NLComponent::Update(delta);
}

void SimpleCameraComponent::Reset()
{
    m_camera->Reset();
}

void SimpleCameraComponent::Destroy()
{
    delete m_camera;
    m_camera = 0;
    m_type = 0;
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
        //m_camera->SetScene(Scene());
        emit propertyChanged("type", m_type);
    }
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

void SimpleCameraComponent::SetLeft(float left)
{
    if(m_camera->Left() != left)
    {
        m_camera->SetLeft(left);
        emit propertyChanged("left", m_camera->Left());
    }
}

void SimpleCameraComponent::SetRight(float right)
{
    if(m_camera->Right() != right)
    {
        m_camera->SetRight(right);
        emit propertyChanged("right", m_camera->Right());
    }
}

void SimpleCameraComponent::SetTop(float top)
{
    if(m_camera->Top() != top)
    {
        m_camera->SetTop(top);
        emit propertyChanged("top", m_camera->Top());
    }
}

void SimpleCameraComponent::SetBottom(float bottom)
{
    if(m_camera->Bottom() != bottom)
    {
        m_camera->SetBottom(bottom);
        emit propertyChanged("bottom", m_camera->Bottom());
    }
}

void SimpleCameraComponent::SetZNear(float n)
{
    if(m_camera->ZNear() != n)
    {
        m_camera->SetZNear(n);
        emit propertyChanged("zNear", m_camera->ZNear());
    }
}

void SimpleCameraComponent::SetZFar(float f)
{
    if(m_camera->ZFar() != f)
    {
        m_camera->SetZFar(f);
        emit propertyChanged("zFar", m_camera->ZFar());
    }
}

void SimpleCameraComponent::SetAlignment(int align)
{
    Qt::Alignment a = static_cast<Qt::Alignment>(align);
    if(m_camera->Alignment() != a)
    {
        m_camera->SetAlignment(a);
        emit propertyChanged("alignment", QVariant::fromValue<int>(m_camera->Alignment()));
    }
}

void SimpleCameraComponent::SetAspect(float aspect)
{
    if(m_camera->Aspect() != aspect)
    {
        m_camera->SetAspect(aspect);
        emit propertyChanged("aspect", m_camera->Aspect());
    }
}

void SimpleCameraComponent::SetFovy(float fovy)
{
    if(m_camera->Fovy() != fovy)
    {
        m_camera->SetFovy(fovy);
        float f = m_camera->Fovy();
        if(f != fovy)
            emit propertyChanged("fovy", f);
    }
}

int SimpleCameraComponent::Alignment() const
{
    return m_camera->Alignment();
}

float SimpleCameraComponent::Left() const
{
    return m_camera->Left();
}

float SimpleCameraComponent::Right() const
{
    return m_camera->Right();
}

float SimpleCameraComponent::Bottom() const
{
    return m_camera->Bottom();
}

float SimpleCameraComponent::Top() const
{
    return m_camera->Top();
}

float SimpleCameraComponent::ZNear() const
{
    return m_camera->ZNear();
}

float SimpleCameraComponent::ZFar() const
{
    return m_camera->ZFar();
}

float SimpleCameraComponent::Fovy() const
{
    return m_camera->Fovy();
}

float SimpleCameraComponent::Aspect() const
{
    return m_camera->Aspect();
}

void SimpleCameraComponent::SetEnabled(bool enabled)
{
    SetRender(enabled);
    NLComponent::SetEnabled(enabled);
}

bool SimpleCameraComponent::IsEnabled() const
{
    return NLComponent::IsEnabled() && m_camera->IsEnabled();
}

void SimpleCameraComponent::SetRender(bool enabled)
{
    if(m_camera->IsEnabled() != enabled)
    {
        m_camera->SetEnabled(enabled);
        emit propertyChanged("render", enabled);
    }
}

bool SimpleCameraComponent::IsRender() const
{
    return m_camera->IsEnabled();
}

bool SimpleCameraComponent::ZIsUp() const
{
    return m_camera->ZIsUp();
}

void SimpleCameraComponent::SetZIsUp(bool b)
{
    m_camera->SetZIsUp(b);
}
