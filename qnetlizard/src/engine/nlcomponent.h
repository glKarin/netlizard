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
    explicit NLComponent(const NLProperties &prop, NLActor *parent = 0);
    explicit NLComponent(NLScene *scene, NLActor *parent = 0);
    explicit NLComponent(NLScene *scene, const NLProperties &prop, NLActor *parent = 0);
    virtual ~NLComponent();
    bool IsMounted() const { return m_mounted; }
    NLActor * Actor();
    const NLActor * Actor() const;
    void SetActor(NLActor *actor);
    NLComponentContainer * Container();
    virtual bool IsActived() const { return NLObject::IsActived() && m_mounted; }

protected:
    virtual bool keyev(int key, bool pressed, int modifier);
    virtual bool mouseev(int mouse, bool pressed, int x, int y, int modifier);
    virtual bool motionev(int mouse, bool pressed, int x, int y, int oldx, int oldy, int modifier);
    virtual bool wheelev(int mouse, int orientation, int delta, int x, int y, int modifier);
    virtual void Destroy();
    virtual void Update(float delta);
    virtual void Mount(NLActor *actor);
    virtual void Unmount();
    void SetContainer(NLComponentContainer *container);
    
signals:
    void mounted();
    void unmounted();
    
public slots:

private:
    void Construct();

private:
    bool m_mounted;
    friend class NLComponentContainer;
    friend class NLActor;
    
    Q_DISABLE_COPY(NLComponent)
};

typedef QList<NLComponent *> NLComponentList;

#endif // _KARIN_NLCOMPONENT_H
