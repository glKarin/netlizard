#include "nlrigidbody.h"

#include <QDebug>

#include "nlforce.h"
#include "nlforcecontainer.h"
#include "utils/nlmath.h"

NLRigidbody::NLRigidbody(NLActor *parent) :
    NLActor(NLPROPERTIY_NAME(NLRigidbody), parent),
    m_zIsUp(false),
    m_fixedUp(true),
    m_free(false),
    m_mass(1),
    m_forces(0)
{
    Construct();
}

NLRigidbody::NLRigidbody(const NLProperties &prop, NLActor *parent) :
    NLActor(NLPROPERTIES_NAME(prop, NLRigidbody), parent),
    m_zIsUp(false),
    m_fixedUp(true),
    m_free(false),
    m_mass(1),
    m_forces(0)
{
    Construct();
}

NLRigidbody::NLRigidbody(NLScene *scene, NLActor *parent) :
    NLActor(scene, NLPROPERTIY_NAME(NLRigidbody), parent),
    m_zIsUp(false),
    m_fixedUp(true),
    m_free(false),
    m_mass(1),
    m_forces(0)
{
    Construct();
}

NLRigidbody::NLRigidbody(NLScene *scene, const NLProperties &prop, NLActor *parent) :
    NLActor(scene, NLPROPERTIES_NAME(prop, NLRigidbody), parent),
    m_zIsUp(false),
    m_fixedUp(true),
    m_free(false),
    m_mass(1),
    m_forces(0)
{
    Construct();
}

NLRigidbody::~NLRigidbody()
{
    Mesa_FreeGLMatrix(&m_moveMatrix);
}

void NLRigidbody::Construct()
{
    CLASS_NAME(NLRigidbody);
    setObjectName("NLRigidbody");
    Mesa_AllocGLMatrix(&m_moveMatrix);
    SetFixedUp(true);
    vector3_identityv(&m_moveRotation);
    m_moveDirection = Direction();

    NLProperties props = PropertyConfig();
    props.Insert("mass", NLProperties("range", NLProperties("min", 0)));
    SetPropertyConfig(props);
}

// limit x to [-90, 90]: [270, 360] | [0, 90]
// limit z to [-90, 90]: [270, 360] | [0, 90]
NLActor & NLRigidbody::Turn(const NLVector3 &v)
{
    if(m_free)
    {
        NLActor::Turn(v);
        m_moveRotation = Rotation();
        UpdateMoveMatrix();
        return *this;
    }

    if(vector3_iszero(&v))
        return *this;

    NLVector3 rot = Rotation();
    NLVector3 nv = v;
    float x = NL::clamp_angle(VECTOR3_X(rot) + VECTOR3_X(v));
    float y = NL::clamp_angle(VECTOR3_Y(rot) + VECTOR3_Y(v));
    float z = NL::clamp_angle(VECTOR3_Z(rot) + VECTOR3_Z(v));
    if(x > 90 && x < 270)
    {
        x = 0;
        VECTOR3_X(nv) = 0;
    }
    if(z > 90 && z < 270)
    {
        z = 0;
        VECTOR3_Z(nv) = 0;
    }
    NLActor::Turn(nv);

    rot = Rotation();
    VECTOR3_Y(m_moveRotation) = y;
    UpdateMoveMatrix();
    return *this;
}

void NLRigidbody::SetRotation(const NLVector3 &v)
{
    if(m_free)
    {
        NLActor::SetRotation(v);
        m_moveRotation = Rotation();
        UpdateMoveMatrix();
        return;
    }

    //NLVector3 rot = Rotation();
    NLVector3 nv = v;
    float x = NL::clamp_angle(VECTOR3_X(nv));
    if(x > 90 && x < 270)
    {
        if(x <= 180)
            VECTOR3_X(nv) = 90;
        else
            VECTOR3_X(nv) = 270;
        //VECTOR3_X(nv) = VECTOR3_X(rot);
    }
    float z = NL::clamp_angle(VECTOR3_Z(nv));
    if(z > 90 && z < 270)
    {
        if(z <= 180)
            VECTOR3_Z(nv) = 90;
        else
            VECTOR3_Z(nv) = 270;
        //VECTOR3_Z(nv) = VECTOR3_Z(rot);
    }
    NLActor::SetRotation(nv);

    if(VECTOR3_Y(m_moveRotation) == VECTOR3_Y(v))
        return;
    VECTOR3_Y(m_moveRotation) = VECTOR3_Y(v);
    UpdateMoveMatrix();
}

void NLRigidbody::UpdateMoveMatrix()
{
    Mesa_glLoadIdentity(&m_moveMatrix);

    Mesa_glRotate(&m_moveMatrix, VECTOR3_Y(m_moveRotation), 0, 1, 0);
    NL::cale_normal_matrix(m_moveMatrix, m_moveMatrix);

    static const float v[] = {0, 0, -1};
    Mesa_glTransform_row(VECTOR3_V(m_moveDirection), v, &m_moveMatrix);
}

