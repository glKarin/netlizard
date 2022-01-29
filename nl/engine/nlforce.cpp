#include "nlforce.h"

#include <QDebug>

#include "nlrigidbody.h"
#include "nlforcecontainer.h"

NLForce::NLForce(NLRigidbody *parent) :
    NLObject(NLObject::Type_Force, 0, NLProperties(), parent),
  m_force(0),
  m_time(0),
  m_state(NLForce::State_Ready),
    m_once(true)
{
    Construct();
    if(parent)
        SetScene(parent->Scene());
}

NLForce::NLForce(const NLProperties &prop, NLRigidbody *parent) :
    NLObject(NLObject::Type_Force, 0, prop, parent),
          m_force(0),
          m_time(0),
          m_state(NLForce::State_Ready),
    m_once(true)
{
    Construct();
}

NLForce::NLForce(NLScene *scene, NLRigidbody *parent) :
    NLObject(NLObject::Type_Force, scene, NLProperties(), parent),
          m_force(0),
          m_time(0),
          m_state(NLForce::State_Ready),
    m_once(true)
{
    Construct();
}

NLForce::NLForce(NLScene *scene, const NLProperties &prop, NLRigidbody *parent) :
    NLObject(NLObject::Type_Force, scene, prop, parent),
          m_force(0),
          m_time(0),
          m_state(NLForce::State_Ready),
    m_once(true)
{
    Construct();
}

NLForce::~NLForce()
{

}

void NLForce::Construct()
{
    NLObject *parent = ParentObject();
    if(parent)
        SetScene(parent->Scene());
    CLASS_NAME(NLForce);
    if(objectName().isEmpty() || !GetInitProperty("name").isValid())
        setObjectName("NLForce");
    SetType(NLObject::Type_Force);
    VECTOR3_X(m_direction) = VECTOR3_Y(m_direction) = VECTOR3_Z(m_direction) = 0;
}

void NLForce::SetOnce(bool b)
{
    if(m_once != b)
    {
        m_once = b;
        emit propertyChanged("once", m_once);
    }
}

void NLForce::Reset()
{
    m_time = 0;
    SetState(NLForce::State_Ready);
}

void NLForce::SetRigidbody(NLRigidbody *o)
{
    if(Rigidbody() != o)
    {
        Finish();
        setParent(o);
    }
}

void NLForce::Update(float delta)
{
    if(!IsActived())
        return;
    if(m_state != NLForce::State_Forcing)
        return;
    m_time += delta;
    //NLObject::Update(delta);
}

void NLForce::Start()
{
    if(m_state != NLForce::State_Ready)
        return;
    SetState(NLForce::State_Forcing);
    emit started();
}

void NLForce::Finish()
{
    if(m_state != NLForce::State_Forcing)
        return;
    SetState(NLForce::State_Finish);
    emit finished();
}

void NLForce::SetState(NLForce::State s)
{
    if(m_state != s)
        m_state = s;
}

NLRigidbody * NLForce::Rigidbody()
{
    return static_cast<NLRigidbody *>(parent());
}

void NLForce::InitProperty()
{
    NLObject::InitProperty();
    m_force = GetInitProperty_T<float>("force", 0);
    m_direction = GetInitProperty_T<NLVector3>("direction");
}

void NLForce::Destroy()
{
    NLObject::Destroy();
    Finish();
}

void NLForce::SetContainer(NLForceContainer *container)
{
    NLObject::SetContainer(container);
}

bool NLForce::IsActived() const
{
    return NLObject::IsActived() && (const_cast<NLForce *>(this))->Rigidbody() != 0;
}

NL::Physics::m NLForce::Mass() const
{
    const NLRigidbody *r = (const_cast<NLForce *>(this))->Rigidbody();
    return r ? r->Mass() : 0;
}

NL::Physics::a NLForce::Acceleration() const
{
    NL::Physics::m mass = Mass();
    return NL::Physics::acceleration(m_force, mass);
}

NL::Physics::a NLForce::Acceleration(NL::Physics::F f0) const
{
    NL::Physics::m mass = Mass();
    return NL::Physics::acceleration(f0, mass);
}



// gravity
NLForce_gravity::NLForce_gravity(NLRigidbody *parent) :
    NLForce(parent),
    m_g(NL::Physics::EARTH_G),
    m_initialSpeed(0),
    m_distance(0),
    m_speed(0),
    m_lastDistance(0)
{
    Construct();
}

NLForce_gravity::NLForce_gravity(const NLProperties &prop, NLRigidbody *parent) :
    NLForce(prop, parent),
    m_g(NL::Physics::EARTH_G),
    m_initialSpeed(0),
    m_distance(0),
    m_speed(0),
    m_lastDistance(0)
{
    Construct();
}

