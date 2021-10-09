#include "simplecontrolcomponent.h"

#include <QDebug>

#include "nlscene.h"

SimpleControlComponent::SimpleControlComponent(const QVariantHash &prop, NLActor *parent) :
    NLComponent(prop, parent),
      m_movesens(1800),
      m_turnsens(18)
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
    ResetCurrent();
    memset(m_direction, 0, sizeof(bool) * Direction_Total);
    memset(m_rotation, 0, sizeof(bool) * Rotation_Total);
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
    //VECTOR3_X(m_rot) = VECTOR3_Y(m_rot) = VECTOR3_Z(m_rot) = 0;
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
                VECTOR3_X(m_rot) += dy;
                VECTOR3_Y(m_rot) += dx;
                VECTOR3_Z(m_rot) = 0;
            }
            return true;
        }
    }
    return false;
}

void SimpleControlComponent::Transform(float delta)
{
    VECTOR3_X(m_move) = VECTOR3_Y(m_move) = VECTOR3_Z(m_move) = 0;
    if(m_direction[Direction_Forward] || m_direction[Direction_Backward]
            || m_direction[Direction_Left] || m_direction[Direction_Right]
            || m_direction[Direction_Down] || m_direction[Direction_Up]
            )
    {
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
    }

    if(m_rotation[Rotation_Up] || m_rotation[Rotation_Down]
            || m_rotation[Rotation_Left] || m_rotation[Rotation_Right]
            )
    {
        float turnsens = 10 * m_turnsens * delta;

        if(m_rotation[Rotation_Up])
            VECTOR3_X(m_turn) = -turnsens;
        else if(m_rotation[Rotation_Down])
            VECTOR3_X(m_turn) = turnsens;

        if(m_rotation[Rotation_Left])
            VECTOR3_Y(m_turn) = -turnsens;
        else if(m_rotation[Rotation_Right])
            VECTOR3_Y(m_turn) = turnsens;
    }
}

vector3_s SimpleControlComponent::CurrentMove(bool reset)
{
    vector3_s res = m_move;
    if(reset)
        VECTOR3_X(m_move) = VECTOR3_Y(m_move) = VECTOR3_Z(m_move) = 0;
    return res;
}

vector3_s SimpleControlComponent::CurrentTurn(bool reset)
{
    vector3_s res = m_turn;
    if(reset)
        VECTOR3_X(m_turn) = VECTOR3_Y(m_turn) = VECTOR3_Z(m_turn) = 0;
    return res;
}

vector3_s SimpleControlComponent::CurrentRot(bool reset)
{
    vector3_s res = m_rot;
    if(reset)
        VECTOR3_X(m_rot) = VECTOR3_Y(m_rot) = VECTOR3_Z(m_rot) = 0;
    return res;
}

void SimpleControlComponent::ResetCurrent()
{

    VECTOR3_X(m_move) = VECTOR3_Y(m_move) = VECTOR3_Z(m_move) = 0;
    VECTOR3_X(m_turn) = VECTOR3_Y(m_turn) = VECTOR3_Z(m_turn) = 0;
    VECTOR3_X(m_rot) = VECTOR3_Y(m_rot) = VECTOR3_Z(m_rot) = 0;
}