NLActor & NLRigidbody::Move(const NLVector3 &v)
{
    NLVector3 pos = NLActor::Position();
    if(m_free)
        return NLActor::Move(v);

    if(vector3_iszero(&v))
        return *this;

    const NLVector3 right = NLActor::Right();
    const NLVector3 up = NLActor::Up();
    vector3_moveve(&pos, &right, VECTOR3_X(v));
    vector3_moveve(&pos, &up, VECTOR3_Y(v));
    vector3_moveve(&pos, &m_moveDirection, VECTOR3_Z(v));
    NLActor::SetPosition(pos);
    return *this;
}

NLActor & NLRigidbody::MoveSelfOriginal(const NLVector3 &unit)
{
    NLVector3 pos = NLActor::Position();
    if(!m_zIsUp)
        return NLActor::MoveOriginal(unit);

    if(vector3_iszero(&unit))
        return *this;

    const NLVector3 &right = NL::Init_Right;
    const NLVector3 &up = m_zIsUp ? NL::Init_Up_z : NL::Init_Up_y;
    const NLVector3 &direction = m_zIsUp ? NL::Init_Direction_z : NL::Init_Direction_y;
    vector3_moveve(&pos, &right, VECTOR3_X(unit));
    vector3_moveve(&pos, &up, VECTOR3_Y(unit));
    vector3_moveve(&pos, &direction, VECTOR3_Z(unit));

    NLActor::SetPosition(pos);
    return *this;
}

void NLRigidbody::SetFree(bool b)
{
    if(m_free != b)
    {
        m_free = b;
        if(!m_free)
            SetRotation(Rotation());
        emit propertyChanged("free", m_free);
    }
}

void NLRigidbody::SetMass(NL::Physics::m m)
{
    if(m_mass != m)
    {
        m_mass = m;
        emit propertyChanged("mass", m_mass);
    }
}

void NLRigidbody::SetZIsUp(bool b)
{
    if(m_zIsUp != b)
    {
        m_zIsUp = b;
        SetUp(m_zIsUp ? NL::Init_Up_z : NL::Init_Up_y);
        //UpdateMatrix();
        UpdateDirection();
        emit propertyChanged("z_is_up", m_zIsUp);
    }
}

void NLRigidbody::SetFixedUp(bool b)
{
    if(m_fixedUp != b)
    {
        m_fixedUp = b;
        if(m_fixedUp)
        {
            SetUp(m_zIsUp ? NL::Init_Up_z : NL::Init_Up_y);
        }
        //UpdateMatrix();
        UpdateDirection();
        emit propertyChanged("fixed_up", m_fixedUp);
    }
}

void NLRigidbody::UpdateUp()
{
    if(!m_fixedUp)
    {
        NLActor::UpdateUp();
        return;
    }
}

void NLRigidbody::InitProperty()
{
    NLActor::InitProperty();
    SetZIsUp(GetInitProperty_T<bool>("z_is_up", false));
    SetFixedUp(GetInitProperty_T<bool>("fixed_up", true));
    SetFree(GetInitProperty_T<bool>("free", true));
    SetMass(GetInitProperty_T<float>("mass", 1));
}

bool NLRigidbody::AddForce(NLForce *item)
{
    if(!item)
        return false;
    if(!m_forces)
    {
        m_forces = new NLForceContainer(this);
        m_forces->SetScene(Scene());
    }
    bool res = m_forces->Add(item);
    emit forceChanged(item);
    return res;
}

bool NLRigidbody::RemoveForce(NLForce *item)
{
    if(!item)
        return false;
    if(!m_forces)
        return false;
    bool res = m_forces->Remove(item);
    if(res)
    {
        item->Destroy();
        delete item;
        emit forceChanged();
    }
    return res;
}

bool NLRigidbody::RemoveForce(int index)
{
    if(!m_forces)
        return false;
    NLForce *force = GetForce(index);
    return RemoveForce(force);
}

bool NLRigidbody::RemoveForce(const QString &name)
{
    if(!m_forces)
        return false;
    NLForce *force = GetForce(name);
    return RemoveForce(force);
}

NLForce * NLRigidbody::GetForce(const QString &name)
{
    if(!m_forces)
        return 0;
    return m_forces->Get(name);
}

NLForce * NLRigidbody::GetForce(int index)
{
    if(!m_forces)
        return 0;
    return m_forces->Get(index);
}

void NLRigidbody::Init()
{
    if(IsInited())
        return;
    if(m_forces)
        m_forces->Init();
    NLActor::Init();
}

void NLRigidbody::Update(float delta)
{
    if(!IsActived())
        return;
    if(m_forces)
        m_forces->Update(delta);
    NLActor::Update(delta);
}

void NLRigidbody::Destroy()
{
    if(!IsInited())
        return;
    NLActor::Destroy();
    if(m_forces)
    {
        m_forces->Destroy();
        delete m_forces;
        m_forces = 0;
    }
}

int NLRigidbody::ForceCount() const
{
    if(m_forces)
        return m_forces->Count();
    return 0;
}

void NLRigidbody::ClearAllForces()
{
    if(!m_forces)
        return;
    bool c = m_forces->Count() > 0;
    m_forces->Clean();
    if(c)
        emit forceChanged();
}

void NLRigidbody::Collision()
{
    if(!m_forces)
        return;
    bool c = m_forces->Count() > 0;
    m_forces->Clean();
    if(c)
        emit forceChanged();
}

void NLRigidbody::Reset()
{
    NLActor::Reset();
    ClearAllForces();
}
