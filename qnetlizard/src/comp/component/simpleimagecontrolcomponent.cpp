#include "simpleimagecontrolcomponent.h"

#include <QDebug>

#include "settings.h"
#include "qdef.h"
#include "nlscene.h"
#include "nlactor.h"

#define MIN_SCALE 0.1

const float SimpleImageControlComponent::M_Trans_Sens = 1000;
const float SimpleImageControlComponent::M_Rot_Sens = 100;
const float SimpleImageControlComponent::M_Zoom_Sens = 0.001;

SimpleImageControlComponent::SimpleImageControlComponent(const NLPropperties &prop, NLActor *parent) :
    NLComponent(prop, parent),
      m_transSens(M_Trans_Sens),
        m_rotSens(M_Rot_Sens),
    m_zoomSens(M_Zoom_Sens),
    m_invertX(false),
    m_invertY(false)
{
    setObjectName("SimpleImageControlComponent");
    Reset();
}

SimpleImageControlComponent::~SimpleImageControlComponent()
{

}

void SimpleImageControlComponent::Update(float delta)
{
    NLComponent::Update(delta);
    Transform(delta);
}

void SimpleImageControlComponent::Init()
{
    Reset();
    NLComponent::Init();
}

void SimpleImageControlComponent::Reset()
{
    memset(m_action, 0, sizeof(bool) * NLAction_Total);
    Settings *settings = SINGLE_INSTANCE_OBJ(Settings);
    SetTransSens(settings->GetSetting<double>("CONTROL_2D/trans_sens", M_Trans_Sens));
    SetRotSens(settings->GetSetting<double>("CONTROL_2D/rot_sens", M_Rot_Sens));
    NLComponent::Reset();
}

void SimpleImageControlComponent::InitProperty()
{
    NLPropperty v;
    v = GetProperty("transSens");
    if(v.isValid())
        SetTransSens(v.toFloat());
    v = GetProperty("rotSens");
    if(v.isValid())
        SetRotSens(v.toFloat());
    v = GetProperty("zoomSens");
    if(v.isValid())
        SetZoomSens(v.toFloat());
}

bool SimpleImageControlComponent::keyev(int key, bool pressed, int modifier)
{
    int i;
    bool r;

    r = false;
    i = -1;
    //if(modifier == Qt::NoModifier)
    {
        switch(key)
        {
        case Qt::Key_Up:
            i = NLAction_Move_Up;
            break;
        case Qt::Key_Down:
            i = NLAction_Move_Down;
            break;
        case Qt::Key_Left:
            i = NLAction_Move_Left;
            break;
        case Qt::Key_Right:
            i = NLAction_Move_Right;
            break;

        case Qt::Key_Comma:
            i = NLAction_Turn_Left;
            break;
        case Qt::Key_Period:
            i = NLAction_Turn_Right;
            break;
        case Qt::Key_Slash:
            if(pressed)
            {
                NLActor *actor = Actor();
                if(actor)
                {
                    NLVector3 vec = actor->Rotation();
                    if(VECTOR3_Z(vec) != 0)
                    {
                        VECTOR3_Z(vec) = 0;
                        actor->SetRotation(vec);
                    }
                }
            }
            break;
        case Qt::Key_Plus:
            i = NLAction_Zoom_In;
            break;
        case Qt::Key_Minus:
            i = NLAction_Zoom_Out;
            break;

        case Qt::Key_Escape:
            if(pressed)
            {
                NLActor *actor = Actor();
                if(actor)
                {
                    actor->Reset();
                }
            }
            break;
        default:
            break;
        }
        if(i != -1)
        {
            m_action[i] = pressed;
            //Transform();
            r = true;
        }
        goto __Exit;
    }

__Exit:
    return r;
}

bool SimpleImageControlComponent::motionev(int button, bool pressed, int x, int y, int oldx, int oldy, int modifier)
{
    if(pressed)
    {
        //if((modifier & Qt::ControlModifier) == 0)
        {
            float dx = x - oldx;
            float dy = y - oldy;
            if(dx != 0 || dy != 0)
            {
                vector3_s m_rot = VECTOR3(dx, -dy, 0);
                NLActor *actor = Actor();
                if(actor)
                {
                    actor->Move(m_rot);
                }
            }
            return true;
        }
    }
    return false;
}

