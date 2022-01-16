#ifndef _KARIN_NLRIGIDBODY_H
#define _KARIN_NLRIGIDBODY_H

#include "nlactor.h"
#include "nlphysics.h"
#include "nlforcecontainer.h"

class NLForce;
//class NLForceContainer;

class NLLIB_EXPORT NLRigidbody : public NLActor
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
    bool RemoveForce(const QString &name);
    NLForce * GetForce(const QString &name);
    NLForce * GetForce(int index);
    NLRigidbody & operator<<(NLForce *item) { AddForce(item); return *this; }
    NLRigidbody & operator+(NLForce *item) { AddForce(item); return *this; }
    NLRigidbody & operator-(NLForce *item) { RemoveForce(item); return *this; }
    template <class T>
    T * GetForce_T(const QString &name);
    template <class T>
    T * GetForce_T(int index);
    NLVector3 MoveDirection() const { return m_moveDirection; }
    virtual void SetRotation(const NLVector3 &v);
    virtual NLActor & Move(const NLVector3 &v);
    virtual NLActor & Turn(const NLVector3 &v);
    virtual void SetRotation(float x, float y, float z) { const NLVector3 v = VECTOR3(x, y, z); SetRotation(v); }
    virtual NLActor & Move(float x, float y, float z) { const NLVector3 v = VECTOR3(x, y, z); return Move(v); }
    virtual NLActor & Turn(float x, float y, float z) { const NLVector3 v = VECTOR3(x, y, z); return Turn(v); }
    void SetFree(bool b);
    bool Free() const { return m_free; }
    bool ZIsUp() const { return m_zIsUp; }
    bool FixedUp() const { return m_fixedUp; }
    void SetMass(int m);
    int ForceCount() const;
    bool HasForce() const { return ForceCount() > 0; }
    NLGETTER(mass) NL::Physics::m Mass() const { return m_mass; }
    NLSETTER(mass) void SetMass(NL::Physics::m m);
    NLActor & MoveSelfOriginal(const NLVector3 &v);
    NLINTERFACE void ClearAllForces();
    NLINTERFACE void Collision();
    virtual void Reset();
    //NLINTERFACE void CollisionFloor();

    template <class T>
    bool ForceIsType(int index) const;
    template <class T>
    bool ForceIsType(const QString &name) const;
    template <class T>
    int TypeForceCount() const;
    template <class T>
    bool HasTypeForce() const;
    template <class T>
    T * GetTypeForce(int index = 0);
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
T * NLRigidbody::GetForce_T(const QString &name)
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
bool NLRigidbody::ForceIsType(const QString &name) const
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
T * NLRigidbody::GetTypeForce(int index)
{
    if(!m_forces)
        return 0;
    return m_forces->GetType<T>(index);
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
