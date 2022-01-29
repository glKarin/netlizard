#include "nlcomponent.h"

#include <QDebug>

#include "nldbg.h"
#include "nlactor.h"
#include "nlcomponentcontainer.h"

NLComponent::NLComponent(NLActor *parent) :
    NLObject(NLObject::Type_Component, 0, NLProperties(), parent),
    m_mounted(false)
{
    Construct();
    if(parent)
        SetScene(parent->Scene());
}

NLComponent::NLComponent(const NLProperties &prop, NLActor *parent) :
    NLObject(NLObject::Type_Component, 0, prop, parent),
    m_mounted(false)
{
    Construct();
}

NLComponent::NLComponent(NLScene *scene, NLActor *parent) :
    NLObject(NLObject::Type_Component, scene, NLProperties(), parent),
    m_mounted(false)
{
    Construct();
}

NLComponent::NLComponent(NLScene *scene, const NLProperties &prop, NLActor *parent) :
    NLObject(NLObject::Type_Component, scene, prop, parent),
    m_mounted(false)
{
    Construct();
}

NLComponent::~NLComponent()
{
    Destroy(); // !! vitrual
}

void NLComponent::Construct()
{
    NLObject *parent = ParentObject();
    if(parent)
        SetScene(parent->Scene());
    CLASS_NAME(NLComponent);
    if(objectName().isEmpty() || !GetInitProperty("name").isValid())
        setObjectName("NLComponent");
    SetType(NLObject::Type_Component);
}

void NLComponent::Update(float delta)
{
    if(!IsActived())
        return;
    //NLObject::Update(delta);
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
    if(!IsActived())
        return false;
    Q_UNUSED(key);
    Q_UNUSED(pressed);
    Q_UNUSED(modifier);
    return false;
}

bool NLComponent::mouseev(int mouse, bool pressed, int x, int y, int modifier)
{
    if(!IsActived())
        return false;
    Q_UNUSED(mouse);
    Q_UNUSED(pressed);
    Q_UNUSED(x);
    Q_UNUSED(y);
    Q_UNUSED(modifier);
    return false;
}

bool NLComponent::motionev(int mouse, bool pressed, int x, int y, int oldx, int oldy, int modifier)
{
    if(!IsActived())
        return false;
    Q_UNUSED(mouse);
    Q_UNUSED(pressed);
    Q_UNUSED(x);
    Q_UNUSED(y);
    Q_UNUSED(oldx);
    Q_UNUSED(oldy);
    Q_UNUSED(modifier);
    return false;
}

bool NLComponent::wheelev(int mouse, int orientation, int delta, int x, int y, int modifier)
{
    if(!IsActived())
        return false;
    Q_UNUSED(mouse);
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

const NLActor * NLComponent::Actor() const
{
    const QObject *p = parent();
    if(p)
        return dynamic_cast<const NLActor *>(p);
    return 0;
}

void NLComponent::SetActor(NLActor *actor)
{
    setParent(actor);
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
#ifdef _DEV_TEST
    qDebug() << objectName() + "(" + Name() + ") -> MOUNTED";
#endif
    emit mounted();
}

void NLComponent::Unmount()
{
    if(!IsMounted())
        return;
    NLActor *actor = Actor();
    if(actor)
    {
        actor->TellComponentRemoved();
        SetActor(0);
    }
    m_mounted = false;
#ifdef _DEV_TEST
    qDebug() << objectName() + "(" + Name() + ") -> UNMOUNTED";
#endif
    emit unmounted();
}

NLComponentContainer * NLComponent::Container()
{
    return static_cast<NLComponentContainer *>(NLObject::Container());
}

void NLComponent::SetContainer(NLComponentContainer *container)
{
    NLObject::SetContainer(container);
}
