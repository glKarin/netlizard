#include "simplelightsourcecomponent.h"

#include <QDebug>

#include "settings.h"
#include "qdef.h"
#include "nlscene.h"
#include "nlactor.h"

SimpleLightSourceComponent::SimpleLightSourceComponent(const NLProperties &prop, NLActor *parent) :
    NLComponent(prop, parent),
    m_type(SimpleLightSourceComponent::LightSourceType_Direction)
{
    setObjectName("SimpleLightSourceComponent");
    NLVector3 v = VECTOR3(0, 0, 0);
    SetPosition(v);
    SetDirecton(v);
}

SimpleLightSourceComponent::~SimpleLightSourceComponent()
{

}

void SimpleLightSourceComponent::Update(float delta)
{
    NLComponent::Update(delta);
}

void SimpleLightSourceComponent::Init()
{
    NLComponent::Init();
    Reset();
}

void SimpleLightSourceComponent::Reset()
{
    NLComponent::Reset();
    NLVector3 v = VECTOR3(0, 0, 0);
    SetPosition(v);
    SetDirecton(v);
}

void SimpleLightSourceComponent::InitProperty()
{
    NLProperty v;
    v = GetProperty("type");
    if(v.isValid())
        SetType(static_cast<SimpleLightSourceComponent::LightSourceType>(v.toInt()));
}

void SimpleLightSourceComponent::SetType(SimpleLightSourceComponent::LightSourceType type)
{
    if(m_type != type)
        m_type = type;
}

SimpleLightSourceComponent::LightSourceType SimpleLightSourceComponent::Type() const
{
    return m_type;
}

bool SimpleLightSourceComponent::IsDirectionLighting() const
{
    return m_type == SimpleLightSourceComponent::LightSourceType_Direction;
}

NLVector3 SimpleLightSourceComponent::Direction() const
{
    return m_direction;
}

NLVector3 SimpleLightSourceComponent::Position() const
{
    return m_position;
}

void SimpleLightSourceComponent::SetPosition(const NLVector3 &pos)
{
    if(!vector3_equals(&m_position, &pos))
    {
        VECTOR3_X(m_position) = VECTOR3_X(pos);
        VECTOR3_Y(m_position) = VECTOR3_Y(pos);
        VECTOR3_Z(m_position) = VECTOR3_Z(pos);
        if(IsDirectionLighting())
        {
            NLVector3 dir = m_position;
            vector3_normalize(&dir);
            VECTOR3_X(m_direction) = VECTOR3_X(dir);
            VECTOR3_Y(m_direction) = VECTOR3_Y(dir);
            VECTOR3_Z(m_direction) = VECTOR3_Z(dir);
        }
    }
}

void SimpleLightSourceComponent::SetDirecton(const NLVector3 &dir)
{
    if(IsDirectionLighting())
        return;
    if(!vector3_equals(&m_direction, &dir))
    {
        VECTOR3_X(m_direction) = VECTOR3_X(dir);
        VECTOR3_Y(m_direction) = VECTOR3_Y(dir);
        VECTOR3_Z(m_direction) = VECTOR3_Z(dir);
    }
}
