#include "simplecontrolcomponent.h"

#include <QDebug>

#include "settings.h"
#include "qdef.h"
#include "engine/nlscene.h"
#include "engine/nlactor.h"

const int SimpleControlComponent::M_Move_Sens = 1500;
const int SimpleControlComponent::M_Turn_Sens = 180;
const float SimpleControlComponent::M_Freelook_Sens = 0.35;
const float SimpleControlComponent::M_Fovy_Sens = 0.01;

SimpleControlComponent::SimpleControlComponent(const NLProperties &prop, NLActor *parent) :
    NLComponent(prop, parent),
      m_moveSens(M_Move_Sens),
      m_turnSens(M_Turn_Sens),
        m_freelookSens(M_Freelook_Sens),
    m_fovySens(M_Fovy_Sens),
    m_upAndDownEnabled(true)
{
    CLASS_NAME(SimpleControlComponent);
    setObjectName("SimpleControlComponent");
    Reset();
}

SimpleControlComponent::~SimpleControlComponent()
{

}

void SimpleControlComponent::Update(float delta)
{
    if(!IsActived())
        return;
    Transform(delta);
    NLComponent::Update(delta);
}

void SimpleControlComponent::Init()
{
    Reset();
    NLComponent::Init();
}

void SimpleControlComponent::Reset()
{
    memset(m_action, 0, sizeof(bool) * NLAction_Total);
    Settings *settings = SINGLE_INSTANCE_OBJ(Settings);
    SetMoveSens(settings->GetSetting<int>("CONTROL_3D/move_sens", M_Move_Sens));
    SetTurnSens(settings->GetSetting<int>("CONTROL_3D/turn_sens", M_Turn_Sens));
    SetFreelookSens(settings->GetSetting<double>("CONTROL_3D/freelook_sens", M_Freelook_Sens));
    SetFovySens(settings->GetSetting<double>("CONTROL_3D/fovy_sens", M_Fovy_Sens));
    NLComponent::Reset();
}

void SimpleControlComponent::InitProperty()
{
    QVariant v;
    v = GetInitProperty("moveSens");
    if(v.isValid())
        SetMoveSens(v.toFloat());
    v = GetInitProperty("turnSens");
    if(v.isValid())
        SetTurnSens(v.toFloat());
    v = GetInitProperty("freelookSens");
    if(v.isValid())
        SetFreelookSens(v.toFloat());
    v = GetInitProperty("fovySens");
    if(v.isValid())
        SetFovySens(v.toFloat());
    v = GetInitProperty("upAndDownEnabled");
    if(v.isValid())
        SetUpAndDownEnabled(v.toBool());
}

