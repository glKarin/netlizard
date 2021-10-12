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
    
signals:
    
public slots:

protected:
    virtual void Init();
    virtual void Reset();
    virtual void Update(float delta);
    virtual bool keyev(int key, bool pressed, int modifier);
    //virtual bool mouseev(int mouse, bool pressed, int x, int y, int modifier);
    virtual bool motionev(int mouse, bool pressed, int x, int y, int oldx, int oldy, int modifier);
    virtual void Transform(float delta);

private:
    float m_movesens;
    float m_turnsens;
    float m_rotsens;
    bool m_direction[Direction_Total];
    bool m_rotation[Rotation_Total];

    static const int M_Move_Sens;
    static const int M_Turn_Sens;
    static const float M_Rot_Sens;
};

#endif // _KARIN_SIMPLECONTROLCOMPONENT_H
