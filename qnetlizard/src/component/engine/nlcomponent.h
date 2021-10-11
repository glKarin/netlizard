#ifndef _KARIN_NLCOMPONENT_H
#define _KARIN_NLCOMPONENT_H

#include "nlobject.h"

class NLComponentContainer;
class NLActor;

class NLComponent : public NLObject
{
    Q_OBJECT
public:
    explicit NLComponent(NLActor *parent = 0);
    explicit NLComponent(const NLPropperties &prop, NLActor *parent = 0);
    explicit NLComponent(NLScene *scene, NLActor *parent = 0);
    explicit NLComponent(NLScene *scene, const NLPropperties &prop, NLActor *parent = 0);
    virtual ~NLComponent();
    bool IsMounted() const;
    NLActor * Actor();
    void SetActor(NLActor *actor);
    NLComponentContainer * Container();

protected:
    virtual bool keyev(int key, bool pressed, int modify);
    virtual bool mouseev(int mouse, bool pressed, int x, int y, int modify);
    virtual bool motionev(int mouse, bool pressed, int x, int y, int oldx, int oldy, int modify);
    virtual bool wheelev(int orientation, int delta, int x, int y, int modify);
    virtual void Destroy();
    virtual void Update(float delta);
    virtual void Mount(NLActor *actor);
    virtual void Unmount();
    void SetContainer(NLComponentContainer *container);
    
signals:
    
public slots:

private:
    void Construct();

private:
    bool m_mounted;
    friend class NLComponentContainer;
    friend class NLActor;
    
    Q_DISABLE_COPY(NLComponent)
};

#endif // _KARIN_NLCOMPONENT_H
