#include "nlobjectpool.h"

#include <QDebug>
#include <limits>

#include "nldbg.h"

NLObjectPool::NLObjectPool(QObject *parent)
    : QObject(parent)
{
    setObjectName("NLObjectPool");
}

NLObjectPool::~NLObjectPool()
{
    NLDEBUG_DESTROY_Q
}

NLName NLObjectPool::GenName(const NLObject *item)
{
typedef quint64 index_t;
#define INDEX_MAX std::numeric_limits<index_t>::max()
    NLName name;
    static index_t _i = 0;
    static char _n = '0';
    static char _c = 'a';
    static char _C = 'A';
    static index_t _I = 0;
    if(_i >= INDEX_MAX)
    {
        _i = 0;
        _n++;
    }
    if(_n > '9')
    {
        _n = '0';
        _c++;
    }
    if(_c > 'z')
    {
        _c = 'a';
        _C++;
    }
    if(_C > 'Z')
    {
        _C = 'A';
        _I++;
    }
    do
    {
        name = item->objectName()
                + "_"
                + QString::number(_I)
                + _C
                + _c
                + _n
                + "_"
                + QString::number(_i++)
                ;
    }
    while(m_namePool.contains(name));
    m_namePool.push_back(name);
    return name;
}

NLObject * NLObjectPool::Get(const NLName &name)
{
    if(!m_pool.contains(name))
        return 0;
    return m_pool[name];
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
    m_itemPool.push_back(item);
    return name;
}

NLObjectPool * NLObjectPool::Detach(NLObject *item)
{
    if(!item)
        return this;
    Q_FOREACH(const NLName &name, m_pool.keys())
    {
        if(m_pool.value(name) == item)
        {
            m_pool.remove(name);
            m_itemPool.removeOne(item);
            break;
        }
    }
    qDebug() << "[NLObjectPool]: Current NLObject count: " << m_pool.size();
    return this;
}

NLObject * NLObjectPool::Detach(const NLName &name)
{
    NLObject *res = 0;
    if(m_pool.contains(name))
    {
        res = m_pool[name];
        m_pool.remove(name);
        m_itemPool.removeOne(res);
    }
    return res;
}

NLObjectPool * NLObjectPool::Instance()
{
    static NLObjectPool _instance;
    return &_instance;
}

void NLObjectPool::Reset()
{
    Clear();
    m_namePool.clear();
}
