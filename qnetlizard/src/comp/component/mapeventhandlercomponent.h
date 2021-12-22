#ifndef _KARIN_MAPEVENTHANDLERCOMPONENT_H
#define _KARIN_MAPEVENTHANDLERCOMPONENT_H

#include "nlcomponent.h"
#include "gl/nl_gl.h"
#include "bound.h"

class NLRigidbody;
class NLSceneCamera;

class MapEventHandler
{
public:
    enum Handler_State_e
    {
        Handler_Ready = 0,
        Handler_Running,
        Handler_Finished
    };

public:
    explicit MapEventHandler(GL_NETLizard_3D_Mesh *item, NLRigidbody *actor, bool loop = false);
    virtual ~MapEventHandler();
    virtual void Update(float delta) = 0;
    virtual bool Start();
    GL_NETLizard_3D_Mesh * Item() { return m_item; }
    bool Loop() const { return m_loop; }
    bool IsRunning() const { return m_state == Handler_Running; }
    bool IsFinished() const { return m_state == Handler_Finished; }
    bool IsReady() const { return m_state == Handler_Ready; }

protected:
    void SetState(Handler_State_e state);
    NLRigidbody * Actor() { return m_actor; }

private:
    GL_NETLizard_3D_Mesh *m_item;
    bool m_loop;
    Handler_State_e m_state;
    NLRigidbody *m_actor;
};

class MapEventHandler_elevator : public MapEventHandler
{
public:
    enum Elevator_Mask_e
    {
        Elevator_Front = 1,
        Elevator_Back = 2,
        Elevator_Front_And_Back = 3
    };
public:
    explicit MapEventHandler_elevator(float min, float max, Elevator_Mask_e mask, bool invert, GL_NETLizard_3D_Mesh *item, NLRigidbody *actor, bool loop = false);
    virtual ~MapEventHandler_elevator();
    virtual void Update(float delta);
    virtual bool Start();

private:
    enum Elevator_State_e
    {
        Elevator_At_Start = 0,
        Elevator_Moving_Front,
        Elevator_At_End,
        Elevator_Moving_Back
    };

private:
    Elevator_State_e m_elevatorState;
    float m_min;
    float m_max;
    float m_unit;
    int m_mask;
    bool m_invert;
    bound_t m_box;
};

class MapEventHandler_fan : public MapEventHandler
{
public:
    enum Fan_Mask_e
    {
        Fan_Pitch = 1,
        Fan_Yaw = 2,
        Fan_Roll = 4
    };
public:
    explicit MapEventHandler_fan(int mask, bool invert, GL_NETLizard_3D_Mesh *item, NLRigidbody *actor, bool loop = false);
    virtual ~MapEventHandler_fan();
    virtual void Update(float delta);

private:
    float m_xUnit;
    float m_yUnit;
    float m_zUnit;
    int m_mask;
    bool m_invert;
};

class MapEventHandler_door : public MapEventHandler
{
public:
    enum Door_Mask_e
    {
        Door_1 = 1,
        Door_2 = 2,
        Door_1_And_2 = 3
    };
    enum Orientation_e
    {
        Orientation_Vertical = 1,
        Orientation_Horizontal_X = 2,
        Orientation_Horizontal_Y = 3
    };
public:
    explicit MapEventHandler_door(const float min[3], const float max[3], Door_Mask_e mask, Orientation_e orientation, float start2, float end2, GL_NETLizard_3D_Mesh *other, float start1, float end1, GL_NETLizard_3D_Mesh *item, NLRigidbody *actor, bool loop = false);
    virtual ~MapEventHandler_door();
    virtual void Update(float delta);
    virtual bool Start();

private:
    enum Door_State_e
    {
        Door_At_Start = 0,
        Door_Moving_Front,
        Door_At_End,
        Door_Moving_Back
    };
    void UpdateVerticalDoor(float delta);
    void UpdateHorizontalDoorY(float delta);
    void UpdateHorizontalDoorX(float delta);
    bool ActorInBox();
    void UpdateDoor(float delta, int coord);

private:
    GL_NETLizard_3D_Mesh *m_otherPart;
    Door_State_e m_doorState;
    Door_State_e m_otherDoorState;
    bound_t m_box;
    float m_unit;
    int m_mask;
    Orientation_e m_orientation;
    float m_start;
    float m_end;
    float m_start2;
    float m_end2;
};

