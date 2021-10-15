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

int NLObjectContainer::Count() const
{
    return m_objectList.count();
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

bool NLObjectContainer::IsEmpty() const
{
    return m_objectList.isEmpty();
}

NLName NLObjectContainer::Find(const NLObject *item)
{
    if(!item)
        return false;
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
    return true;
}

NLObjectContainer * NLObjectContainer::operator<<(NLObject *item)
{
    Add(item);
    return this;
}

bool NLObjectContainer::Remove(NLObject *item)
{
    if(!Exists(item))
        return false;
    m_objectList.removeOne(item);
    item->SetContainer(0);
    item->SetScene(0);
    return true;
}

bool NLObjectContainer::Remove(int index)
{
    NLObject *item = Get(index);
    if(!item)
        return false;
    m_objectList.removeOne(item);
    item->SetContainer(0);
    item->SetScene(0);
    return true;
}

bool NLObjectContainer::Remove(const NLName &name)
{
    NLObject *item = Get(name);
    if(!item)
        return false;
    m_objectList.removeOne(item);
    item->SetContainer(0);
    item->SetScene(0);
    return true;
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

NLObject * NLObjectContainer::operator[](const NLName &name)
{
    return Get(name);
}

NLObject * NLObjectContainer::Get(int index)
{
    if(index < 0 || index >= m_objectList.count())
        return 0;
    return m_objectList[index];
}

NLObject * NLObjectContainer::operator[](int index)
{
    return Get(index);
}

void NLObjectContainer::Clear()
{
    Q_FOREACH(NLObject *obj, m_objectList)
    {
        obj->SetContainer(0);
        obj->SetScene(0);
    }
    m_objectList.clear();
}

NLObjectList & NLObjectContainer::ObjectList()
{
    return m_objectList;
}

NLScene * NLObjectContainer::Scene()
{
    return m_scene;
}

void NLObjectContainer::SetScene(NLScene *scene)
{
    m_scene = scene;
    Q_FOREACH(NLObject *obj, m_objectList)
    {
        obj->SetScene(scene);
    }
}
