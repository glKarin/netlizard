#ifndef _KARIN_NLRIGIDBODY_H
#define _KARIN_NLRIGIDBODY_H

#include "nlactor.h"

class NLRigidbody : public NLActor
{
    Q_OBJECT
public:
    explicit NLRigidbody(NLActor *parent = 0);
    explicit NLRigidbody(const NLPropperties &prop, NLActor *parent = 0);
    explicit NLRigidbody(NLScene *scene, NLActor *parent = 0);
    explicit NLRigidbody(NLScene *scene, const NLPropperties &prop, NLActor *parent = 0);
    virtual ~NLRigidbody();
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

    Q_DISABLE_COPY(NLRigidbody)
};

#endif // _KARIN_NLRIGIDBODY_H
