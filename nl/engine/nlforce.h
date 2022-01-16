#ifndef _KARIN_NLFORCE_H
#define _KARIN_NLFORCE_H

#include "nlobject.h"
#include "nlphysics.h"

class NLRigidbody;
class NLForceContainer;

class NLLIB_EXPORT NLForce : public NLObject
{
    Q_OBJECT
    Q_PROPERTY(float force READ Force FINAL)
    Q_PROPERTY(NLVector3 direction READ Direction FINAL)
    Q_PROPERTY(bool once READ Once WRITE SetOnce FINAL)
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
    NL::Physics::t Time() const { return m_time; }
    NL::Physics::F Force() const { return m_force; }
    NL::Physics::m Mass() const;
    bool IsForcing() const { return m_state == NLForce::State_Forcing; }
    bool IsFinished() const { return m_state == NLForce::State_Finish; }
    bool IsStarted() const { return m_state != NLForce::State_Ready; }
    State ForceState() const { return m_state; }
    virtual void Reset();
    virtual void Start();
    virtual void Finish();
    void SetRigidbody(NLRigidbody *o);
    NLRigidbody * Rigidbody();
    void SetOnce(bool b);
    bool Once() const { return m_once; }
    NLVector3 Direction() const { return m_direction; }
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
    NL::Physics::a Acceleration(NL::Physics::F force) const;

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

class NLLIB_EXPORT NLForce_gravity : public NLForce
{
public:
    explicit NLForce_gravity(NLRigidbody *parent = 0);
    explicit NLForce_gravity(const NLProperties &prop, NLRigidbody *parent = 0);
    explicit NLForce_gravity(NLScene *scene, NLRigidbody *parent = 0);
    explicit NLForce_gravity(NLScene *scene, const NLProperties &prop, NLRigidbody *parent = 0);
    virtual ~NLForce_gravity();
    virtual void Reset();
    NL::Physics::g Gravity() const { return m_g; }
    NL::Physics::d Distance() const { return m_distance; }
    NL::Physics::v Speed() const { return m_speed; }

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

class NLLIB_EXPORT NLForce_push : public NLForce
{
public:
    explicit NLForce_push(NLRigidbody *parent = 0);
    explicit NLForce_push(const NLProperties &prop, NLRigidbody *parent = 0);
    explicit NLForce_push(NLScene *scene, NLRigidbody *parent = 0);
    explicit NLForce_push(NLScene *scene, const NLProperties &prop, NLRigidbody *parent = 0);
    virtual ~NLForce_push();
    virtual void Reset();
    NL::Physics::F FragForce() const { return m_dragForce; }
    NL::Physics::d Distance() const { return m_distance; }
    NL::Physics::v Speed() const { return m_speed; }

protected:
    virtual void InitProperty();
    virtual void Update(float delta);

private:
    void Construct();

private:
    NL::Physics::g m_dragForce;
    NL::Physics::v m_initialSpeed;
    NL::Physics::d m_distance;
    NL::Physics::v m_speed;
    NL::Physics::d m_lastDistance;
    NL::Physics::a m_acceleration;
};

#endif // _KARIN_NLFORCE_H
