#ifndef _KARIN_NLACTOR_H
#define _KARIN_NLACTOR_H

#include "nlobject.h"

class NLActorContainer;
class NLRenderable;
class NLComponentContainer;
class NLComponent;

class NLActor : public NLObject
{
    Q_OBJECT
public:
    explicit NLActor(const NLPropperties &prop = NLPropperties(), NLActor *parent = 0);
    virtual ~NLActor();
    void SetRenderable(NLRenderable *renderable);
    NLRenderable * Renderable();
    NLActor * ParentActor();
    void SetParentActor(NLActor *actor);
    NLActorContainer * Container();

protected:
    virtual bool keyev(int key, bool pressed, int modify);
    virtual bool mouseev(int mouse, bool pressed, int x, int y, int modify);
    virtual bool motionev(int mouse, bool pressed, int x, int y, int oldx, int oldy, int modify);
    virtual bool wheelev(int orientation, int delta, int x, int y, int modify);
    virtual void Init();
    virtual void Destroy();
    virtual void Update(float delta);
    virtual void Render();
    void SetContainer(NLActorContainer *container);
    bool AddComponent(NLComponent *item);
    bool RemoveComponent(NLComponent *item);
    
signals:
    
public slots:

private:
    NLRenderable *m_renderable;
    NLComponentContainer *m_components;
    NLActorContainer *m_children;

    friend class NLActorContainer;
    
    Q_DISABLE_COPY(NLActor)
};

#endif // _KARIN_NLACTOR_H
