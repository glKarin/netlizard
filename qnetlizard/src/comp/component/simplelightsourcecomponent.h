#ifndef _KARIN_SIMPLELIGHTSOURCECOMPONENT_H
#define _KARIN_SIMPLELIGHTSOURCECOMPONENT_H

#include "nlcomponent.h"

NLCOMPONENT(SimpleLightSourceComponent)
class SimpleLightSourceComponent : public NLComponent
{
    Q_OBJECT
    public:
    enum LightSourceType
    {
        LightSourceType_Direction = 1,
        LightSourceType_spot,
        LightSourceType_Point,
    };

public:
    explicit SimpleLightSourceComponent(const NLPropperties &prop = NLPropperties(), NLActor *parent = 0);
    virtual ~SimpleLightSourceComponent();
    NLSETTER(type) void SetType(LightSourceType type);
    NLGETTER(type) LightSourceType Type() const;
    
public slots:

protected:
    virtual void Init();
    virtual void Update(float delta);
    virtual void InitProperty();

private:
    NLPROPERTY(int, type) LightSourceType m_type;
};

#endif // _KARIN_SIMPLELIGHTSOURCECOMPONENT_H
