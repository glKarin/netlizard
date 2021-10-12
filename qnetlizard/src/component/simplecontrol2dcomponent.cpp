#include "simplecontrol2dcomponent.h"

#include <QDebug>

#include "settings.h"
#include "qdef.h"
#include "nlscene.h"
#include "nlactor.h"

const int SimpleControl2DComponent::M_Move_Sens = 1000;
const float SimpleControl2DComponent::M_Rot_Sens = 100;
const float SimpleControl2DComponent::M_Zoom_Sens = 0.001;

SimpleControl2DComponent::SimpleControl2DComponent(const QVariantHash &prop, NLActor *parent) :
    NLComponent(prop, parent),
      m_moveSens(M_Move_Sens),
        m_rotSens(M_Rot_Sens),
    m_zoomSens(M_Zoom_Sens)
{
    setObjectName("SimpleControl2DComponent");
    Reset();
}

SimpleControl2DComponent::~SimpleControl2DComponent()
{

}

void SimpleControl2DComponent::Update(float delta)
{
    NLComponent::Update(delta);
    Transform(delta);
}

void SimpleControl2DComponent::Init()
{
    Reset();
    NLComponent::Init();
}

void SimpleControl2DComponent::Reset()
{
    memset(m_action, 0, sizeof(bool) * NLAction_Total);
    Settings *settings = SINGLE_INSTANCE_OBJ(Settings);
    m_moveSens = settings->GetSetting<int>("CONTROL_2D/trans_sens", M_Move_Sens);
    m_rotSens = settings->GetSetting<double>("CONTROL_2D/rot_sens", M_Rot_Sens);
    NLComponent::Reset();
}

bool SimpleControl2DComponent::keyev(int key, bool pressed, int modifier)
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

        case Qt::Key_W:
            i = NLAction_Jump;
            break;
        case Qt::Key_S:
            i = NLAction_Crough;
            break;
        case Qt::Key_A:
            i = NLAction_Roll_Left;
            break;
        case Qt::Key_D:
            i = NLAction_Roll_Right;
            break;

        case Qt::Key_Comma:
            i = NLAction_Turn_Left;
            break;
        case Qt::Key_Period:
            i = NLAction_Turn_Right;
            break;
        case Qt::Key_Slash:
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
                NLScene *scene = Scene();
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

bool SimpleControl2DComponent::motionev(int button, bool pressed, int x, int y, int oldx, int oldy, int modifier)
{
    if(pressed)
    {
        //if((modifier & Qt::ControlModifier) == 0)
        {
            float dx = x - oldx;
            float dy = y - oldy;
            if(dx != 0 || dy != 0)
            {
                vector3_s m_rot = VECTOR3(-dx, - -dy, 0);
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

bool SimpleControl2DComponent::wheelev(int orientation, int delta, int x, int y, int modifier)
{
    NLActor *actor = Actor();
    if(!actor)
        return false;

#define MIN_SCALE 0.1
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
#undef MIN_SCALE
    return false;
}

void SimpleControl2DComponent::Transform(float delta)
{
    NLActor *actor = Actor();
    if(!actor)
        return;

    if(m_action[NLAction_Move_Left] || m_action[NLAction_Move_Right]
            || m_action[NLAction_Move_Down] || m_action[NLAction_Move_Up]
            )
    {
        vector3_s m_move = VECTOR3(0, 0, 0);
        float movesens = m_moveSens * delta;

        if(m_action[NLAction_Move_Left])
            VECTOR3_X(m_move) = -movesens;
        else if(m_action[NLAction_Move_Right])
            VECTOR3_X(m_move) = movesens;

        if(m_action[NLAction_Move_Down])
            VECTOR3_Y(m_move) = -movesens;
        else if(m_action[NLAction_Move_Up])
            VECTOR3_Y(m_move) = movesens;

        vector3_invertv(&m_move);
        actor->Move(m_move);
    }

    if(m_action[NLAction_Roll_Left] || m_action[NLAction_Roll_Right]
            || m_action[NLAction_Crough] || m_action[NLAction_Jump]
            )
    {
        vector3_s m_move = VECTOR3(0, 0, 0);
        float movesens = m_moveSens * delta;

        if(m_action[NLAction_Roll_Left])
            VECTOR3_X(m_move) = -movesens;
        else if(m_action[NLAction_Roll_Right])
            VECTOR3_X(m_move) = movesens;

        if(m_action[NLAction_Crough])
            VECTOR3_Y(m_move) = -movesens;
        else if(m_action[NLAction_Jump])
            VECTOR3_Y(m_move) = movesens;

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

        vector3_invertv(&m_turn);

        actor->Turn(m_turn);
    }
}
