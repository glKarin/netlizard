#include "nlrigidbody.h"

#include <QDebug>

#include "nlfuncs.h"

static const NLVector3 InitUp_z = VECTOR3(0, 0, 1);
static const NLVector3 InitUp_y = VECTOR3(0, 1, 0);
static const NLVector3 InitDirection_z = VECTOR3(0, 1, 0);
static const NLVector3 InitDirection_y = VECTOR3(0, 0, -1);
static const NLVector3 InitRight = VECTOR3(1, 0, 0);
static const NLVector3 InitPosition = VECTOR3(0, 0, 0);
static const NLVector3 InitRotation = VECTOR3(/*-9*/0, 0, 0);
static const NLVector3 InitScale = VECTOR3(1, 1, 1);

NLRigidbody::NLRigidbody(NLActor *parent) :
    NLActor(parent),
    m_zIsUp(false),
    m_fixedUp(true),
    m_free(false)
{
    Construct();
}

NLRigidbody::NLRigidbody(const NLPropperties &prop, NLActor *parent) :
    NLActor(prop, parent),
    m_zIsUp(false),
    m_fixedUp(true),
    m_free(false)
{
    Construct();
}

NLRigidbody::NLRigidbody(NLScene *scene, NLActor *parent) :
    NLActor(scene, parent),
    m_zIsUp(false),
    m_fixedUp(true),
    m_free(false)
{
    Construct();
}

NLRigidbody::NLRigidbody(NLScene *scene, const NLPropperties &prop, NLActor *parent) :
    NLActor(scene, prop, parent),
    m_zIsUp(false),
    m_fixedUp(true),
    m_free(false)
{
    Construct();
}

NLRigidbody::~NLRigidbody()
{
    Mesa_FreeGLMatrix(&m_moveMatrix);
}

void NLRigidbody::Construct()
{
    Mesa_AllocGLMatrix(&m_moveMatrix);
    SetFixedUp(true);
    setObjectName("NLRigidbody");
    vector3_identityv(&m_moveRotation);
    m_moveDirection = Direction();
}

NLVector3 NLRigidbody::MoveDirection() const
{
    return m_moveDirection;
}

// limit x to [-90, 90]: [270, 360] | [0, 90]
// limit z to [-90, 90]: [270, 360] | [0, 90]
NLActor * NLRigidbody::Turn(const NLVector3 &v)
{
    if(m_free)
    {
        NLActor::Turn(v);
        return this;
    }

    if(vector3_iszero(&v))
        return this;

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
    VECTOR3_Y(m_moveRotation) = VECTOR3_Y(rot);
    UpdateMoveMatrix();
    return this;
}

void NLRigidbody::SetRotation(const NLVector3 &v)
{
    if(m_free)
    {
        NLActor::SetRotation(v);
        return;
    }

    NLVector3 rot = Rotation();
    NLVector3 nv = v;
    float x = NL::clamp_angle(VECTOR3_X(nv));
    if(x > 90 && x < 270)
        VECTOR3_X(nv) = VECTOR3_X(rot);
    float z = NL::clamp_angle(VECTOR3_Z(nv));
    if(z > 90 && z < 270)
        VECTOR3_Z(nv) = VECTOR3_Z(rot);
    NLActor::SetRotation(nv);

    if(VECTOR3_Y(m_moveDirection) == VECTOR3_Y(v))
        return;
    VECTOR3_Y(m_moveDirection) = VECTOR3_Y(v);
    UpdateMoveMatrix();
}

void NLRigidbody::UpdateMoveMatrix()
{
    Mesa_glLoadIdentity(&m_moveMatrix);

    Mesa_glRotate(&m_moveMatrix, VECTOR3_Y(m_moveRotation), 0, 1, 0);
    NL::cale_normal_matrix(m_moveMatrix, m_moveMatrix);

    float v[] = {0, 0, -1};
    Mesa_glTransform_row(VECTOR3_V(m_moveDirection), v, &m_moveMatrix);
}

NLActor * NLRigidbody::Move(const NLVector3 &v)
{
    NLVector3 pos = NLActor::Position();
    if(m_free)
        return NLActor::Move(v);

    if(vector3_iszero(&v))
        return this;

    NLVector3 right = NLActor::Right();
    NLVector3 up = NLActor::Up();
    vector3_moveve(&pos, &right, VECTOR3_X(v));
    vector3_moveve(&pos, &up, VECTOR3_Y(v));
    vector3_moveve(&pos, &m_moveDirection, VECTOR3_Z(v));
    NLActor::SetPosition(pos);
    return this;
}

void NLRigidbody::SetFree(bool b)
{
    if(m_free != b)
    {
        m_free = b;
        UpdateDirection();
    }
}

bool NLRigidbody::Free() const
{
    return m_free;
}


void NLRigidbody::SetZIsUp(bool b)
{
    if(m_zIsUp != b)
    {
        m_zIsUp = b;
        SetUp(m_zIsUp ? InitUp_z : InitUp_y);
        //UpdateMatrix();
        UpdateDirection();
    }
}

void NLRigidbody::SetFixedUp(bool b)
{
    if(m_fixedUp != b)
    {
        m_fixedUp = b;
        if(m_fixedUp)
        {
            SetUp(m_zIsUp ? InitUp_z : InitUp_y);
        }
        //UpdateMatrix();
        UpdateDirection();
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
    SetZIsUp(GetProperty<bool>("z_is_up", false));
    SetFixedUp(GetProperty<bool>("fixed_up", true));
    SetFree(GetProperty<bool>("free", true));
}

