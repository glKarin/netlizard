#ifndef _KARIN_SIMPLELIGHTSOURCEACTOR_H
#define _KARIN_SIMPLELIGHTSOURCEACTOR_H

#include "engine/nlactor.h"

class SimpleLightSourceComponent;

NLACTOR(SimpleLightSourceActor)
class SimpleLightSourceActor : public NLActor
{
    Q_OBJECT
    Q_PROPERTY(NLComponent* lightSource READ LightSource FINAL)
public:
    explicit SimpleLightSourceActor(const NLProperties &prop = NLProperties(), NLActor *parent = 0);
    virtual ~SimpleLightSourceActor();
    NLComponent * LightSource() { return m_lightSource; }
    NLVector3 LightSourceDirection() const;
    NLVector3 LightSourcePosition() const;
    bool IsDirectionLighting() const;

protected:
    virtual void Init();
    
signals:
    
public slots:

private:
    NLComponent *m_lightSource;

    Q_DISABLE_COPY(SimpleLightSourceActor)
};

#endif // _KARIN_SIMPLELIGHTSOURCEACTOR_H
