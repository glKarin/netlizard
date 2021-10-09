#include "nlobjectpool.h"

#include <QDebug>

#include "qdef.h"

NLObjectPool::NLObjectPool(QObject *parent)
    : QObject(parent)
{
    setObjectName("NLObjectPool");
}

NLObjectPool::~NLObjectPool()
{
    DEBUG_DESTROY_Q
}

NLName NLObjectPool::GenName(const NLObject *item)
{
    NLName name;
    int i = 0;
    do
    {
        name = item->objectName() + "_" + QString::number(i++);
    }
    while(m_namePool.contains(name));
    m_namePool.push_back(name);
    return name;
}

int NLObjectPool::Count() const
{
    return m_pool.count();
}

bool NLObjectPool::Exists(const NLName &name) const
{
    return m_pool.contains(name);
}

bool NLObjectPool::Exists(const NLObject *item) const
{
    Q_FOREACH(const NLName &name, m_pool.keys())
    {
        if(m_pool.value(name) == item)
            return true;
    }
    return false;
}

bool NLObjectPool::IsEmpty() const
{
    return m_pool.isEmpty();
}

NLObject * NLObjectPool::Get(const NLName &name)
{
    if(!m_pool.contains(name))
        return 0;
    return m_pool[name];
}

NLObject * NLObjectPool::operator[](const NLName &name)
{
    return Get(name);
}

NLName NLObjectPool::Find(const NLObject *item)
{
    if(!item)
        return QString();

    Q_FOREACH(const NLName &name, m_pool.keys())
    {
        if(m_pool.value(name) == item)
            return name;
    }
    return QString();
}

NLName NLObjectPool::Attach(NLObject *item)
{
    if(!item)
        return NLName();
    if(!Find(item).isEmpty())
        return NLName();
    NLName name = GenName(item);
    m_pool.insert(name, item);
    return name;
}

NLObjectPool * NLObjectPool::operator<<(NLObject *item)
{
    Attach(item);
    return this;
}

NLObjectPool * NLObjectPool::Detach(NLObject *item)
{
    if(!item)
        return this;
    QString name = Find(item);
    if(!name.isEmpty())
        m_pool.remove(name);
    return this;
}

NLObject * NLObjectPool::Detach(const NLName &name)
{
    NLObject *res = 0;
    if(m_pool.contains(name))
    {
        res = m_pool[name];
        m_pool.remove(name);
    }
    return res;
}

void NLObjectPool::Clear()
{
    m_pool.clear();
}

NLObjectPool * NLObjectPool::Instance()
{
    static NLObjectPool _instance;
    return &_instance;
}
