#include "nlrenderable.h"

#include <QDebug>

#include "qdef.h"

NLRenderable::NLRenderable(NLActor *actor)
    : m_actor(actor)
{
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
