#ifndef _KARIN_NLFORCE_H
#define _KARIN_NLFORCE_H

#include "nlobject.h"
#include "nlphysics.h"

class NLRigidbody;
class NLForceContainer;

class NLForce : public NLObject
{
    Q_OBJECT
public:
    enum State
    {
        State_Ready = 0,
        State_Forcing,
        State_Finish
    };

public:
    explicit NLForce(NLRigidbody *parent = 0);
    explicit NLForce(const NLProperties &prop, NLRigidbody *parent = 0);
    explicit NLForce(NLScene *scene, NLRigidbody *parent = 0);
    explicit NLForce(NLScene *scene, const NLProperties &prop, NLRigidbody *parent = 0);
    virtual ~NLForce();
    NL::Physics::t Time() const;
    NL::Physics::F Force() const;
    NL::Physics::m Mass() const;
    bool IsForcing() const;
    bool IsFinished() const;
    bool IsStarted() const;
    State ForceState() const;
    virtual void Reset();
    virtual void Start();
    virtual void Finish();
    void SetRigidbody(NLRigidbody *o);
    NLRigidbody * Rigidbody();
    void SetOnce(bool b);
    bool Once() const;
    NLVector3 Direction() const;
    virtual bool IsActived() const;

signals:
    void started();
    void finished();

protected:
    virtual void Update(float delta);
    virtual void Destroy();
    void SetState(State s);
    virtual void InitProperty();
    void SetContainer(NLForceContainer *container);
    NL::Physics::a Acceleration() const;

protected:
    NL::Physics::F m_force;
    NL::Physics::t m_time;
    NLVector3 m_direction;

private:
    void Construct();

private:
    State m_state;
    bool m_once;

    friend class NLForceContainer;
    friend class NLRigidbody;

    Q_DISABLE_COPY(NLForce)
};

typedef QList<NLForce *> NLForceList;

class NLForce_gravity : public NLForce
{
public:
    explicit NLForce_gravity(NLRigidbody *parent = 0);
    explicit NLForce_gravity(const NLProperties &prop, NLRigidbody *parent = 0);
    explicit NLForce_gravity(NLScene *scene, NLRigidbody *parent = 0);
    explicit NLForce_gravity(NLScene *scene, const NLProperties &prop, NLRigidbody *parent = 0);
    virtual ~NLForce_gravity();
    virtual void Reset();
    NL::Physics::g Gravity() const;
    NL::Physics::d Distance() const;
    NL::Physics::v Speed() const;

protected:
    virtual void InitProperty();
    virtual void Update(float delta);

private:
    void Construct();

private:
    NL::Physics::g m_g;
    NL::Physics::v m_initialSpeed;
    NL::Physics::d m_distance;
    NL::Physics::v m_speed;
    NL::Physics::d m_lastDistance;
};

#endif // _KARIN_NLFORCE_H
