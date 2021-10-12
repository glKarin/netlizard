#ifndef _KARIN_NLCOMPONENTCONTAINER_H
#define _KARIN_NLCOMPONENTCONTAINER_H

#include "nlobjectcontainer.h"

class NLComponent;
class NLActor;

class NLComponentContainer : public NLObjectContainer
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
    NLComponent * Get(const NLName &name);
    NLComponent * Get(int index);
    virtual bool KeyEventHandler(int key, bool pressed, int modifier);
    virtual bool MouseEventHandler(int button, bool pressed, int x, int y, int modifier);
    virtual bool MouseMotionHandler(int button, bool pressed, int x, int y, int oldx, int oldy, int modifier);
    virtual bool WheelHandler(int orientation, int delta, int x, int y, int modifier);
    NLActor * Actor();
    void SetActor(NLActor *actor);

    NLComponent * operator[](const NLName &name);
    NLComponent * operator[](int index);
    NLComponentContainer * operator<<(NLComponent *item);

signals:

public slots:

private:
    void Construct();

private:
    friend class NLActor;
    Q_DISABLE_COPY(NLComponentContainer)

};
#endif // _KARIN_NLCOMPONENTCONTAINER_H
