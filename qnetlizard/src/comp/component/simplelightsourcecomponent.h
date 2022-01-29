#ifndef _KARIN_SIMPLELIGHTSOURCECOMPONENT_H
#define _KARIN_SIMPLELIGHTSOURCECOMPONENT_H

#include "engine/nlcomponent.h"
#include "common/nlvec.h"

NLCOMPONENT(SimpleLightSourceComponent)
class SimpleLightSourceComponent : public NLComponent
{
    Q_OBJECT
    Q_ENUMS(LightSourceType)
    Q_PROPERTY(LightSourceType transSens READ Type WRITE SetType FINAL)
    Q_PROPERTY(NLVector3 position READ Position WRITE SetPosition FINAL)
    Q_PROPERTY(NLVector3 direction READ Direction WRITE SetDirecton FINAL)
    public:
    enum LightSourceType
    {
        LightSourceType_Direction = 1,
        LightSourceType_spot,
        LightSourceType_Point
    };

public:
    explicit SimpleLightSourceComponent(const NLProperties &prop = NLProperties(), NLActor *parent = 0);
    virtual ~SimpleLightSourceComponent();
    NLSETTER(type) void SetType(LightSourceType type);
    NLGETTER(type) LightSourceType Type() const { return m_type; }
    NLGETTER(direction) NLVector3 Direction() const { return m_direction; }
    NLGETTER(position) NLVector3 Position() const { return m_position; }
    NLSETTER(position) void SetPosition(const NLVector3 &pos);
    NLSETTER(direction) void SetDirecton(const NLVector3 &dir);
    bool IsDirectionLighting() const { return m_type == SimpleLightSourceComponent::LightSourceType_Direction; }
    virtual void Reset();
    
public slots:

protected:
    virtual void Init();
    virtual void Update(float delta);
    virtual void InitProperty();

private:
    NLPROPERTY(int, type) LightSourceType m_type;
    NLPROPERTY(NLVector3, position) NLVector3 m_position;
    NLPROPERTY(NLVector3, direction) NLVector3 m_direction;
};

#endif // _KARIN_SIMPLELIGHTSOURCECOMPONENT_H
