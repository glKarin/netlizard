#ifndef _KARIN_RENDERABLE_H
#define _KARIN_RENDERABLE_H

class NLActor;

#include <qglbuffer.h>

class NLRenderable
{
public:
    explicit NLRenderable();
    virtual ~NLRenderable();

protected:
    virtual void InitRender() {}
    virtual void Render() = 0;
    virtual void DeinitRender() {}

private:
    friend class NLActor;
    Q_DISABLE_COPY(NLRenderable)
};

#endif // _KARIN_RENDERABLE_H
