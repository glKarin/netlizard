#include "simplelightsourcecomponent.h"

#include <QDebug>

#include "settings.h"
#include "qdef.h"
#include "nlscene.h"
#include "nlactor.h"

SimpleLightSourceComponent::SimpleLightSourceComponent(const NLPropperties &prop, NLActor *parent) :
    NLComponent(prop, parent),
    m_type(SimpleLightSourceComponent::LightSourceType_Direction)
{
    setObjectName("SimpleLightSourceComponent");
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
}

void SimpleLightSourceComponent::InitProperty()
{
    NLPropperty v;
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
