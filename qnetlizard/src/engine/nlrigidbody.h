#ifndef _KARIN_NLRIGIDBODY_H
#define _KARIN_NLRIGIDBODY_H

#include "nlactor.h"
#include "nlphysics.h"
#include "nlforcecontainer.h"

class NLForce;
//class NLForceContainer;

class NLRigidbody : public NLActor
{
    Q_OBJECT
    Q_PROPERTY(bool free READ Free WRITE SetFree FINAL)
    Q_PROPERTY(bool z_is_up READ ZIsUp WRITE SetZIsUp FINAL)
    Q_PROPERTY(bool fixed_up READ FixedUp WRITE SetFixedUp FINAL)
    Q_PROPERTY(float mass READ Mass WRITE SetMass FINAL)
public:
    explicit NLRigidbody(NLActor *parent = 0);
    explicit NLRigidbody(const NLProperties &prop, NLActor *parent = 0);
    explicit NLRigidbody(NLScene *scene, NLActor *parent = 0);
    explicit NLRigidbody(NLScene *scene, const NLProperties &prop, NLActor *parent = 0);
    virtual ~NLRigidbody();
    bool AddForce(NLForce *actor);
    bool RemoveForce(NLForce *actor);
    bool RemoveForce(int index);
    bool RemoveForce(const NLName &name);
    NLForce * GetForce(const NLName &name);
    NLForce * GetForce(int index);
    friend NLRigidbody & operator+(NLRigidbody &actor, NLForce *item);
    friend NLRigidbody & operator-(NLRigidbody &actor, NLForce *item);
    template <class T>
    T * GetForce_T(const NLName &name);
    template <class T>
    T * GetForce_T(int index);
    NLVector3 MoveDirection() const;
    virtual void SetRotation(const NLVector3 &v);
    virtual NLActor * Move(const NLVector3 &v);
    virtual NLActor * Turn(const NLVector3 &v);
    void SetFree(bool b);
    bool Free() const;
    bool ZIsUp() const;
    bool FixedUp() const;
    void SetMass(int m);
    int ForceCount() const;
    bool HasForce() const;
    NLGETTER(mass) NL::Physics::m Mass() const;
    NLSETTER(mass) void SetMass(NL::Physics::m m);
    NLActor * MoveSelfOriginal(const NLVector3 &v);
    NLINTERFACE void ClearAllForces();
    NLINTERFACE void Collision();
    //NLINTERFACE void CollisionFloor();

    template <class T>
    bool ForceIsType(int index) const;
    template <class T>
    bool ForceIsType(const NLName &name) const;
    template <class T>
    int TypeForceCount() const;
    template <class T>
    bool HasTypeForce() const;
    template <class T>
    T * GetTypeForce();
    template <class T>
    QList<T *> GetTypeForces();
    template <class T>
    bool RemoveTypeForce();
    template <class T>
    int RemoveTypeForces();

Q_SIGNALS:
    void forceChanged(const NLForce *force = 0);

protected:
    void SetZIsUp(bool b);
    void SetFixedUp(bool b);
    virtual void UpdateUp();
    virtual void InitProperty();
    virtual void Init();
    virtual void Destroy();
    virtual void Update(float delta);

private:
    void Construct();
    void UpdateMoveMatrix();

private:
    bool m_zIsUp;
    bool m_fixedUp;
    NLPROPERTY(bool, free) bool m_free;
    NLMatrix4 m_moveMatrix;
    NLVector3 m_moveRotation;
    NLVector3 m_moveDirection;
    NLPROPERTY(float, mess) NL::Physics::m m_mass;
    NLForceContainer *m_forces;

    Q_DISABLE_COPY(NLRigidbody)
};

template <class T>
T * NLRigidbody::GetForce_T(const NLName &name)
{
    NLForce *obj = GetForce(name);
    if(!obj)
        return 0;
    return dynamic_cast<T *>(obj);
}

template <class T>
T * NLRigidbody::GetForce_T(int index)
{
    NLForce *obj = GetForce(index);
    if(!obj)
        return 0;
    return dynamic_cast<T *>(obj);
}

template <class T>
bool NLRigidbody::ForceIsType(int index) const
{
    if(!m_forces)
        return false;
    return m_forces->IsType<T>(index);
}

template <class T>
bool NLRigidbody::ForceIsType(const NLName &name) const
{
    if(!m_forces)
        return false;
    return m_forces->IsType<T>(name);
}

template <class T>
int NLRigidbody::TypeForceCount() const
{
    if(!m_forces)
        return 0;
    return m_forces->TypeCount<T>();
}

template <class T>
bool NLRigidbody::HasTypeForce() const
{
    if(!m_forces)
        return false;
    return m_forces->HasType<T>();
}

template <class T>
T * NLRigidbody::GetTypeForce()
{
    if(!m_forces)
        return 0;
    return m_forces->GetType<T>();
}

template <class T>
QList<T *> NLRigidbody::GetTypeForces()
{
    if(!m_forces)
        return 0;
    return m_forces->GetTypes<T>();
}

template <class T>
bool NLRigidbody::RemoveTypeForce()
{
    if(!m_forces)
        return false;
    T *r = m_forces->GetType<T>();
    if(!r)
        return false;
    return RemoveForce(r);
}

template <class T>
int NLRigidbody::RemoveTypeForces()
{
    if(!m_forces)
        return 0;
    QList<T *> list = m_forces->GetTypes<T>();
    int c = 0;
    Q_FOREACH(T *obj, list)
    {
        if(RemoveForce(obj))
            c++;
    }
    return c;
}

#endif // _KARIN_NLRIGIDBODY_H
