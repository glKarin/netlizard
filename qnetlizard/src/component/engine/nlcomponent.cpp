#include "nlcomponent.h"

#include <QDebug>

#include "qdef.h"
#include "nlactor.h"
#include "nlcomponentcontainer.h"

NLComponent::NLComponent(NLActor *parent) :
    NLObject(parent),
    m_mounted(false)
{
    Construct();
    if(parent)
        SetScene(parent->Scene());
}

NLComponent::NLComponent(const NLPropperties &prop, NLActor *parent) :
    NLObject(prop, parent),
    m_mounted(false)
{
    Construct();
    if(parent)
        SetScene(parent->Scene());
}

NLComponent::NLComponent(NLScene *scene, NLActor *parent) :
    NLObject(scene, parent),
    m_mounted(false)
{
    Construct();
}

NLComponent::NLComponent(NLScene *scene, const NLPropperties &prop, NLActor *parent) :
    NLObject(scene, prop, parent),
    m_mounted(false)
{
    Construct();
}

NLComponent::~NLComponent()
{
}

void NLComponent::Construct()
{
    setObjectName("NLComponent");
    SetType(NLObject::Type_Component);
}

void NLComponent::Update(float delta)
{
    if(!IsInited())
        return;
    if(!IsMounted())
        return;
    NLObject::Update(delta);
}

void NLComponent::Destroy()
{
    if(!IsInited())
        return;
    if(IsMounted())
        Unmount();
    NLObject::Destroy();
}

bool NLComponent::keyev(int key, bool pressed, int modifier)
{
    Q_UNUSED(key);
    Q_UNUSED(pressed);
    Q_UNUSED(modifier);
    return false;
}

bool NLComponent::mouseev(int mouse, bool pressed, int x, int y, int modifier)
{
    Q_UNUSED(mouse);
    Q_UNUSED(pressed);
    Q_UNUSED(x);
    Q_UNUSED(y);
    Q_UNUSED(modifier);
    return false;
}

bool NLComponent::motionev(int mouse, bool pressed, int x, int y, int oldx, int oldy, int modifier)
{
    Q_UNUSED(mouse);
    Q_UNUSED(pressed);
    Q_UNUSED(x);
    Q_UNUSED(y);
    Q_UNUSED(oldx);
    Q_UNUSED(oldy);
    Q_UNUSED(modifier);
    return false;
}

bool NLComponent::wheelev(int orientation, int delta, int x, int y, int modifier)
{
    Q_UNUSED(orientation);
    Q_UNUSED(delta);
    Q_UNUSED(x);
    Q_UNUSED(y);
    Q_UNUSED(modifier);
    return false;
}

NLActor * NLComponent::Actor()
{
    QObject *p = parent();
    if(p)
        return dynamic_cast<NLActor *>(p);
    return 0;
}

void NLComponent::SetActor(NLActor *actor)
{
    setParent(actor);
}

bool NLComponent::IsMounted() const
{
    return m_mounted;
}

void NLComponent::Mount(NLActor *actor)
{
    if(!actor)
        return;
    if(IsMounted())
        return;
    if(!IsInited())
        Init();
    SetActor(actor);
    m_mounted = true;
    qDebug() << objectName() + ": " + Name() + " -> mounted";
}

void NLComponent::Unmount()
{
    if(!IsMounted())
        return;
    SetActor(0);
    m_mounted = false;
    qDebug() << objectName() + ": " + Name() + " ->unmounted";
}

NLComponentContainer * NLComponent::Container()
{
    return static_cast<NLComponentContainer *>(NLObject::Container());
}

void NLComponent::SetContainer(NLComponentContainer *container)
{
    NLObject::SetContainer(container);
}
