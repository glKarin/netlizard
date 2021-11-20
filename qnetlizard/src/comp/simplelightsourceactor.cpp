#include "simplelightsourceactor.h"

#include <QDebug>

#include "lib/vector3.h"
#include "simplelightsourcecomponent.h"

SimpleLightSourceActor::SimpleLightSourceActor(const NLProperties &prop, NLActor *parent) :
    NLActor(prop, parent),
    m_lightSource(0)
{
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

void SimpleLightSourceActor::Destroy()
{
    NLActor::Destroy();
}

void SimpleLightSourceActor::Update(float delta)
{
    if(!IsInited())
        return;
    NLActor::Update(delta);
    if(m_lightSource)
        m_lightSource->SetPosition(Position());
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
