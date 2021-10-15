#ifndef _KARIN_NLOBJECTCONTAINER_H
#define _KARIN_NLOBJECTCONTAINER_H

#include <QObject>

#include "nlobject.h"

class NLScene;

class NLObjectContainer : public QObject
{
    Q_OBJECT
public:
    explicit NLObjectContainer(QObject *parent = 0);
    explicit NLObjectContainer(NLScene *scene, QObject *parent = 0);
    virtual ~NLObjectContainer();
    int Count() const;
    bool IsEmpty() const;
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
    virtual void Init();
    virtual void Destroy();
    virtual void Update(float delta);
    virtual void Reset();
    void SetScene(NLScene *scene);
    NLScene * Scene();

    NLObject * operator[](const NLName &name);
    NLObject * operator[](int index);
    NLObjectContainer * operator<<(NLObject *item);
    
signals:
    
public slots:

protected:
    NLObjectList & ObjectList();

private:
    void Construct();

private:
    NLObjectList m_objectList;
    NLScene *m_scene;

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

#endif // _KARIN_NLOBJECTCONTAINER_H
