#include "nlactorcontainer.h"

#include <QDebug>

#include "qdef.h"
#include "nlactor.h"

NLActorContainer::NLActorContainer(QObject *parent) :
    NLObjectContainer(parent)
{
    Construct();
}

NLActorContainer::NLActorContainer(NLScene *scene, QObject *parent) :
    NLObjectContainer(scene, parent)
{
    Construct();
}

NLActorContainer::~NLActorContainer()
{
}

void NLActorContainer::Construct()
{
    setObjectName("NLActorContainer");
}

bool NLActorContainer::KeyEventHandler(int key, bool pressed, int modifier)
{
    int i = 0;
    NLObjectList &list = ObjectList();
    Q_FOREACH(NLObject *obj, list)
    {
        NLActor *actor = static_cast<NLActor *>(obj);
        if(actor->IsActived())
            if(actor->keyev(key, pressed, modifier))
                i++;
    }
    return i > 0;
}

bool NLActorContainer::MouseEventHandler(int button, bool pressed, int x, int y, int modifier)
{
    int i = 0;
    NLObjectList &list = ObjectList();
    Q_FOREACH(NLObject *obj, list)
    {
        NLActor *actor = static_cast<NLActor *>(obj);
        if(actor->IsActived())
            if(actor->mouseev(button, pressed, x, y, modifier))
                i++;
    }
    return i > 0;
}

bool NLActorContainer::MouseMotionHandler(int button, bool pressed, int x, int y, int oldx, int oldy, int modifier)
{
    int i = 0;
    NLObjectList &list = ObjectList();
    Q_FOREACH(NLObject *obj, list)
    {
        NLActor *actor = static_cast<NLActor *>(obj);
        if(actor->IsActived())
            if(actor->motionev(button, pressed, x, y, oldx, oldy, modifier))
                i++;
    }
    return i > 0;
}

bool NLActorContainer::WheelEventHandler(int mouse, int orientation, int delta, int x, int y, int modifier)
{
    int i = 0;
    NLObjectList &list = ObjectList();
    Q_FOREACH(NLObject *obj, list)
    {
        NLActor *actor = static_cast<NLActor *>(obj);
        if(actor->IsActived())
            if(actor->wheelev(mouse, orientation, delta, x, y, modifier))
                i++;
    }
    return i > 0;
}

NLName NLActorContainer::Find(const NLActor *item)
{
    return NLObjectContainer::Find(item);
}

bool NLActorContainer::Add(NLActor *item)
{
    bool res = NLObjectContainer::Add(item);
    if(res)
        item->SetParentActor(Actor());
    return res;
}

bool NLActorContainer::Remove(NLActor *item)
{
    bool res = NLObjectContainer::Remove(item);
    if(res)
        item->SetParentActor(0);
    return res;
}

bool NLActorContainer::Remove(int index)
{
    NLActor *item = Get(index);
    if(!item)
        return false;
    bool res = NLObjectContainer::Remove(item);
    if(res)
        item->SetParentActor(0);
    return res;
}

bool NLActorContainer::Remove(const NLName &name)
{
    NLActor *item = Get(name);
    if(!item)
        return false;
    bool res = NLObjectContainer::Remove(item);
    if(res)
        item->SetParentActor(0);
    return res;
}

NLActor * NLActorContainer::Get(const NLName &name)
{
    return static_cast<NLActor *>(NLObjectContainer::Get(name));
}

NLActor * NLActorContainer::Get(int index)
{
    return static_cast<NLActor *>(NLObjectContainer::Get(index));
}

void NLActorContainer::Render()
{
    NLObjectList &list = ObjectList();
    Q_FOREACH(NLObject *obj, list)
    {
        NLActor *actor = static_cast<NLActor *>(obj);
        if(actor->IsActived())
            actor->Render();
    }
}

NLActor * NLActorContainer::Actor()
{
    QObject *p = parent();
    if(p)
        return dynamic_cast<NLActor *>(p);
    return 0;
}

void NLActorContainer::SetActor(NLActor *actor)
{
    setParent(actor);
}

void NLActorContainer::Clear()
{
    NLObjectList &list = ObjectList();
    Q_FOREACH(NLObject *obj, list)
    {
        (static_cast<NLActor *>(obj))->SetParentActor(0);
    }
    NLObjectContainer::Clear();
}

int NLActorContainer::TotalCount() const
{
    const NLObjectList &list = ObjectList();
    int res = list.size();
    Q_FOREACH(const NLObject *obj, list)
    {
        res += (static_cast<const NLActor *>(obj))->ChildrenTotalCount();
    }
    return res;
}
