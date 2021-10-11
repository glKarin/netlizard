#include "simplecontrolcomponent.h"

#include <QDebug>

#include "settings.h"
#include "qdef.h"
#include "nlscene.h"
#include "nlactor.h"

const int SimpleControlComponent::M_Move_Sens = 1800;
const int SimpleControlComponent::M_Turn_Sens = 18;
const float SimpleControlComponent::M_Rot_Sens = 0.5;

SimpleControlComponent::SimpleControlComponent(const QVariantHash &prop, NLActor *parent) :
    NLComponent(prop, parent),
      m_movesens(M_Move_Sens),
      m_turnsens(M_Turn_Sens),
        m_rotsens(M_Rot_Sens)
{
    setObjectName("SimpleControlComponent");
    Reset();
}

SimpleControlComponent::~SimpleControlComponent()
{

}

void SimpleControlComponent::Update(float delta)
{
    NLComponent::Update(delta);
    Transform(delta);
}

void SimpleControlComponent::Init()
{
    Reset();
    NLComponent::Init();
}

void SimpleControlComponent::Reset()
{
    memset(m_direction, 0, sizeof(bool) * Direction_Total);
    memset(m_rotation, 0, sizeof(bool) * Rotation_Total);
    Settings *settings = SINGLE_INSTANCE_OBJ(Settings);
    m_movesens = settings->GetSetting<int>("CONTROL/move_sens", M_Move_Sens);
    m_turnsens = settings->GetSetting<int>("CONTROL/turn_sens", M_Turn_Sens);
    m_rotsens = settings->GetSetting<double>("CONTROL/rot_sens", M_Rot_Sens);
    NLComponent::Reset();
}

bool SimpleControlComponent::keyev(int key, bool pressed, int modify)
{
    int i, j;
    bool r;

    r = false;
    if(modify == Qt::NoModifier)
    {
        i = -1;
        j = -1;
        switch(key)
        {
        case Qt::Key_W:
            i = Direction_Forward;
            break;
        case Qt::Key_S:
            i = Direction_Backward;
            break;
        case Qt::Key_A:
            i = Direction_Left;
            break;
        case Qt::Key_D:
            i = Direction_Right;
            break;
        case Qt::Key_Q:
            i = Direction_Down;
            break;
        case Qt::Key_E:
            i = Direction_Up;
            break;

        case Qt::Key_Up:
            j = Rotation_Up;
            break;
        case Qt::Key_Down:
            j = Rotation_Down;
            break;
        case Qt::Key_Left:
            j = Rotation_Left;
            break;
        case Qt::Key_Right:
            j = Rotation_Right;
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
            m_direction[i] = pressed;
            //Transform();
            r = true;
        }
        if(j != -1)
        {
            m_rotation[j] = pressed;
            //Transform();
            r = true;
        }
        goto __Exit;
    }
    else if(modify & Qt::ControlModifier)
    {
        goto __Exit;
    }
    else
        goto __Exit;

__Exit:
    return r;
}

bool SimpleControlComponent::motionev(int button, bool pressed, int x, int y, int oldx, int oldy, int modify)
{
    NLScene *scene = Scene();
    if(!scene)
        return false;
    if(pressed || scene->IsGrabMouseCursor())
    {
        //if((modify & Qt::ControlModifier) == 0)
        {
            float dx = x - oldx;
            float dy = y - oldy;
            if(dx != 0 || dy != 0)
            {
                vector3_s m_rot = VECTOR3(dy * m_rotsens, dx * m_rotsens, 0);
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
    if(m_direction[Direction_Forward] || m_direction[Direction_Backward]
            || m_direction[Direction_Left] || m_direction[Direction_Right]
            || m_direction[Direction_Down] || m_direction[Direction_Up]
            )
    {
        vector3_s m_move;
        VECTOR3_X(m_move) = VECTOR3_Y(m_move) = VECTOR3_Z(m_move) = 0;
        float movesens = m_movesens * delta;

        if(m_direction[Direction_Forward])
            VECTOR3_Z(m_move) = movesens;
        else if(m_direction[Direction_Backward])
            VECTOR3_Z(m_move) = -movesens;

        if(m_direction[Direction_Left])
            VECTOR3_X(m_move) = -movesens;
        else if(m_direction[Direction_Right])
            VECTOR3_X(m_move) = movesens;

        if(m_direction[Direction_Down])
            VECTOR3_Y(m_move) = -movesens;
        else if(m_direction[Direction_Up])
            VECTOR3_Y(m_move) = movesens;

        NLActor *actor = Actor();
        if(actor)
        {
            actor->Move(m_move);
        }
    }

    if(m_rotation[Rotation_Up] || m_rotation[Rotation_Down]
            || m_rotation[Rotation_Left] || m_rotation[Rotation_Right]
            )
    {
        vector3_s m_turn;
        VECTOR3_X(m_turn) = VECTOR3_Y(m_turn) = VECTOR3_Z(m_turn) = 0;
        float turnsens = 10 * m_turnsens * delta;

        if(m_rotation[Rotation_Up])
            VECTOR3_X(m_turn) = -turnsens;
        else if(m_rotation[Rotation_Down])
            VECTOR3_X(m_turn) = turnsens;

        if(m_rotation[Rotation_Left])
            VECTOR3_Y(m_turn) = -turnsens;
        else if(m_rotation[Rotation_Right])
            VECTOR3_Y(m_turn) = turnsens;

        NLActor *actor = Actor();
        if(actor)
        {
            actor->Turn(m_turn);
        }
    }
}
