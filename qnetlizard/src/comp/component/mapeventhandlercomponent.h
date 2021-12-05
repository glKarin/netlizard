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
    explicit MapEventHandler(GL_NETLizard_3D_Mesh *item, bool loop = false);
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

private:
    GL_NETLizard_3D_Mesh *m_item;
    bool m_loop;
    Handler_State_e m_state;
};

class MapEventHandler_elevator : public MapEventHandler
{
public:
    enum Elevator_State_e
    {
        Elevator_At_Start = 0,
        Elevator_Moving_Up,
        Elevator_At_End,
        Elevator_Moving_Down
    };
public:
    explicit MapEventHandler_elevator(float min, float max, GL_NETLizard_3D_Mesh *item, bool loop = false);
    virtual ~MapEventHandler_elevator();
    virtual void Update(float delta);
    virtual bool Start();

private:
    Elevator_State_e m_elevatorState;
    float m_min;
    float m_max;
    float m_unit;
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

private:
    QList<MapEventHandler *> m_handlers;
    QHash<int, MapEventHandler *> m_map;
};

NLCOMPONENT(MapEventHandlerComponent)
class MapEventHandlerComponent : public NLComponent
{
    Q_OBJECT
    Q_PROPERTY(QObject* teleportActor READ TeleportActor FINAL)

public:
    explicit MapEventHandlerComponent(const NLProperties &prop = NLProperties(), NLActor *parent = 0);
    virtual ~MapEventHandlerComponent();
    GL_NETLizard_3D_Model * Model() { return m_model; }
    void SetModel(GL_NETLizard_3D_Model *model, int level);
    virtual void Reset();
    bool Trigger(int item);
    bool Collision(int item);
    void SetTeleportActor(NLRigidbody *actor);
    QObject * TeleportActor();
    
public slots:

protected:
    virtual void Update(float delta);

    private:
    void ElevatorEvent();
    void ConvToAlgoVector3(vector3_t &v);
    void ConvToRenderVector3(vector3_t &v);
    NLSceneCamera * SceneCamera();

private:
    typedef QHash<NLint, const NETLizard_Level_Teleport *> MapTeleportMap;
    GL_NETLizard_3D_Model *m_model;
    NLRigidbody *m_teleportActor;
    MapTeleportMap m_teleport;
    MapEventHandlerContainer m_handlers;
};

#endif // _KARIN_MAPEVENTHANDLERCOMPONENT_H
