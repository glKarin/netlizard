#include "nlscriptcontainer.h"

#include <QDebug>

#include "nldbg.h"
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

QString NLScriptContainer::Find(const NLScript *item)
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
    return res;
}

bool NLScriptContainer::Remove(int index)
{
    NLScript *item = Get(index);
    bool res = Remove(item);
    return res;
}

bool NLScriptContainer::Remove(const QString &name)
{
    NLScript *item = Get(name);
    bool res = Remove(item);
    return res;
}

NLScript * NLScriptContainer::Get(const QString &name)
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

const NLActor * NLScriptContainer::Actor() const
{
    const QObject *p = parent();
    if(p)
        return dynamic_cast<const NLActor *>(p);
    return 0;
}

void NLScriptContainer::SetActor(NLActor *actor)
{
    setParent(actor);
}

void NLScriptContainer::Clear()
{
    NLObjectContainer::Clear();
}

bool NLScriptContainer::ScriptIsAvailable(NLScript *item) const
{
    return ObjectIsAvailable(item) && item->Actor() == Actor();
}

bool NLScriptContainer::ItemIsAvailable(NLObject *item) const
{
    return ScriptIsAvailable(static_cast<NLScript *>(item));
}
