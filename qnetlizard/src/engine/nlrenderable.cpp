#include "nlrenderable.h"

#include <QDebug>

#include "qdef.h"

NLRenderable::NLRenderable(NLActor *actor)
    : m_actor(actor)
{
    SetName("NLRenderable");
}

NLRenderable::~NLRenderable()
{
    DEBUG_DESTROY("NLRenderable")
}

void NLRenderable::SetActor(NLActor *actor)
{
    m_actor = actor;
}

NLActor * NLRenderable::Actor()
{
    return m_actor;
}

QString NLRenderable::Name() const
{
    return m_name;
}

void NLRenderable::SetName(const QString &name)
{
    if(m_name != name)
        m_name = name;
}
