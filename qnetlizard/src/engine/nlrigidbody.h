#ifndef _KARIN_NLRIGIDBODY_H
#define _KARIN_NLRIGIDBODY_H

#include "nlactor.h"

class NLForce;
class NLForceContainer;

class NLRigidbody : public NLActor
{
    Q_OBJECT
public:
    explicit NLRigidbody(NLActor *parent = 0);
    explicit NLRigidbody(const NLPropperties &prop, NLActor *parent = 0);
    explicit NLRigidbody(NLScene *scene, NLActor *parent = 0);
    explicit NLRigidbody(NLScene *scene, const NLPropperties &prop, NLActor *parent = 0);
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

protected:
    void SetZIsUp(bool b);
    void SetFixedUp(bool b);
    virtual void UpdateUp();
    virtual void InitProperty();

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

#endif // _KARIN_NLRIGIDBODY_H