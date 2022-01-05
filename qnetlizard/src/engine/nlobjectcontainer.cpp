#include "nlobjectcontainer.h"

#include <QDebug>

#include "qdef.h"
#include "nlobjectpool.h"

NLObjectContainer::NLObjectContainer(QObject *parent) :
    QObject(parent),
    m_scene(0)
{
    Construct();
}

NLObjectContainer::NLObjectContainer(NLScene *scene, QObject *parent) :
    QObject(parent),
    m_scene(scene)
{
    Construct();
}

NLObjectContainer::~NLObjectContainer()
{
    Destroy();
    DEBUG_DESTROY_Q;
}

void NLObjectContainer::Construct()
{
    setObjectName("NLObjectContainer");
}

void NLObjectContainer::Init()
{
    Q_FOREACH(NLObject *obj, m_objectList)
    {
        obj->Init();
    }
}

void NLObjectContainer::Destroy()
{
    Q_FOREACH(NLObject *obj, m_objectList)
    {
        obj->Destroy();
        delete obj;
    }
    m_objectList.clear();
}

void NLObjectContainer::Update(float delta)
{
    Q_FOREACH(NLObject *obj, m_objectList)
    {
        obj->Update(delta);
    }
}

void NLObjectContainer::Reset()
{
    Q_FOREACH(NLObject *obj, m_objectList)
    {
        obj->Reset();
    }
}

bool NLObjectContainer::Exists(const NLName &name) const
{
    Q_FOREACH(NLObject *obj, m_objectList)
    {
        if(obj->Name() == name)
            return true;
    }
    return false;
}

bool NLObjectContainer::Exists(const NLObject *item) const
{
    if(!item)
        return false;
    return m_objectList.contains((NLObject *)item);
}

NLName NLObjectContainer::Find(const NLObject *item)
{
    if(!item)
        return QString();
    if(!m_objectList.contains((NLObject *)item))
        return QString();
    return item->Name();
}

bool NLObjectContainer::Add(NLObject *item)
{
    if(!item)
        return false;
    if(item->Container())
        return false;
    if(Exists(item))
        return false;
    item->SetContainer(this);
    item->SetScene(m_scene);
    item->Init();
    m_objectList.push_back(item);
    emit objectAdded(item);
    emit objectChanged(item);
    return true;
}

bool NLObjectContainer::Take(NLObject *item)
{
    if(!Exists(item))
        return false;
    m_objectList.removeOne(item);
    emit objectRemoved(item);
    emit objectChanged(item);
    return true;
}

bool NLObjectContainer::Remove(NLObject *item)
{
    if(!Exists(item))
        return false;
    m_objectList.removeOne(item);
    emit objectRemoved(item);
    emit objectChanged(item);
    item->Destroy();
    return true;
}

bool NLObjectContainer::Remove(int index)
{
    NLObject *item = Get(index);
    return Remove(item);
}

bool NLObjectContainer::Remove(const NLName &name)
{
    NLObject *item = Get(name);
    return Remove(item);
}

NLObject * NLObjectContainer::Get(const NLName &name)
{
    Q_FOREACH(NLObject *obj, m_objectList)
    {
        if(obj->Name() == name)
            return obj;
    }
    return 0;
}

NLObject * NLObjectContainer::Get(int index)
{
    int i = index < 0 ? m_objectList.count() + index : index;
    if(i < 0 || i >= m_objectList.count())
        return 0;
    return m_objectList[i];
}

void NLObjectContainer::Clear()
{
    while(!m_objectList.isEmpty())
    {
        NLObject *obj = m_objectList.takeLast();
        obj->Destroy();
    }
}

void NLObjectContainer::Clean()
{
    while(!m_objectList.isEmpty())
    {
        NLObject *obj = m_objectList.takeLast();
        obj->Destroy();
        delete obj;
    }
}

void NLObjectContainer::SetScene(NLScene *scene)
{
    m_scene = scene;
    Q_FOREACH(NLObject *obj, m_objectList)
    {
        obj->SetScene(scene);
    }
}
