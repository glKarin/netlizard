#ifndef _KARIN_SIMPLECONTROLCOMPONENT_H
#define _KARIN_SIMPLECONTROLCOMPONENT_H

#include "nlcomponent.h"

#include "lib/camera.h"

class QWidget;

class SimpleControlComponent : public NLComponent
{
    Q_OBJECT
public:
    explicit SimpleControlComponent(const QVariantHash &prop = QVariantHash(), NLActor *parent = 0);
    virtual ~SimpleControlComponent();
    NLGETTER(move) vector3_s CurrentMove(bool reset = true);
    NLGETTER(turn) vector3_s CurrentTurn(bool reset = true);
    NLGETTER(rot) vector3_s CurrentRot(bool reset = true);
    NLINTERFACE void ResetCurrent();
    
signals:
    
public slots:

protected:
    virtual void Init();
    virtual void Reset();
    virtual void Update(float delta);
    virtual bool keyev(int key, bool pressed, int modify);
    //virtual bool mouseev(int mouse, bool pressed, int x, int y, int modify);
    virtual bool motionev(int mouse, bool pressed, int x, int y, int oldx, int oldy, int modify);
    virtual void Transform(float delta);

private:
    float m_movesens;
    float m_turnsens;
    bool m_direction[Direction_Total];
    bool m_rotation[Rotation_Total];
    vector3_s m_move;
    vector3_s m_turn;
    vector3_s m_rot;

    static const int M_Timer_Interval = 100;
    static const int M_Move_Unit = 10;
    
};

#endif // _KARIN_SIMPLECONTROLCOMPONENT_H
