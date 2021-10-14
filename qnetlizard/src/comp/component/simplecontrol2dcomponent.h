#ifndef _KARIN_SIMPLECONTROL2DCOMPONENT_H
#define _KARIN_SIMPLECONTROL2DCOMPONENT_H

#include "nlcomponent.h"

NLCOMPONENT(SimpleControl2DComponent)
class SimpleControl2DComponent : public NLComponent
{
    Q_OBJECT
public:
    explicit SimpleControl2DComponent(const NLPropperties &prop = NLPropperties(), NLActor *parent = 0);
    virtual ~SimpleControl2DComponent();
    virtual void Reset();
    NLSETTER(moveSens) void SetMoveSens(float moveSens);
    NLGETTER(moveSens) float MoveSens() const;
    
signals:
    
public slots:

protected:
    virtual void Init();
    virtual void Update(float delta);
    virtual bool keyev(int key, bool pressed, int modifier);
    virtual void Transform(float delta);
    virtual void InitProperty();

private:
    NLPROPERTY(float, moveSens) float m_moveSens;
    bool m_action[NLAction_Total];

    static const float M_Move_Sens;
};

#endif // _KARIN_SIMPLECONTROL2DCOMPONENT_H
