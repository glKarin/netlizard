#include "nlforce.h"

#include <QDebug>

#include "nlrigidbody.h"
#include "nlforcecontainer.h"

NLForce::NLForce(NLRigidbody *parent) :
    NLObject(parent),
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
    NLObject(prop, parent),
          m_force(0),
          m_time(0),
          m_state(NLForce::State_Ready),
    m_once(true)
{
    Construct();
}

NLForce::NLForce(NLScene *scene, NLRigidbody *parent) :
    NLObject(scene, parent),
          m_force(0),
          m_time(0),
          m_state(NLForce::State_Ready),
    m_once(true)
{
    Construct();
}

NLForce::NLForce(NLScene *scene, const NLProperties &prop, NLRigidbody *parent) :
    NLObject(scene, prop, parent),
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
    setObjectName("NLForce");
    SetType(NLObject::Type_Force);
    VECTOR3_X(m_direction) = VECTOR3_Y(m_direction) = VECTOR3_Z(m_direction) = 0;
}

void NLForce::SetOnce(bool b)
{
    if(m_once != b)
        m_once = b;
}

bool NLForce::Once() const
{
    return m_once;
}

void NLForce::Reset()
{
    m_time = 0;
    SetState(NLForce::State_Ready);
}

NL::Physics::t NLForce::Time() const
{
    return m_time;
}

NL::Physics::F NLForce::Force() const
{
    return m_force;
}

void NLForce::SetRigidbody(NLRigidbody *o)
{
    if(Rigidbody() != o)
    {
        Finish();
        setParent(o);
    }
}

bool NLForce::IsForcing() const
{
    return m_state == NLForce::State_Forcing;
}

bool NLForce::IsFinished() const
{
    return m_state == NLForce::State_Finish;
}

bool NLForce::IsStarted() const
{
    return m_state != NLForce::State_Ready;
}

NLForce::State NLForce::ForceState() const
{
    return m_state;
}

void NLForce::Update(float delta)
{
    NLObject::Update(delta);
    if(m_state != NLForce::State_Forcing)
        return;
    m_time += delta;
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
    m_force = GetProperty_T<float>("force", 0);
    VECTOR3_X(m_direction) = GetProperty_T<float>("direction_x", 0);
    VECTOR3_Y(m_direction) = GetProperty_T<float>("direction_y", 0);
    VECTOR3_Z(m_direction) = GetProperty_T<float>("direction_z", 0);
}

void NLForce::Destroy()
{
    NLObject::Destroy();
    Finish();
}

NLVector3 NLForce::Direction() const
{
    return m_direction;
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
    m_g = GetProperty_T<float>("g", NL::Physics::EARTH_G);
    m_initialSpeed = Acceleration();
    m_speed = m_initialSpeed;
}

void NLForce_gravity::Construct()
{
    m_force = NL::Physics::gravity_force(1, m_g);
    NLVector3 down = VECTOR3(0, -1, 0);
    m_direction = down;
}

NL::Physics::g NLForce_gravity::Gravity() const
{
    return m_g;
}

NL::Physics::d NLForce_gravity::Distance() const
{
    return m_distance;
}

NL::Physics::v NLForce_gravity::Speed() const
{
    return m_speed;
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
