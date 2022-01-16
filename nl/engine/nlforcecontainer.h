#ifndef _KARIN_NLFORCECONTAINER_H
#define _KARIN_NLFORCECONTAINER_H

#include "nlobjectcontainer.h"

class NLForce;
class NLRigidbody;

class NLLIB_EXPORT NLForceContainer : public NLObjectContainer
{
    Q_OBJECT
public:
    explicit NLForceContainer(NLRigidbody *parent = 0);
    explicit NLForceContainer(NLScene *scene, NLRigidbody *parent = 0);
    virtual ~NLForceContainer();
    bool Exists(const NLForce &item) const;
    QString Find(const NLForce *item);
    bool Add(NLForce *item, bool start = true);
    bool Remove(NLForce *item);
    bool Remove(int index);
    bool Remove(const QString &name);
    NLForce * Get(const QString &name);
    NLForce * Get(int index);
    virtual void Clear();
    void ClearInvalid();

    NLForce * operator[](const QString &name) { return Get(name); }
    NLForce * operator[](int index) { return Get(index); }
    NLForceContainer & operator<<(NLForce *item) { Add(item); return *this; }
    NLForceContainer & operator+(NLForce *item) { Add(item); return *this; }
    NLForceContainer & operator-(NLForce *item) { Remove(item); return *this; }
    NLForceContainer & operator-(int index) { Remove(index); return *this; }
    NLForceContainer & operator-(const QString &name) { Remove(name); return *this; }
    NLRigidbody * Rigidbody();
    const NLRigidbody * Rigidbody() const;
    bool ForceIsAvailable(NLForce *item) const;

protected:
    virtual void Update(float delta);

signals:

public slots:

protected:
    void SetRigidbody(NLRigidbody *actor);
    virtual bool ItemIsAvailable(NLObject *item) const;

private:
    void Construct();

private:
    friend class NLRigidbody;
    Q_DISABLE_COPY(NLForceContainer)

};
#endif // _KARIN_NLFORCECONTAINER_H
