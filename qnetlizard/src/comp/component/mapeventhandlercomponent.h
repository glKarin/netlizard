#ifndef _KARIN_MAPEVENTHANDLERCOMPONENT_H
#define _KARIN_MAPEVENTHANDLERCOMPONENT_H

#include "nlcomponent.h"
#include "gl/nl_gl.h"

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
    enum Elevator_State_e
    {
        Elevator_At_Start = 0,
        Elevator_Moving_Front,
        Elevator_At_End,
        Elevator_Moving_Back
    };
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
    Elevator_State_e m_elevatorState;
    float m_min;
    float m_max;
    float m_unit;
    int m_mask;
    bool m_invert;
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

private:
    typedef QHash<NLint, const NETLizard_Level_Teleport *> MapTeleportMap;
    typedef QList<const NETLizard_Level_Elevator *> MapElevatorList;
    typedef QHash<NLint, MapElevatorList> MapElevatorMap;
    GL_NETLizard_3D_Model *m_model;
    NLRigidbody *m_teleportActor;
    MapTeleportMap m_teleport;
    MapElevatorMap m_elevator;
    MapEventHandlerContainer m_handlers;
};

#endif // _KARIN_MAPEVENTHANDLERCOMPONENT_H
