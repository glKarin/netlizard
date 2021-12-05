#include "mapeventhandlercomponent.h"

#include <QDebug>
#include <QList>
#include <QHash>

#include "settings.h"
#include "qdef.h"
#include "nlscene.h"
#include "nlrigidbody.h"
#include "nlscenecamera.h"
#include "matrix.h"

MapEventHandlerContainer::MapEventHandlerContainer()
{

}

MapEventHandlerContainer::~MapEventHandlerContainer()
{
    Clear();
    DEBUG_DESTROY("MapEventHandlerContainer")
}

bool MapEventHandlerContainer::Add(int itemIndex, MapEventHandler *item)
{
    if(itemIndex < 0 || !item)
        return false;
    if(Exists(itemIndex))
        return false;
    m_handlers.push_back(item);
    m_map.insert(itemIndex, item);
    qDebug() << item->Start();
    return true;
}

void MapEventHandlerContainer::Clear()
{
    QList<MapEventHandler *> list = m_handlers;
    m_handlers.clear();
    Q_FOREACH(MapEventHandler *item, list)
        delete item;
    m_map.clear();
}

void MapEventHandlerContainer::Update(float delta)
{
    QList<MapEventHandler *> list;
    Q_FOREACH(MapEventHandler *item, m_handlers)
    {
        item->Update(delta);
        if(item->IsFinished())
            list.push_back(item);
    }

    while(!list.isEmpty())
    {
        MapEventHandler *item = list[0];
        m_handlers.removeOne(item);
        m_map.remove(m_map.key(item));
        delete item;
        list.removeAt(0);
    }
}

bool MapEventHandlerContainer::Exists(int item)
{
    return m_map.contains(item);
}

MapEventHandler::MapEventHandler(GL_NETLizard_3D_Mesh *item, bool loop)
    : m_item(item),
      m_loop(loop),
      m_state(MapEventHandler::Handler_Ready)
{

}

MapEventHandler::~MapEventHandler()
{
    m_item = 0;
    DEBUG_DESTROY("MapEventHandler")
}

void MapEventHandler::SetState(MapEventHandler::Handler_State_e state)
{
    if(m_state != state)
        m_state = state;
}

bool MapEventHandler::Start()
{
    if(m_state != MapEventHandler::Handler_Ready)
        return false;
    m_state = MapEventHandler::Handler_Running;
    return true;
}

MapEventHandler_elevator::MapEventHandler_elevator(float min, float max, GL_NETLizard_3D_Mesh *item, bool loop)
    : MapEventHandler(item, loop),
      m_elevatorState(MapEventHandler_elevator::Elevator_At_Start),
      m_min(min),
      m_max(max),
      m_unit(200)
{

}

MapEventHandler_elevator::~MapEventHandler_elevator()
{
    DEBUG_DESTROY("MapEventHandler_elevator")
}

bool MapEventHandler_elevator::Start()
{
    return MapEventHandler::Start();
}

void MapEventHandler_elevator::Update(float delta)
{
    if(!IsRunning())
        return;
    GL_NETLizard_3D_Mesh *item = Item();
    bool is_downing = m_elevatorState == MapEventHandler_elevator::Elevator_At_End || m_elevatorState == MapEventHandler_elevator::Elevator_Moving_Down;
    int neg = is_downing ? -1 : 1;
    float length = m_unit * delta * neg;
    float z = item->position[2];
    float newZ = z + length;
    qDebug() << newZ;
    MapEventHandler_elevator::Elevator_State_e newState = m_elevatorState;
    if(is_downing)
    {
        if(newZ < m_min)
        {
            newZ = m_min;
            newState = MapEventHandler_elevator::Elevator_At_Start;
        }
        else
        {
            if(m_elevatorState == MapEventHandler_elevator::Elevator_At_End)
                newState = MapEventHandler_elevator::Elevator_Moving_Down;
        }
    }
    else
    {
        if(newZ > m_max)
        {
            newZ = m_max;
            newState = MapEventHandler_elevator::Elevator_At_End;
        }
        else
        {
            if(m_elevatorState == MapEventHandler_elevator::Elevator_At_Start)
                newState = MapEventHandler_elevator::Elevator_Moving_Up;
        }
    }
    item->position[2] = newZ;

    m_elevatorState = newState;

    if(newState == MapEventHandler_elevator::Elevator_At_Start)
    {
        SetState(MapEventHandler::Handler_Finished);
    }
}





