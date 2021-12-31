#include "nlscriptcontainer.h"

#include <QDebug>

#include "qdef.h"
#include "nlactor.h"
#include "nlscript.h"

NLScriptContainer::NLScriptContainer(NLActor *parent) :
    NLObjectContainer(parent)
{
    Construct();
}

NLScriptContainer::NLScriptContainer(NLScene *scene, NLActor *parent) :
    NLObjectContainer(scene, parent)
{
    Construct();
}

NLScriptContainer::~NLScriptContainer()
{
}

void NLScriptContainer::Construct()
{
    setObjectName("NLScriptContainer");
}

NLName NLScriptContainer::Find(const NLScript *item)
{
    return NLObjectContainer::Find(item);
}

bool NLScriptContainer::Add(NLScript *item)
{
    bool res = NLObjectContainer::Add(item);
    if(res)
        item->Mount(Actor());
    return res;
}

bool NLScriptContainer::Remove(NLScript *item)
{
    bool res = NLObjectContainer::Remove(item);
    if(res)
        item->Unmount();
    return res;
}

bool NLScriptContainer::Remove(int index)
{
    NLScript *item = Get(index);
    if(!item)
        return false;
    bool res = NLObjectContainer::Remove(item);
    if(res)
        item->Unmount();
    return res;
}

bool NLScriptContainer::Remove(const NLName &name)
{
    NLScript *item = Get(name);
    if(!item)
        return false;
    bool res = NLObjectContainer::Remove(item);
    if(res)
        item->Unmount();
    return res;
}

NLScript * NLScriptContainer::Get(const NLName &name)
{
    return static_cast<NLScript *>(NLObjectContainer::Get(name));
}

NLScript * NLScriptContainer::Get(int index)
{
    return static_cast<NLScript *>(NLObjectContainer::Get(index));
}

NLActor * NLScriptContainer::Actor()
{
    QObject *p = parent();
    if(p)
        return dynamic_cast<NLActor *>(p);
    return 0;
}

void NLScriptContainer::SetActor(NLActor *actor)
{
    setParent(actor);
}

void NLScriptContainer::Clear()
{
    NLObjectList &list = ObjectList();
    Q_FOREACH(NLObject *obj, list)
    {
        (static_cast<NLScript *>(obj))->Unmount();
    }
    NLObjectContainer::Clear();
}
