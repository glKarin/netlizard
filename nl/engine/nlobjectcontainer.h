#ifndef _KARIN_NLOBJECTCONTAINER_H
#define _KARIN_NLOBJECTCONTAINER_H

#include <QObject>

#include "nlobject.h"

class NLScene;

class NLLIB_EXPORT NLObjectContainer : public QObject
{
    Q_OBJECT
public:
    explicit NLObjectContainer(QObject *parent = 0);
    explicit NLObjectContainer(NLScene *scene, QObject *parent = 0);
    virtual ~NLObjectContainer();
    int Count() const { return m_objectList.count(); }
    bool IsEmpty() const { return m_objectList.isEmpty(); }
    bool Exists(const NLName &name) const;
    bool Exists(const NLObject *item) const;
    NLName Find(const NLObject *item);
    bool Add(NLObject *item);
    bool Remove(NLObject *item);
    bool Remove(int index);
    bool Remove(const NLName &name);
    NLObject * Get(const NLName &name);
    NLObject * Get(int index);
    template <class T>
    T * Get_T(const NLName &name);
    template <class T>
    T * Get_T(int index);
    virtual void Clear();
    virtual void Clean();
    virtual void Init();
    virtual void Destroy();
    virtual void Update(float delta);
    virtual void Reset();
    void SetScene(NLScene *scene);
    NLScene * Scene() { return m_scene; }
    bool ObjectIsAvailable(NLObject *item) const;

    NLObject * operator[](const NLName &name) { return Get(name); }
    NLObject * operator[](int index) { return Get(index); }
    NLObjectContainer & operator<<(NLObject *item) { Add(item); return *this; }
    NLObjectContainer & operator+(NLObject *item) { Add(item); return *this; }
    NLObjectContainer & operator-(NLObject *item) { Remove(item); return *this; }
    NLObjectContainer & operator-(int index) { Remove(index); return *this; }
    NLObjectContainer & operator-(const NLName &name) { Remove(name); return *this; }

    template <class T>
    bool IsType(int index) const;
    template <class T>
    bool IsType(const NLName &name) const;
    template <class T>
    int TypeCount() const;
    template <class T>
    bool HasType() const;
    template <class T>
    T * GetType(int index = 0);
    template <class T>
    QList<T *> GetTypes();
    template <class T>
    bool RemoveType();
    template <class T>
    int RemoveTypes();
    
signals:
    void objectAdded(NLObject *object);
    void objectRemoved(NLObject *object);
    void objectChanged(NLObject *object);
    
public slots:

protected:
    NLObjectList & ObjectList() { return m_objectList; }
    const NLObjectList & ObjectList() const { return m_objectList; }
    virtual bool ItemIsAvailable(NLObject *item) const { return ObjectIsAvailable(item); }

private:
    void Construct();
    bool Take(NLObject *item);

private:
    NLObjectList m_objectList;
    NLScene *m_scene;

    friend class NLObject;
    Q_DISABLE_COPY(NLObjectContainer)
};

template <class T>
T * NLObjectContainer::Get_T(const NLName &name)
{
    NLObject *obj = Get(name);
    if(!obj)
        return 0;
    return dynamic_cast<T *>(obj);
}

template <class T>
T * NLObjectContainer::Get_T(int index)
{
    NLObject *obj = Get(index);
    if(!obj)
        return 0;
    return dynamic_cast<T *>(obj);
}

template <class T>
bool NLObjectContainer::IsType(int index) const
{
    NLObject *obj = Get(index);
    return dynamic_cast<T *>(obj) != 0;
}

template <class T>
bool NLObjectContainer::IsType(const NLName &name) const
{
    NLObject *obj = Get(name);
    return dynamic_cast<T *>(obj) != 0;
}

template <class T>
int NLObjectContainer::TypeCount() const
{
    int c = 0;
    Q_FOREACH(NLObject *obj, m_objectList)
    {
        if(dynamic_cast<T *>(obj) != 0)
            c++;
    }
    return c;
}

template <class T>
bool NLObjectContainer::HasType() const
{
    Q_FOREACH(NLObject *obj, m_objectList)
    {
        if(dynamic_cast<T *>(obj) != 0)
            return true;
    }
    return false;
}

template <class T>
T * NLObjectContainer::GetType(int index)
{
    int i = 0;
    Q_FOREACH(NLObject *obj, m_objectList)
    {
        T *r = dynamic_cast<T *>(obj);
        if(r != 0)
        {
            if(i == index)
                return r;
            i++;
        }
    }
    return 0;
}

template <class T>
QList<T *> NLObjectContainer::GetTypes()
{
    QList<T *> list;
    Q_FOREACH(NLObject *obj, m_objectList)
    {
        T *r = dynamic_cast<T *>(obj);
        if(r != 0)
            list.push_back(r);
    }
    return list;
}

template <class T>
bool NLObjectContainer::RemoveType()
{
    T *r = GetType<T>();
    if(r)
        return Remove(r);
    return false;
}

template <class T>
int NLObjectContainer::RemoveTypes()
{
    QList<T *> list = GetTypes<T>();
    int c = 0;
    Q_FOREACH(T *obj, list)
    {
        if(Remove(obj))
            c++;
    }
    return c;
}

#endif // _KARIN_NLOBJECTCONTAINER_H
