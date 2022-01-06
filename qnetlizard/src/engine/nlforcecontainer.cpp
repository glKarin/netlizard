#include "nlforcecontainer.h"

#include <QDebug>

#include "qdef.h"
#include "nlrigidbody.h"
#include "nlforce.h"

NLForceContainer::NLForceContainer(NLRigidbody *parent) :
    NLObjectContainer(parent)
{
    Construct();
}

NLForceContainer::NLForceContainer(NLScene *scene, NLRigidbody *parent) :
    NLObjectContainer(scene, parent)
{
    Construct();
}

NLForceContainer::~NLForceContainer()
{
}

void NLForceContainer::Construct()
{
    setObjectName("NLForceContainer");
}

NLName NLForceContainer::Find(const NLForce *item)
{
    return NLObjectContainer::Find(item);
}

bool NLForceContainer::Add(NLForce *item, bool start)
{
    bool res = NLObjectContainer::Add(item);
    if(res)
    {
        item->SetRigidbody(Rigidbody());
        if(start)
            item->Start();
    }
    return res;
}

bool NLForceContainer::Remove(NLForce *item)
{
    bool res = NLObjectContainer::Remove(item);
    if(res)
        item->Finish();
    return res;
}

bool NLForceContainer::Remove(int index)
{
    NLForce *item = Get(index);
    bool res = Remove(item);
    return res;
}

bool NLForceContainer::Remove(const NLName &name)
{
    NLForce *item = Get(name);
    bool res = Remove(item);
    return res;
}

NLForce * NLForceContainer::Get(const NLName &name)
{
    return static_cast<NLForce *>(NLObjectContainer::Get(name));
}

NLForce * NLForceContainer::Get(int index)
{
    return static_cast<NLForce *>(NLObjectContainer::Get(index));
}

NLRigidbody * NLForceContainer::Rigidbody()
{
    QObject *p = parent();
    if(p)
        return dynamic_cast<NLRigidbody *>(p);
    return 0;
}

const NLRigidbody * NLForceContainer::Rigidbody() const
{
    const QObject *p = parent();
    if(p)
        return dynamic_cast<const NLRigidbody *>(p);
    return 0;
}

void NLForceContainer::SetRigidbody(NLRigidbody *actor)
{
    setParent(actor);
}

void NLForceContainer::Clear()
{
    NLObjectList &list = ObjectList();
    Q_FOREACH(NLObject *obj, list)
    {
        NLForce *force = static_cast<NLForce *>(obj);
        if(ForceIsAvailable(force))
            force->Finish();
    }
    NLObjectContainer::Clear();
}

void NLForceContainer::ClearInvalid()
{
    NLObjectList &list = ObjectList();
    NLForceList rml;
    Q_FOREACH(NLObject *obj, list)
    {
        NLForce *force = static_cast<NLForce *>(obj);
        if(force->IsFinished() && force->Once())
            rml.push_back(force);
    }
    Q_FOREACH(NLForce *obj, rml)
    {
        Remove(obj);
        obj->Destroy();
        delete obj;
    }
}

void NLForceContainer::Update(float delta)
{
    NLObjectContainer::Update(delta);
    ClearInvalid();
}

bool NLForceContainer::ForceIsAvailable(NLForce *item) const
{
    return ObjectIsAvailable(item) && item->Rigidbody() == Rigidbody();
}

bool NLForceContainer::ItemIsAvailable(NLObject *item) const
{
    return ForceIsAvailable(static_cast<NLForce *>(item));
}
