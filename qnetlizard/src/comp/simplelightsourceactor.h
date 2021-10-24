#ifndef _KARIN_SIMPLELIGHTSOURCEACTOR_H
#define _KARIN_SIMPLELIGHTSOURCEACTOR_H

#include "nlactor.h"

class SimpleLightSourceComponent;

NLACTOR(SimpleLightSourceActor)
class SimpleLightSourceActor : public NLActor
{
    Q_OBJECT
public:
    explicit SimpleLightSourceActor(const NLPropperties &prop = QVariantHash(), NLActor *parent = 0);
    virtual ~SimpleLightSourceActor();
    SimpleLightSourceComponent * LightSource();
    NLVector3 LightDirection() const;

protected:
    virtual void Init();
    virtual void Destroy();
    virtual void Update(float delta);
    
signals:
    
public slots:

private:
    SimpleLightSourceComponent *m_lightSource;

    Q_DISABLE_COPY(SimpleLightSourceActor)
};

#endif // _KARIN_SIMPLELIGHTSOURCEACTOR_H
