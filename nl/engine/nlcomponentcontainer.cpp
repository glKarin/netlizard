#include "nlcomponentcontainer.h"

#include <QDebug>

#include "nldbg.h"
#include "nlactor.h"
#include "nlcomponent.h"

NLComponentContainer::NLComponentContainer(NLActor *parent) :
    NLObjectContainer(parent)
{
    Construct();
}

NLComponentContainer::NLComponentContainer(NLScene *scene, NLActor *parent) :
    NLObjectContainer(scene, parent)
{
    Construct();
}

NLComponentContainer::~NLComponentContainer()
{
}

void NLComponentContainer::Construct()
{
    setObjectName("NLComponentContainer");
}

bool NLComponentContainer::KeyEventHandler(int key, bool pressed, int modifier)
{
    int i = 0;
    NLObjectList &list = ObjectList();
    Q_FOREACH(NLObject *obj, list)
    {
        NLComponent *comp = static_cast<NLComponent *>(obj);
        if(ComponentIsAvailable(comp) && comp->IsActived())
            if(comp->keyev(key, pressed, modifier))
                i++;
    }
    return i > 0;
}

bool NLComponentContainer::MouseEventHandler(int button, bool pressed, int x, int y, int modifier)
{
    int i = 0;
    NLObjectList &list = ObjectList();
    Q_FOREACH(NLObject *obj, list)
    {
        NLComponent *comp = static_cast<NLComponent *>(obj);
        if(ComponentIsAvailable(comp) && comp->IsActived())
            if(comp->mouseev(button, pressed, x, y, modifier))
                i++;
    }
    return i > 0;
}

bool NLComponentContainer::MouseMotionHandler(int button, bool pressed, int x, int y, int oldx, int oldy, int modifier)
{
    int i = 0;
    NLObjectList &list = ObjectList();
    Q_FOREACH(NLObject *obj, list)
    {
        NLComponent *comp = static_cast<NLComponent *>(obj);
        if(ComponentIsAvailable(comp) && comp->IsActived())
            if(comp->motionev(button, pressed, x, y, oldx, oldy, modifier))
                i++;
    }
    return i > 0;
}

bool NLComponentContainer::WheelEventHandler(int mouse, int orientation, int delta, int x, int y, int modifier)
{
    int i = 0;
    NLObjectList &list = ObjectList();
    Q_FOREACH(NLObject *obj, list)
    {
        NLComponent *comp = static_cast<NLComponent *>(obj);
        if(ComponentIsAvailable(comp) && comp->IsActived())
            if(comp->wheelev(mouse, orientation, delta, x, y, modifier))
                i++;
    }
    return i > 0;
}

QString NLComponentContainer::Find(const NLComponent *item)
{
    return NLObjectContainer::Find(item);
}

bool NLComponentContainer::Add(NLComponent *item)
{
    bool res = NLObjectContainer::Add(item);
    if(res)
        item->Mount(Actor());
    return res;
}

bool NLComponentContainer::Remove(NLComponent *item)
{
    bool res = NLObjectContainer::Remove(item);
    return res;
}

bool NLComponentContainer::Remove(int index)
{
    NLComponent *item = Get(index);
    bool res = Remove(item);
    return res;
}

bool NLComponentContainer::Remove(const QString &name)
{
    NLComponent *item = Get(name);
    bool res = Remove(item);
    return res;
}

NLComponent * NLComponentContainer::Get(const QString &name)
{
    return static_cast<NLComponent *>(NLObjectContainer::Get(name));
}

NLComponent * NLComponentContainer::Get(int index)
{
    return static_cast<NLComponent *>(NLObjectContainer::Get(index));
}

NLActor * NLComponentContainer::Actor()
{
    QObject *p = parent();
    if(p)
        return dynamic_cast<NLActor *>(p);
    return 0;
}

const NLActor * NLComponentContainer::Actor() const
{
    const QObject *p = parent();
    if(p)
        return dynamic_cast<const NLActor *>(p);
    return 0;
}

void NLComponentContainer::SetActor(NLActor *actor)
{
    setParent(actor);
}

void NLComponentContainer::Clear()
{
    NLObjectContainer::Clear();
}

bool NLComponentContainer::ComponentIsAvailable(NLComponent *item) const
{
    return ObjectIsAvailable(item) && item->Actor() == Actor();
}

bool NLComponentContainer::ItemIsAvailable(NLObject *item) const
{
    return ComponentIsAvailable(static_cast<NLComponent *>(item));
}
