#include "simplelightsourceactor.h"

#include <QDebug>

#include "simplelightsourcecomponent.h"

SimpleLightSourceActor::SimpleLightSourceActor(const NLProperties &prop, NLActor *parent) :
    NLActor(prop, parent),
    m_lightSource(0)
{
    CLASS_NAME(SimpleLightSourceActor);
    setObjectName("SimpleLightSourceActor");
}

SimpleLightSourceActor::~SimpleLightSourceActor()
{
}

void SimpleLightSourceActor::Init()
{
    if(IsInited())
        return;
    NLProperties prop;
    QVariant type = GetInitProperty("type");
    if(type.isValid())
        prop.insert("type", type);
    m_lightSource = new SimpleLightSourceComponent(prop, this);
    m_lightSource->SetScene(Scene());
    AddComponent(m_lightSource);

    NLActor::Init();
}

NLVector3 SimpleLightSourceActor::LightSourceDirection() const
{
    if(!m_lightSource)
        return Direction();
    return m_lightSource->Direction();
}

NLVector3 SimpleLightSourceActor::LightSourcePosition() const
{
    if(!m_lightSource)
        return Position();
    return m_lightSource->Position();
}

bool SimpleLightSourceActor::IsDirectionLighting() const
{
    if(!m_lightSource)
        return false;
    return m_lightSource->IsDirectionLighting();
}