bool SimpleImageControlComponent::wheelev(int mouse, int orientation, int delta, int x, int y, int modifier)
{
    NLActor *actor = Actor();
    if(!actor)
        return false;

    if(orientation == Qt::Vertical)
    {
        NLVector3 scale = actor->Scale();
        float d = delta * m_zoomSens;
        if(delta < 0 && (VECTOR3_X(scale) * VECTOR3_Y(scale) < 1.0))
            d /= 4;
        NLVector3 unit = VECTOR3(0, 0, 0);
        if(VECTOR3_X(scale) + d < MIN_SCALE)
            VECTOR3_X(unit) = VECTOR3_X(scale) - MIN_SCALE;
        else
            VECTOR3_X(unit) = d;
        if(VECTOR3_Y(scale) + d < MIN_SCALE)
            VECTOR3_Y(unit) = VECTOR3_Y(scale) - MIN_SCALE;
        else
            VECTOR3_Y(unit) = d;
        actor->Zoom(unit);
        return true;
    }
    return false;
}

void SimpleImageControlComponent::Transform(float delta)
{
    NLActor *actor = Actor();
    if(!actor)
        return;

    if(m_action[NLAction_Move_Left] || m_action[NLAction_Move_Right]
            || m_action[NLAction_Move_Down] || m_action[NLAction_Move_Up]
            )
    {
        vector3_s m_move = VECTOR3(0, 0, 0);
        float movesens = m_transSens * delta;

        if(m_action[NLAction_Move_Left])
            VECTOR3_X(m_move) = - -movesens;
        else if(m_action[NLAction_Move_Right])
            VECTOR3_X(m_move) = -movesens;

        if(m_action[NLAction_Move_Down])
            VECTOR3_Y(m_move) = - -movesens;
        else if(m_action[NLAction_Move_Up])
            VECTOR3_Y(m_move) = -movesens;

        vector3_invertv(&m_move);
        if(m_invertX)
            VECTOR3_X(m_move) = -VECTOR3_X(m_move);
        if(m_invertY)
            VECTOR3_Y(m_move) = -VECTOR3_Y(m_move);
        actor->Move(m_move);
    }

    if(m_action[NLAction_Turn_Left] || m_action[NLAction_Turn_Right])
    {
        vector3_s m_turn = VECTOR3(0, 0, 0);
        float turnsens = m_rotSens * delta;

        if(m_action[NLAction_Turn_Left])
            VECTOR3_Z(m_turn) = -turnsens;
        else if(m_action[NLAction_Turn_Right])
            VECTOR3_Z(m_turn) = turnsens;

        if(!m_invertY)
            vector3_invertv(&m_turn);

        actor->Turn(m_turn);
    }

    if(m_action[NLAction_Zoom_Out] || m_action[NLAction_Zoom_In])
    {
        float d = 0;
        if(m_action[NLAction_Zoom_Out])
            d -= delta;
        else if(m_action[NLAction_Zoom_In])
            d += delta;
        if(d != 0)
        {
            NLVector3 scale = actor->Scale();
            if(delta < 0 && (VECTOR3_X(scale) * VECTOR3_Y(scale) < 1.0))
                d /= 4;
            NLVector3 unit = VECTOR3(0, 0, 0);
            if(VECTOR3_X(scale) + d < MIN_SCALE)
                VECTOR3_X(unit) = VECTOR3_X(scale) - MIN_SCALE;
            else
                VECTOR3_X(unit) = d;
            if(VECTOR3_Y(scale) + d < MIN_SCALE)
                VECTOR3_Y(unit) = VECTOR3_Y(scale) - MIN_SCALE;
            else
                VECTOR3_Y(unit) = d;
            actor->Zoom(unit);
        }
    }
}

void SimpleImageControlComponent::SetTransSens(float transSens)
{
    if(m_transSens != transSens)
        m_transSens = transSens;
}

void SimpleImageControlComponent::SetRotSens(float rotSens)
{
    if(m_rotSens != rotSens)
        m_rotSens = rotSens;
}

void SimpleImageControlComponent::SetZoomSens(float zoomSens)
{
    if(m_zoomSens != zoomSens)
        m_zoomSens = zoomSens;
}

float SimpleImageControlComponent::TransSens() const
{
    return m_transSens;
}

float SimpleImageControlComponent::RotSens() const
{
    return m_rotSens;
}

float SimpleImageControlComponent::ZoomSens() const
{
    return m_zoomSens;
}

void SimpleImageControlComponent::SetInvertX(bool b)
{
    if(m_invertX != b)
        m_invertX = b;
}

void SimpleImageControlComponent::SetInvertY(bool b)
{
    if(m_invertY != b)
        m_invertY = b;
}

bool SimpleImageControlComponent::InvertX() const
{
    return m_invertX;
}

bool SimpleImageControlComponent::InvertY() const
{
    return m_invertY;
}