MapEventHandlerComponent::MapEventHandlerComponent(const NLProperties &prop, NLActor *parent) :
    NLComponent(prop, parent),
    m_model(0),
    m_teleportActor(0)
{
    CLASS_NAME(MapEventHandlerComponent);
    setObjectName("MapEventHandlerComponent");
}

MapEventHandlerComponent::~MapEventHandlerComponent()
{

}

void MapEventHandlerComponent::SetTeleportActor(NLRigidbody *actor)
{
    if(m_teleportActor != actor)
        m_teleportActor = actor;
}

QObject * MapEventHandlerComponent::TeleportActor()
{
    return m_teleportActor;
}

void MapEventHandlerComponent::Update(float delta)
{
    if(!IsActived())
        return;
    if(!m_model)
        return;
    m_handlers.Update(delta);
    NLComponent::Update(delta);
}

void MapEventHandlerComponent::Reset()
{
    NLComponent::Reset();
    m_teleport.clear();
    m_model = 0;
}

void MapEventHandlerComponent::SetModel(GL_NETLizard_3D_Model *model, int level)
{
    if(m_model != model)
    {
        m_model = model;
        if(m_model && level >= 0)
        {
            NLint count = 0;
            const NETLizard_Level_Teleport *teleport = nlGet3DGameTeleport(m_model->game, level, -1, &count);
            if(teleport)
            {
                for(int i = 0; i < count; i++)
                {
                    const NETLizard_Level_Teleport *t = teleport + i;
                    for(unsigned j = 0; j < countof(t->item); j++)
                    {
                        if(t->item[j] >= 0)
                            m_teleport.insert(t->item[j], t);
                    }
                }
            }
        }
    }
}

bool MapEventHandlerComponent::Trigger(int item)
{
    if(!m_model)
        return false;
    if(item < 0)
        return false;
 // 26 41
    if(item != 66)
        return false;
    if(m_handlers.Exists(65))
        return false;
    GL_NETLizard_3D_Mesh *mesh = m_model->item_meshes + 65;
    MapEventHandler_elevator *handler = new MapEventHandler_elevator(0, 1200, mesh, false);
    qDebug() << m_handlers.Add(item, handler);
    return true;
}

bool MapEventHandlerComponent::Collision(int item)
{
    //qDebug() << item << m_teleport.keys();
    if(!m_model)
        return false;
    if(item < 0)
        return false;
    if(!m_teleportActor)
        return false;
    if(m_teleport.isEmpty())
        return false;
    if(!m_teleport.contains(item))
        return false;
    NLSceneCamera *camera = SceneCamera();
    if(!camera)
        return false;
    m_teleportActor->Collision();
    const NETLizard_Level_Teleport *teleport = m_teleport[item];
    NLVector3 pos = m_teleportActor->Position();
    ConvToAlgoVector3(pos);
    if(teleport->mask & 1) VECTOR3_X(pos) = teleport->position[0] >> 16;
    if(teleport->mask & 2) VECTOR3_Y(pos) = teleport->position[1] >> 16;
    if(teleport->mask & 4) VECTOR3_Z(pos) = teleport->position[2] >> 16;
    ConvToRenderVector3(pos);
    m_teleportActor->SetPosition(pos);
    NLVector3 rot = m_teleportActor->Rotation();
    if(teleport->mask & 8) VECTOR3_X(rot) = teleport->rotation[1];
    if(teleport->mask & 16) VECTOR3_Y(rot) = teleport->rotation[0];
    m_teleportActor->SetRotation(rot);
    //camera->UpdateCamera();
    return true;
}

NLSceneCamera * MapEventHandlerComponent::SceneCamera()
{
    NLScene *scene = Scene();
    if(!scene)
        return 0;
    return scene->CurrentCamera();
}

void MapEventHandlerComponent::ConvToAlgoVector3(vector3_t &v)
{
    NLSceneCamera *camera = SceneCamera();
    if(!camera)
        return;
    const NLMatrix4 *mat = camera->RenderMatrix();
    matrix_transformv_self_row(mat, &v);
}

void MapEventHandlerComponent::ConvToRenderVector3(vector3_t &v)
{
    NLSceneCamera *camera = SceneCamera();
    if(!camera)
        return;
    const NLMatrix4 *mat = camera->RenderMatrix();
    matrix_transformv_self(mat, &v);
}
