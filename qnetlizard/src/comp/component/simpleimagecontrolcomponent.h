#ifndef _KARIN_SIMPLEIMAGECONTROLCOMPONENT_H
#define _KARIN_SIMPLEIMAGECONTROLCOMPONENT_H

#include "nlcomponent.h"

NLCOMPONENT(SimpleImageControlComponent)
class SimpleImageControlComponent : public NLComponent
{
    Q_OBJECT
public:
    explicit SimpleImageControlComponent(const NLProperties &prop = NLProperties(), NLActor *parent = 0);
    virtual ~SimpleImageControlComponent();
    virtual void Reset();
    NLSETTER(transSens) void SetTransSens(float transSens);
    NLSETTER(rotSens) void SetRotSens(float rotSens);
    NLSETTER(zoomSens) void SetZoomSens(float zoomSens);
    NLGETTER(transSens) float TransSens() const;
    NLGETTER(rotSens) float RotSens() const;
    NLGETTER(zoomSens) float ZoomSens() const;
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
    virtual bool motionev(int mouse, bool pressed, int x, int y, int oldx, int oldy, int modifier);
    virtual bool wheelev(int mouse, int orientation, int delta, int x, int y, int modifier);
    virtual void Transform(float delta);
    virtual void InitProperty();

private:
    NLPROPERTY(float, transSens) float m_transSens;
    NLPROPERTY(float, rotSens) float m_rotSens;
    NLPROPERTY(float, zoomSens) float m_zoomSens;
    bool m_action[NLAction_Total];
    NLPROPERTY(bool, invertX) bool m_invertX;
    NLPROPERTY(bool, invertY)bool m_invertY;

    static const float M_Trans_Sens;
    static const float M_Rot_Sens;
    static const float M_Zoom_Sens;
};

#endif // _KARIN_SIMPLEIMAGECONTROLCOMPONENT_H
