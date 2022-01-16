#include "simplelightsourcecomponent.h"

#include <QDebug>

#include "settings.h"
#include "qdef.h"
#include "engine/nlscene.h"
#include "engine/nlactor.h"

SimpleLightSourceComponent::SimpleLightSourceComponent(const NLProperties &prop, NLActor *parent) :
    NLComponent(prop, parent),
    m_type(SimpleLightSourceComponent::LightSourceType_Direction)
{
    CLASS_NAME(SimpleLightSourceComponent);
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
    if(!IsActived())
        return;
    NLActor *actor = Actor();
    if(actor)
        SetPosition(actor->Position());
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
    v = GetInitProperty("type");
    if(v.isValid())
        SetType(static_cast<SimpleLightSourceComponent::LightSourceType>(v.toInt()));
}

void SimpleLightSourceComponent::SetType(SimpleLightSourceComponent::LightSourceType type)
{
    if(m_type != type)
        m_type = type;
}

void SimpleLightSourceComponent::SetPosition(const NLVector3 &pos)
{
    if(!vector3_equals(&m_position, &pos))
    {
        m_position = pos;
        if(IsDirectionLighting())
        {
            NLVector3 dir = m_position;
            vector3_normalize(&dir);
            m_direction = dir;
            emit propertyChanged("direction", NLProperty::fromValue<NLVector3>(m_direction));
        }
        emit propertyChanged("position",  NLProperty::fromValue<NLVector3>(m_position));
    }
}

void SimpleLightSourceComponent::SetDirecton(const NLVector3 &dir)
{
    if(IsDirectionLighting())
        return;
    if(!vector3_equals(&m_direction, &dir))
    {
        m_direction = dir;
        emit propertyChanged("direction", NLProperty::fromValue<NLVector3>(m_direction));
    }
}
