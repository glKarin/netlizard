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
        if((static_cast<NLActor *>(obj))->keyev(key, pressed, modifier))
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
        if((static_cast<NLActor *>(obj))->mouseev(button, pressed, x, y, modifier))
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
        if((static_cast<NLActor *>(obj))->motionev(button, pressed, x, y, oldx, oldy, modifier))
            i++;
    }
    return i > 0;
}

bool NLActorContainer::WheelEventHandler(int orientation, int delta, int x, int y, int modifier)
{
    int i = 0;
    NLObjectList &list = ObjectList();
    Q_FOREACH(NLObject *obj, list)
    {
        if((static_cast<NLActor *>(obj))->wheelev(orientation, delta, x, y, modifier))
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
    return NLObjectContainer::Add(item);
}

bool NLActorContainer::Remove(NLActor *item)
{
    return NLObjectContainer::Remove(item);
}

NLActor * NLActorContainer::Get(const NLName &name)
{
    return static_cast<NLActor *>(NLObjectContainer::Get(name));
}

NLActor * NLActorContainer::Get(int index)
{
    return static_cast<NLActor *>(NLObjectContainer::Get(index));
}

NLActor * NLActorContainer::operator[](const NLName &name)
{
    return Get(name);
}

NLActor * NLActorContainer::operator[](int index)
{
    return Get(index);
}

NLActorContainer * NLActorContainer::operator<<(NLActor *item)
{
    Add(item);
    return this;
}

void NLActorContainer::Render()
{
    NLObjectList &list = ObjectList();
    Q_FOREACH(NLObject *obj, list)
    {
        (static_cast<NLActor *>(obj))->Render();
    }
}