bool SimpleControlComponent::keyev(int key, bool pressed, int modifier)
{
    int i;
    bool r;

    r = false;
    i = -1;
    //if(modifier == Qt::NoModifier)
    {
        switch(key)
        {
        case Qt::Key_W:
            i = NLAction_Move_Forward;
            break;
        case Qt::Key_S:
            i = NLAction_Move_Backward;
            break;
        case Qt::Key_A:
            i = NLAction_Move_Left;
            break;
        case Qt::Key_D:
            i = NLAction_Move_Right;
            break;
        case Qt::Key_Q:
        //case Qt::Key_Control:
            i = NLAction_Move_Down;
            break;
        case Qt::Key_E:
        //case Qt::Key_Space:
            i = NLAction_Move_Up;
            break;

        case Qt::Key_Up:
            if(m_upAndDownEnabled)
                i = NLAction_Turn_Up;
            break;
        case Qt::Key_Down:
            if(m_upAndDownEnabled)
                i = NLAction_Turn_Down;
            break;
        case Qt::Key_Left:
            i = NLAction_Turn_Left;
            break;
        case Qt::Key_Right:
            i = NLAction_Turn_Right;
            break;
        case Qt::Key_Z:
            i = NLAction_Roll_Left;
            break;
        case Qt::Key_C:
            i = NLAction_Roll_Right;
            break;
        case Qt::Key_X:
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

        case Qt::Key_M:
            if(pressed)
            {
                NLScene *scene = Scene();
                if(scene)
                    scene->GrabMouseCursor(!scene->IsGrabMouseCursor());
            }
            break;
        case Qt::Key_Escape:
            if(pressed)
            {
                NLScene *scene = Scene();
                if(scene)
                    scene->GrabMouseCursor(false);
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

bool SimpleControlComponent::mouseev(int mouse, bool pressed, int x, int y, int modifier)
{
    if(pressed)
    {
        if(mouse == Qt::MidButton)
        {
            emit fovyChanged(0);
            return true;
        }
    }
    return false;
}

bool SimpleControlComponent::motionev(int button, bool pressed, int x, int y, int oldx, int oldy, int modifier)
{
    NLScene *scene = Scene();
    if(!scene)
        return false;
    if(pressed || scene->IsGrabMouseCursor())
    {
        //if((modifier & Qt::ControlModifier) == 0)
        {
            float dx = x - oldx;
            float dy = y - oldy;
            if(dx != 0 || dy != 0)
            {
                vector3_t m_rot = VECTOR3(dy * m_freelookSens, dx * m_freelookSens, 0);
                NLActor *actor = Actor();
                if(actor)
                {
                    actor->Turn(m_rot);
                }
            }
            return true;
        }
    }
    return false;
}

void SimpleControlComponent::Transform(float delta)
{
    NLActor *actor = Actor();
    if(!actor)
        return;

    if(m_action[NLAction_Move_Forward] || m_action[NLAction_Move_Backward]
            || m_action[NLAction_Move_Left] || m_action[NLAction_Move_Right]
            || (m_upAndDownEnabled && (m_action[NLAction_Move_Down] || m_action[NLAction_Move_Up]))
            )
    {
        const bool IsWalk = Scene()->KeyState(Qt::Key_Shift);
        const bool IsRun = Scene()->KeyState(Qt::Key_Alt);
        vector3_t m_move = VECTOR3(0, 0, 0);
        float movesens = m_moveSens * delta;
        if(IsWalk && !IsRun)
            movesens *= 0.5;
        else if(IsRun && !IsWalk)
            movesens *= 2;

        if(m_action[NLAction_Move_Forward])
            VECTOR3_Z(m_move) = movesens;
        else if(m_action[NLAction_Move_Backward])
            VECTOR3_Z(m_move) = -movesens;

        if(m_action[NLAction_Move_Left])
            VECTOR3_X(m_move) = -movesens;
        else if(m_action[NLAction_Move_Right])
            VECTOR3_X(m_move) = movesens;

        if(m_upAndDownEnabled)
        {
            if(m_action[NLAction_Move_Down])
                VECTOR3_Y(m_move) = -movesens;
            else if(m_action[NLAction_Move_Up])
                VECTOR3_Y(m_move) = movesens;
        }

        actor->Move(m_move);
    }

    if(m_action[NLAction_Turn_Up] || m_action[NLAction_Turn_Down]
            || m_action[NLAction_Turn_Left] || m_action[NLAction_Turn_Right]
            || m_action[NLAction_Roll_Left] || m_action[NLAction_Roll_Right]
            )
    {
        vector3_t m_turn = VECTOR3(0, 0, 0);
        float turnsens = m_turnSens * delta;

        if(m_action[NLAction_Turn_Up])
            VECTOR3_X(m_turn) = -turnsens;
        else if(m_action[NLAction_Turn_Down])
            VECTOR3_X(m_turn) = turnsens;

        if(m_action[NLAction_Turn_Left])
            VECTOR3_Y(m_turn) = -turnsens;
        else if(m_action[NLAction_Turn_Right])
            VECTOR3_Y(m_turn) = turnsens;

        if(m_action[NLAction_Roll_Left])
            VECTOR3_Z(m_turn) = -turnsens;
        else if(m_action[NLAction_Roll_Right])
            VECTOR3_Z(m_turn) = turnsens;

        actor->Turn(m_turn);
    }
}

void SimpleControlComponent::SetMoveSens(float moveSens)
{
    if(m_moveSens != moveSens)
    {
        m_moveSens = moveSens;
        emit propertyChanged("moveSens", m_moveSens);
    }
}

void SimpleControlComponent::SetTurnSens(float turnSens)
{
    if(m_turnSens != turnSens)
    {
        m_turnSens = turnSens;
        emit propertyChanged("turnSens", m_turnSens);
    }
}

void SimpleControlComponent::SetFreelookSens(float freelookSens)
{
    if(m_freelookSens != freelookSens)
    {
        m_freelookSens = freelookSens;
        emit propertyChanged("freelookSens", m_freelookSens);
    }
}

void SimpleControlComponent::SetFovySens(float fovySens)
{
    if(m_fovySens != fovySens)
    {
        m_fovySens = fovySens;
        emit propertyChanged("fovySens", m_fovySens);
    }
}

void SimpleControlComponent::SetUpAndDownEnabled(bool upAndDownEnabled)
{
    if(m_upAndDownEnabled != upAndDownEnabled)
    {
        m_upAndDownEnabled = upAndDownEnabled;
        m_action[NLAction_Move_Up] = m_action[NLAction_Move_Down] = false;
        emit propertyChanged("upAndDownEnabled", m_upAndDownEnabled);
    }
}

bool SimpleControlComponent::wheelev(int mouse, int orientation, int delta, int x, int y, int modifier)
{
    NLActor *actor = Actor();
    if(!actor)
        return false;

    if(orientation == Qt::Vertical)
    {
        float d = -delta * m_fovySens;
        emit fovyChanged(d);
        return true;
    }
//    else
//    {
//        if(mouse == Qt::MidButton)
//        {
//            emit fovyChanged(0);
//        }
//    }
    return false;
}