NLForce_gravity::NLForce_gravity(NLScene *scene, NLRigidbody *parent) :
    NLForce(scene, parent),
    m_g(NL::Physics::EARTH_G),
    m_initialSpeed(0),
    m_distance(0),
    m_speed(0),
    m_lastDistance(0)
{
    Construct();
}

NLForce_gravity::NLForce_gravity(NLScene *scene, const NLProperties &prop, NLRigidbody *parent) :
    NLForce(scene, prop, parent),
    m_g(NL::Physics::EARTH_G),
    m_initialSpeed(0),
    m_distance(0),
    m_speed(0),
    m_lastDistance(0)
{
    Construct();
}

NLForce_gravity::~NLForce_gravity()
{

}

void NLForce_gravity::InitProperty()
{
    NLForce::InitProperty();
    m_g = GetInitProperty_T<float>("g", NL::Physics::EARTH_G);
    m_initialSpeed = Acceleration();
    m_speed = m_initialSpeed;
}

void NLForce_gravity::Construct()
{
    CLASS_NAME(NLForce_gravity);
    setObjectName("NLForce_gravity");
    m_force = NL::Physics::gravity_force(1, m_g);
    NLVector3 down = VECTOR3(0, -1, 0);
    m_direction = down;
}

void NLForce_gravity::Reset()
{
    NLForce::Reset();
    m_distance = 0;
    m_speed = 0;
    m_lastDistance = 0;
}

void NLForce_gravity::Update(float delta)
{
    if(!IsActived())
        return;
    if(!IsForcing())
        return;
    NLForce::Update(delta);

    m_lastDistance = NL::Physics::gravity_distance(delta, m_speed, m_g);
    m_speed = NL::Physics::gravity_speed(m_time, m_initialSpeed, m_g);
    m_distance = NL::Physics::gravity_distance(m_time, m_initialSpeed, m_g);

    NLRigidbody *actor = Rigidbody();
    if(actor)
    {
        NLVector3 unit = VECTOR3(0, -m_lastDistance, 0);
        actor->MoveOriginal(unit);
    }
}



// push
NLForce_push::NLForce_push(NLRigidbody *parent) :
    NLForce(parent),
    m_dragForce(0),
    m_initialSpeed(0),
    m_distance(0),
    m_speed(0),
    m_lastDistance(0),
    m_acceleration(0)
{
    Construct();
}

NLForce_push::NLForce_push(const NLProperties &prop, NLRigidbody *parent) :
    NLForce(prop, parent),
    m_dragForce(NL::Physics::EARTH_G),
    m_initialSpeed(0),
    m_distance(0),
    m_speed(0),
    m_lastDistance(0),
    m_acceleration(0)
{
    Construct();
}

NLForce_push::NLForce_push(NLScene *scene, NLRigidbody *parent) :
    NLForce(scene, parent),
    m_dragForce(0),
    m_initialSpeed(0),
    m_distance(0),
    m_speed(0),
    m_lastDistance(0),
    m_acceleration(0)
{
    Construct();
}

NLForce_push::NLForce_push(NLScene *scene, const NLProperties &prop, NLRigidbody *parent) :
    NLForce(scene, prop, parent),
    m_dragForce(0),
    m_initialSpeed(0),
    m_distance(0),
    m_speed(0),
    m_lastDistance(0),
    m_acceleration(0)
{
    Construct();
}

NLForce_push::~NLForce_push()
{

}

void NLForce_push::InitProperty()
{
    NLForce::InitProperty();
    m_dragForce = GetInitProperty_T<float>("drag_force", 0);
    m_initialSpeed = Acceleration();
    m_speed = m_initialSpeed;
    m_acceleration = NL::Physics::acceleration(m_force + m_dragForce, Mass());
}

void NLForce_push::Construct()
{
    CLASS_NAME(NLForce_push);
    setObjectName("NLForce_push");
}

void NLForce_push::Reset()
{
    NLForce::Reset();
    m_distance = 0;
    m_speed = 0;
    m_lastDistance = 0;
}

void NLForce_push::Update(float delta)
{
    if(!IsActived())
        return;
    if(!IsForcing())
        return;
    NLForce::Update(delta);

    m_speed = NL::Physics::speed(m_time, m_acceleration, m_initialSpeed);
    if(m_speed <= 0)
    {
        m_speed = 0;
        Finish();
        return;
    }
    m_lastDistance = NL::Physics::distance(delta, m_speed, m_acceleration);
    m_distance = NL::Physics::distance(m_time, m_initialSpeed, m_acceleration);

    NLRigidbody *actor = Rigidbody();
    if(actor)
    {
        actor->MoveDirectionOriginal(m_lastDistance, m_direction);
    }
}
