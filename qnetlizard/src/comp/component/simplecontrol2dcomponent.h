#ifndef _KARIN_SIMPLECONTROL2DCOMPONENT_H
#define _KARIN_SIMPLECONTROL2DCOMPONENT_H

#include "nlcomponent.h"

NLCOMPONENT(SimpleControl2DComponent)
class SimpleControl2DComponent : public NLComponent
{
    Q_OBJECT
public:
    explicit SimpleControl2DComponent(const NLProperties &prop = NLProperties(), NLActor *parent = 0);
    virtual ~SimpleControl2DComponent();
    virtual void Reset();
    NLSETTER(moveSens) void SetMoveSens(float moveSens);
    NLGETTER(moveSens) float MoveSens() const;
    NLSETTER(invertX) void SetInvertX(bool b);
    NLSETTER(invertY) void SetInvertY(bool b);
    NLGETTER(invertX) bool InvertX() const;
    NLGETTER(invertY) bool InvertY() const;
    
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
    NLPROPERTY(bool, invertX) bool m_invertX;
    NLPROPERTY(bool, invertY)bool m_invertY;

    static const float M_Move_Sens;
};

#endif // _KARIN_SIMPLECONTROL2DCOMPONENT_H
