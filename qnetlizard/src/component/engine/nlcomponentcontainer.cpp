#include "nlcomponentcontainer.h"

#include <QDebug>

#include "qdef.h"
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
        if((static_cast<NLComponent *>(obj))->keyev(key, pressed, modifier))
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
        if((static_cast<NLComponent *>(obj))->mouseev(button, pressed, x, y, modifier))
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
        if((static_cast<NLComponent *>(obj))->motionev(button, pressed, x, y, oldx, oldy, modifier))
            i++;
    }
    return i > 0;
}

bool NLComponentContainer::WheelHandler(int orientation, int delta, int x, int y, int modifier)
{
    int i = 0;
    NLObjectList &list = ObjectList();
    Q_FOREACH(NLObject *obj, list)
    {
        if((static_cast<NLComponent *>(obj))->wheelev(orientation, delta, x, y, modifier))
            i++;
    }
    return i > 0;
}

NLName NLComponentContainer::Find(const NLComponent *item)
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
    if(res)
        item->Unmount();
    return res;
}

NLComponent * NLComponentContainer::Get(const NLName &name)
{
    return static_cast<NLComponent *>(NLObjectContainer::Get(name));
}

NLComponent * NLComponentContainer::Get(int index)
{
    return static_cast<NLComponent *>(NLObjectContainer::Get(index));
}

NLComponent * NLComponentContainer::operator[](const NLName &name)
{
    return Get(name);
}

NLComponent * NLComponentContainer::operator[](int index)
{
    return Get(index);
}

NLComponentContainer * NLComponentContainer::operator<<(NLComponent *item)
{
    Add(item);
    return this;
}

NLActor * NLComponentContainer::Actor()
{
    QObject *p = parent();
    if(p)
        return dynamic_cast<NLActor *>(p);
    return 0;
}

void NLComponentContainer::SetActor(NLActor *actor)
{
    setParent(actor);
}
