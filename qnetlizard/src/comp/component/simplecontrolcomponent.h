#ifndef _KARIN_SIMPLECONTROLCOMPONENT_H
#define _KARIN_SIMPLECONTROLCOMPONENT_H

#include "nlcomponent.h"

NLCOMPONENT(SimpleControlComponent)
class SimpleControlComponent : public NLComponent
{
    Q_OBJECT
    Q_PROPERTY(float moveSens READ MoveSens WRITE SetMoveSens FINAL)
    Q_PROPERTY(float turnSens READ TurnSens WRITE SetTurnSens FINAL)
    Q_PROPERTY(float freelookSens READ FreelookSens WRITE SetFreelookSens FINAL)
    Q_PROPERTY(float fovySens READ FovySens WRITE SetFovySens FINAL)
public:
    explicit SimpleControlComponent(const NLProperties &prop = NLProperties(), NLActor *parent = 0);
    virtual ~SimpleControlComponent();
    virtual void Reset();
    NLSETTER(moveSens) void SetMoveSens(float moveSens);
    NLSETTER(turnSens) void SetTurnSens(float turnSens);
    NLSETTER(freelookSens) void SetFreelookSens(float freelookSens);
    NLSETTER(fovySens) void SetFovySens(float fovySens);
    NLGETTER(moveSens) float MoveSens() const{ return m_moveSens; }
    NLGETTER(turnSens) float TurnSens() const { return m_turnSens; }
    NLGETTER(freelookSens) float FreelookSens() const { return m_freelookSens; }
    NLGETTER(fovySens) float FovySens() const { return m_fovySens; }
    
signals:
    void fovyChanged(float f);
    
public slots:

protected:
    virtual void Init();
    virtual void Update(float delta);
    virtual bool keyev(int key, bool pressed, int modifier);
    virtual bool mouseev(int mouse, bool pressed, int x, int y, int modifier);
    virtual bool motionev(int mouse, bool pressed, int x, int y, int oldx, int oldy, int modifier);
    virtual bool wheelev(int mouse, int orientation, int delta, int x, int y, int modifier);
    virtual void Transform(float delta);
    virtual void InitProperty();

private:
    NLPROPERTY(float, moveSens) float m_moveSens;
    NLPROPERTY(float, turnSens) float m_turnSens;
    NLPROPERTY(float, freelookSens)float m_freelookSens;
    NLPROPERTY(float, fovySens) float m_fovySens;
    bool m_action[NLAction_Total];

    static const int M_Move_Sens;
    static const int M_Turn_Sens;
    static const float M_Freelook_Sens;
    static const float M_Fovy_Sens;
};

#endif // _KARIN_SIMPLECONTROLCOMPONENT_H
