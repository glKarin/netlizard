#ifndef _KARIN_NLCOMPONENTCONTAINER_H
#define _KARIN_NLCOMPONENTCONTAINER_H

#include "nlobjectcontainer.h"

class NLComponent;
class NLActor;

class NLLIB_EXPORT NLComponentContainer : public NLObjectContainer
{
    Q_OBJECT
public:
    explicit NLComponentContainer(NLActor *parent = 0);
    explicit NLComponentContainer(NLScene *scene, NLActor *parent = 0);
    virtual ~NLComponentContainer();
    bool Exists(const NLComponent &item) const;
    NLName Find(const NLComponent *item);
    bool Add(NLComponent *item);
    bool Remove(NLComponent *item);
    bool Remove(int index);
    bool Remove(const NLName &name);
    NLComponent * Get(const NLName &name);
    NLComponent * Get(int index);
    virtual void Clear();

    NLComponent * operator[](const NLName &name) { return Get(name); }
    NLComponent * operator[](int index) { return Get(index); }
    NLComponentContainer & operator<<(NLComponent *item) { Add(item); return *this; }
    NLComponentContainer & operator+(NLComponent *item) { Add(item); return *this; }
    NLComponentContainer & operator-(NLComponent *item) { Remove(item); return *this; }
    NLComponentContainer & operator-(int index) { Remove(index); return *this; }
    NLComponentContainer & operator-(const NLName &name) { Remove(name); return *this; }
    NLActor * Actor();
    const NLActor * Actor() const;
    bool ComponentIsAvailable(NLComponent *item) const;

signals:

public slots:

protected:
    virtual bool KeyEventHandler(int key, bool pressed, int modifier);
    virtual bool MouseEventHandler(int button, bool pressed, int x, int y, int modifier);
    virtual bool MouseMotionHandler(int button, bool pressed, int x, int y, int oldx, int oldy, int modifier);
    virtual bool WheelEventHandler(int mouse, int orientation, int delta, int x, int y, int modifier);
    void SetActor(NLActor *actor);
    virtual bool ItemIsAvailable(NLObject *item) const;

private:
    void Construct();

private:
    friend class NLActor;
    Q_DISABLE_COPY(NLComponentContainer)

};
#endif // _KARIN_NLCOMPONENTCONTAINER_H
