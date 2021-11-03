#ifndef _KARIN_NLFORCECONTAINER_H
#define _KARIN_NLFORCECONTAINER_H

#include "nlobjectcontainer.h"

class NLForce;
class NLRigidbody;

class NLForceContainer : public NLObjectContainer
{
    Q_OBJECT
public:
    explicit NLForceContainer(NLRigidbody *parent = 0);
    explicit NLForceContainer(NLScene *scene, NLRigidbody *parent = 0);
    virtual ~NLForceContainer();
    bool Exists(const NLForce &item) const;
    NLName Find(const NLForce *item);
    bool Add(NLForce *item, bool start = true);
    bool Remove(NLForce *item);
    bool Remove(int index);
    bool Remove(const NLName &name);
    NLForce * Get(const NLName &name);
    NLForce * Get(int index);
    virtual void Clear();
    void ClearInvalid();

    NLForce * operator[](const NLName &name);
    NLForce * operator[](int index);
    NLForceContainer * operator<<(NLForce *item);
    NLRigidbody * Rigidbody();

protected:
    virtual void Update(float delta);

signals:

public slots:

protected:
    void SetRigidbody(NLRigidbody *actor);

private:
    void Construct();

private:
    friend class NLRigidbody;
    Q_DISABLE_COPY(NLForceContainer)

};
#endif // _KARIN_NLFORCECONTAINER_H
