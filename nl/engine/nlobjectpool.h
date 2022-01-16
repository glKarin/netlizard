#ifndef _KARIN_NLOBJECTPOOL_H
#define _KARIN_NLOBJECTPOOL_H

#include <QObject>
#include <QHash>

#include "nlobject.h"

class NLLIB_EXPORT NLObjectPool : public QObject
{
    Q_OBJECT
public:
    virtual ~NLObjectPool();
    static NLObjectPool * Instance();
    int Count() const { return m_pool.count(); }
    bool Exists(const QString &name) const { return m_pool.contains(name); }
    bool Exists(const NLObject *item) const { return m_itemPool.contains(item); }
    bool IsEmpty() const { return m_pool.isEmpty(); }
    QString Find(const NLObject *item);
    QString Attach(NLObject *item);
    NLObjectPool * Detach(NLObject *item);
    NLObject * Detach(const QString &name);
    NLObject * Get(const QString &name);
    void Clear() { m_pool.clear(); m_itemPool.clear();}
    void Reset();

    NLObject * operator[](const QString &name) { return Get(name); }
    NLObjectPool & operator<<(NLObject *item) { Attach(item); return *this; }
    NLObjectPool & operator+(NLObject *item) { Attach(item); return *this; }
    NLObjectPool & operator-(NLObject *item) { Detach(item); return *this; }
    NLObjectPool & operator-(const QString &name) { Detach(name); return *this; }

private:
    QString GenName(const NLObject *item);

private:
    typedef QHash<QString, NLObject *> ObjectPool;
    typedef QList<QString> NamePool;
    typedef QList<const NLObject *> ItemPool;
    NLObjectPool(QObject *parent = 0);
    ObjectPool m_pool;
    NamePool m_namePool;
    ItemPool m_itemPool;

    Q_DISABLE_COPY(NLObjectPool)
};

#endif // _KARIN_NLObjectPool_H
