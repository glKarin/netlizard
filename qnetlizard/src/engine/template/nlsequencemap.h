#ifndef _KARIN_NLSEQUENCEMAP_H
#define _KARIN_NLSEQUENCEMAP_H

#include <QHash>
#include <QMap>
#include <QList>

#define SEQUENCEMAP(X, P) \
    template <class K, class V > \
    class X : public P<K, V > \
{ \
    public: \
    typename P<K, V >::iterator insert(const K &key, const V &value) \
    { \
        typename P<K, V >::iterator itor = P<K, V >::insert(key, value); \
        if(!m_sequenceKeys.contains(key)) \
            m_sequenceKeys.push_back(key); \
        return itor; \
    } \
    typename P<K, V >::iterator insertMulti(const K &key, const V &value) \
    { \
        typename P<K, V >::iterator itor = P<K, V>::insertMulti(key, value); \
        if(!m_sequenceKeys.contains(key)) \
            m_sequenceKeys.push_back(key); \
        return itor; \
    } \
    typename P<K, V >::iterator erase(typename P<K, V >::iterator pos) \
    { \
        QString key(pos.key()); \
        typename P<K, V >::iterator itor = P<K, V>::erase(pos); \
        if(!m_sequenceKeys.contains(key)) \
            m_sequenceKeys.push_back(key); \
        return itor; \
    } \
    V take(const K &key) \
    { \
        V res = P<K, V>::take(key); \
        if(m_sequenceKeys.contains(key)) \
            m_sequenceKeys.removeOne(key); \
        return res; \
    } \
    int	remove(const K &key) \
    { \
        int res = P<K, V>::remove(key); \
        if(m_sequenceKeys.contains(key)) \
            m_sequenceKeys.removeOne(key); \
        return res; \
    } \
    V &	operator[](const K &key) \
    { \
        V &res = P<K, V>::operator[](key); \
        if(!m_sequenceKeys.contains(key)) \
            m_sequenceKeys.push_back(key); \
        return res; \
    } \
    const V	operator[](const K &key) const \
    { \
        return P<K, V>::value(key); \
    } \
    bool operator!=(const X<K, V> &other) const \
    { \
        return P<K, V>::operator!=(other) && m_sequenceKeys != other.m_sequenceKeys; \
    } \
    bool operator==(const X<K, V> &other) const \
    { \
        return P<K, V>::operator==(other) && m_sequenceKeys == other.m_sequenceKeys; \
    } \
    void clear() \
    { \
        P<K, V>::clear(); \
        m_sequenceKeys.clear(); \
    } \
    QList<K> SequenceKeys() const { \
        return m_sequenceKeys; \
    } \
    void SortSequenceKeys() const { \
        qSort(m_sequenceKeys); \
    } \
    template <class LessThan> \
    void SortSequenceKeys(LessThan lessThan) const { \
        qSort(m_sequenceKeys.begin(), m_sequenceKeys.end(), lessThan); \
    } \
    private: \
    QList<K> m_sequenceKeys; \
};

SEQUENCEMAP(NLSequenceHash, QHash)
SEQUENCEMAP(NLSequenceMap, QMap)


#endif // _KARIN_NLSEQUENCEMAP_H
