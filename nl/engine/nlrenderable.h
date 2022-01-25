#ifndef _KARIN_RENDERABLE_H
#define _KARIN_RENDERABLE_H

#include "nlobject.h"
#include "nldef.h"

class NLActor;

class NLLIB_EXPORT NLRenderable : public NLObject
{
    Q_OBJECT
public:
    explicit NLRenderable(NLActor *actor = 0);
    explicit NLRenderable(const NLProperties &prop, NLActor *parent = 0);
    explicit NLRenderable(NLScene *scene, NLActor *parent = 0);
    explicit NLRenderable(NLScene *scene, const NLProperties &prop, NLActor *parent = 0);
    virtual ~NLRenderable();
    NLActor * Actor();

protected:
    void SetActor(NLActor *actor);
    virtual void Render() = 0;
    virtual void Update(float delta);
    virtual void Destroy();

private:
    void Construct();

private:
    friend class NLActor;
    Q_DISABLE_COPY(NLRenderable)
};

Q_DECLARE_METATYPE(NLRenderable*)

#endif // _KARIN_RENDERABLE_H
