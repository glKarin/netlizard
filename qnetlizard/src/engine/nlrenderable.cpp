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
    if(m_actor != actor)
        m_actor = actor;
}

void NLRenderable::SetName(const QString &name)
{
    if(m_name != name)
        m_name = name;
}