class MapEventHandler_ladder : public MapEventHandler
{
public:
    enum Ladder_Movment_e
    {
        Ladder_Move_Up = 1,
        Ladder_Move_Down = 2
    };
public:
    explicit MapEventHandler_ladder(Ladder_Movment_e movment, GL_NETLizard_3D_Mesh *item, NLRigidbody *actor, bool loop = false);
    virtual ~MapEventHandler_ladder();
    virtual void Update(float delta);

private:
    float m_unit;
    Ladder_Movment_e m_movment;
};

class MapEventHandlerContainer
{
public:
    explicit MapEventHandlerContainer();
    virtual ~MapEventHandlerContainer();
    void Update(float delta);
    bool Add(int itemIndex, MapEventHandler *item);
    void Clear();
    bool Exists(int item);
    bool Remove(int itemIndex);
    template <class T>
    bool HasType();

private:
    QList<MapEventHandler *> m_handlers;
    QHash<int, MapEventHandler *> m_map;
};

NLCOMPONENT(MapEventHandlerComponent)
class MapEventHandlerComponent : public NLComponent
{
    Q_OBJECT
    Q_PROPERTY(QObject* teleportActor READ TeleportActorObject FINAL)

public:
    explicit MapEventHandlerComponent(const NLProperties &prop = NLProperties(), NLActor *parent = 0);
    virtual ~MapEventHandlerComponent();
    GL_NETLizard_3D_Model * Model() { return m_model; }
    void SetModel(GL_NETLizard_3D_Model *model, int level);
    virtual void Reset();
    bool Trigger(int item);
    bool Collision(int item);
    void SetTeleportActor(NLRigidbody *actor);
    NLRigidbody * TeleportActor() { return m_teleportActor; }
    QObject * TeleportActorObject();
    template <class T>
    bool HasEventHandler();
    
public slots:

protected:
    virtual void Update(float delta);

    private:
    void ElevatorEvent();
    void ConvToAlgoVector3(vector3_t &v);
    void ConvToRenderVector3(vector3_t &v);
    NLSceneCamera * SceneCamera();
    bool HandleElevator(int item);
    bool HandleTeleport(int item);
    bool HandleFan(int item);
    bool HandleDoor(int item);
    bool HandleLadder(int item);

private:
    typedef QHash<NLint, const NETLizard_Level_Teleport *> MapTeleportMap;
    typedef QList<const NETLizard_Level_Elevator *> MapElevatorList;
    typedef QHash<NLint, MapElevatorList> MapElevatorMap;
    typedef QHash<NLint, const NETLizard_Level_Door *> MapDoorMap;
    GL_NETLizard_3D_Model *m_model;
    NLRigidbody *m_teleportActor;
    MapTeleportMap m_teleport;
    MapElevatorMap m_elevator;
    MapDoorMap m_door;
    MapEventHandlerContainer m_handlers;

    Q_DISABLE_COPY(MapEventHandlerComponent)
};

template <class T>
bool MapEventHandlerContainer::HasType()
{
    Q_FOREACH(MapEventHandler *item, m_handlers)
    {
        if(!item)
            continue;
        if(dynamic_cast<T *>(item))
            return true;
    }
    return false;
}

template <class T>
bool MapEventHandlerComponent::HasEventHandler()
{
    return m_handlers.HasType<T>();
}

#endif // _KARIN_MAPEVENTHANDLERCOMPONENT_H
