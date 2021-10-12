#ifndef _KARIN_RENDERABLE_H
#define _KARIN_RENDERABLE_H

class NLActor;

#include <qglbuffer.h>

class NLRenderable
{
public:
    explicit NLRenderable(NLActor *actor = 0);
    virtual ~NLRenderable();

protected:
    void SetActor(NLActor *actor);
    NLActor * Actor();
    virtual void InitRender() {}
    virtual void Render() = 0;
    virtual void DeinitRender() {}

private:
    NLActor *m_actor;
    friend class NLActor;
    Q_DISABLE_COPY(NLRenderable)
};

#endif // _KARIN_RENDERABLE_H
