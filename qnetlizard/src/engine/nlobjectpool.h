#ifndef _KARIN_NLOBJECTPOOL_H
#define _KARIN_NLOBJECTPOOL_H

#include <QObject>
#include <QHash>

#include "nlobject.h"

class NLObjectPool : public QObject
{
    Q_OBJECT
public:
    virtual ~NLObjectPool();
    static NLObjectPool * Instance();
    int Count() const { return m_pool.count(); }
    bool Exists(const NLName &name) const { return m_pool.contains(name); }
    bool Exists(const NLObject *item) const;
    bool IsEmpty() const { return m_pool.isEmpty(); }
    NLName Find(const NLObject *item);
    NLName Attach(NLObject *item);
    NLObjectPool * Detach(NLObject *item);
    NLObject * Detach(const NLName &name);
    NLObject * Get(const NLName &name);
    void Clear() { m_pool.clear(); }
    void Reset();

    NLObject * operator[](const NLName &name) { return Get(name); }
    NLObjectPool & operator<<(NLObject *item) { Attach(item); return *this; }

private:
    NLName GenName(const NLObject *item);

private:
    typedef QHash<NLName, NLObject *> ObjectPool;
    typedef QList<NLName> NamePool;
    NLObjectPool(QObject *parent = 0);
    ObjectPool m_pool;
    NamePool m_namePool;

    Q_DISABLE_COPY(NLObjectPool)
};

#endif // _KARIN_NLObjectPool_H
