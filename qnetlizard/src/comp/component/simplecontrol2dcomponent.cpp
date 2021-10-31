#include "simplecontrol2dcomponent.h"

#include <QDebug>

#include "settings.h"
#include "qdef.h"
#include "nlscene.h"
#include "nlactor.h"

const float SimpleControl2DComponent::M_Move_Sens = 1000;

SimpleControl2DComponent::SimpleControl2DComponent(const NLPropperties &prop, NLActor *parent) :
    NLComponent(prop, parent),
      m_moveSens(M_Move_Sens),
        m_invertX(false),
        m_invertY(false)
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
    SetMoveSens(settings->GetSetting<double>("CONTROL_2D/trans_sens", M_Move_Sens));
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
        case Qt::Key_W:
            i = NLAction_Move_Up;
            break;
        case Qt::Key_S:
            i = NLAction_Move_Down;
            break;
        case Qt::Key_A:
            i = NLAction_Move_Left;
            break;
        case Qt::Key_D:
            i = NLAction_Move_Right;
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

void SimpleControl2DComponent::Transform(float delta)
{
    NLActor *actor = Actor();
    if(!actor)
        return;

    if(m_action[NLAction_Move_Left] || m_action[NLAction_Move_Right]
            || m_action[NLAction_Move_Down] || m_action[NLAction_Move_Up]
            )
    {
        vector3_t m_move = VECTOR3(0, 0, 0);
        float movesens = m_moveSens * delta;

        if(m_action[NLAction_Move_Left])
            VECTOR3_X(m_move) = -movesens;
        else if(m_action[NLAction_Move_Right])
            VECTOR3_X(m_move) = movesens;

        if(m_action[NLAction_Move_Down])
            VECTOR3_Y(m_move) = -movesens;
        else if(m_action[NLAction_Move_Up])
            VECTOR3_Y(m_move) = movesens;

        if(m_invertX)
            VECTOR3_X(m_move) = -VECTOR3_X(m_move);
        if(m_invertY)
            VECTOR3_Y(m_move) = -VECTOR3_Y(m_move);
        actor->Move(m_move);
    }
}

void SimpleControl2DComponent::InitProperty()
{
    NLPropperty v;
    v = GetProperty("moveSens");
    if(v.isValid())
        SetMoveSens(v.toFloat());
}

void SimpleControl2DComponent::SetMoveSens(float moveSens)
{
    if(m_moveSens != moveSens)
        m_moveSens = moveSens;
}

float SimpleControl2DComponent::MoveSens() const
{
    return m_moveSens;
}

void SimpleControl2DComponent::SetInvertX(bool b)
{
    if(m_invertX != b)
        m_invertX = b;
}

void SimpleControl2DComponent::SetInvertY(bool b)
{
    if(m_invertY != b)
        m_invertY = b;
}

bool SimpleControl2DComponent::InvertX() const
{
    return m_invertX;
}

bool SimpleControl2DComponent::InvertY() const
{
    return m_invertY;
}
