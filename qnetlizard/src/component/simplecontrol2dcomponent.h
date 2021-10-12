#ifndef _KARIN_SIMPLECONTROL2DCOMPONENT_H
#define _KARIN_SIMPLECONTROL2DCOMPONENT_H

#include "nlcomponent.h"
#include "nldef.h"

class QWidget;

class SimpleControl2DComponent : public NLComponent
{
    Q_OBJECT
public:
    explicit SimpleControl2DComponent(const QVariantHash &prop = QVariantHash(), NLActor *parent = 0);
    virtual ~SimpleControl2DComponent();
    
signals:
    
public slots:

protected:
    virtual void Init();
    virtual void Reset();
    virtual void Update(float delta);
    virtual bool keyev(int key, bool pressed, int modifier);
    virtual bool motionev(int mouse, bool pressed, int x, int y, int oldx, int oldy, int modifier);
    virtual bool wheelev(int orientation, int delta, int x, int y, int modifier);
    virtual void Transform(float delta);

private:
    float m_moveSens;
    float m_rotSens;
    float m_zoomSens;
    bool m_action[NLAction_Total];

    static const int M_Move_Sens;
    static const float M_Rot_Sens;
    static const float M_Zoom_Sens;
};

#endif // _KARIN_SIMPLECONTROL2DCOMPONENT_H
