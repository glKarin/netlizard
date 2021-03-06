#ifndef _KARIN_SIMPLECONTROL2DCOMPONENT_H
#define _KARIN_SIMPLECONTROL2DCOMPONENT_H

#include "engine/nlcomponent.h"
#include "common/nlconstants.h"

NLCOMPONENT(SimpleControl2DComponent)
class SimpleControl2DComponent : public NLComponent
{
    Q_OBJECT
    Q_PROPERTY(float moveSens READ MoveSens WRITE SetMoveSens FINAL)
    Q_PROPERTY(bool invertX READ InvertX WRITE SetInvertX FINAL)
    Q_PROPERTY(bool invertY READ InvertY WRITE SetInvertY FINAL)
public:
    explicit SimpleControl2DComponent(const NLProperties &prop = NLProperties(), NLActor *parent = 0);
    virtual ~SimpleControl2DComponent();
    virtual void Reset();
    NLSETTER(moveSens) void SetMoveSens(float moveSens);
    NLGETTER(moveSens) float MoveSens() const { return m_moveSens; }
    NLSETTER(invertX) void SetInvertX(bool b);
    NLSETTER(invertY) void SetInvertY(bool b);
    NLGETTER(invertX) bool InvertX() const { return m_invertX; }
    NLGETTER(invertY) bool InvertY() const { return m_invertY; }
    
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
    NLPROPERTY(bool, invertX) bool m_invertX;
    NLPROPERTY(bool, invertY)bool m_invertY;
    bool m_action[NLAction_Total];

    static const float M_Move_Sens;
};

#endif // _KARIN_SIMPLECONTROL2DCOMPONENT_H
