#ifndef _KARIN_RENDERABLE_H
#define _KARIN_RENDERABLE_H

#include "nldef.h"

class NLActor;

class NLLIB_EXPORT NLRenderable
{
public:
    explicit NLRenderable(NLActor *actor = 0);
    virtual ~NLRenderable();
    NLActor * Actor() { return m_actor; }
    QString Name() const { return m_name; }

protected:
    void SetActor(NLActor *actor);
    void SetName(const QString &name);
    virtual void InitRender() {}
    virtual void Render() = 0;
    virtual void DeinitRender() {}

private:
    NLActor *m_actor;
    QString m_name;
    friend class NLActor;
    Q_DISABLE_COPY(NLRenderable)
};

#endif // _KARIN_RENDERABLE_H