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
#include "bound.h"
#include "nl_util.h"

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
    return item->Start();
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

bool MapEventHandlerContainer::Remove(int itemIndex)
{
    if(!m_map.contains(itemIndex))
        return false;
    MapEventHandler *item = m_map[itemIndex];
    m_handlers.removeOne(item);
    m_map.remove(itemIndex);
    delete item;
    return true;
}

MapEventHandler::MapEventHandler(GL_NETLizard_3D_Mesh *item, NLRigidbody *actor, bool loop)
    : m_item(item),
      m_loop(loop),
      m_state(MapEventHandler::Handler_Ready),
      m_actor(actor)
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

MapEventHandler_elevator::MapEventHandler_elevator(float min, float max, MapEventHandler_elevator::Elevator_Mask_e mask, bool invert, GL_NETLizard_3D_Mesh *item, NLRigidbody *actor, bool loop)
    : MapEventHandler(item, actor, loop),
      m_elevatorState(MapEventHandler_elevator::Elevator_At_Start),
      m_min(min),
      m_max(max),
      m_unit(500),
      m_mask(mask),
      m_invert(invert)
{

}

MapEventHandler_elevator::~MapEventHandler_elevator()
{
    DEBUG_DESTROY("MapEventHandler_elevator")
}

bool MapEventHandler_elevator::Start()
{
    bool res = MapEventHandler::Start();
    if(!res)
        return false;
    if(m_mask == MapEventHandler_elevator::Elevator_Back)
        m_elevatorState = MapEventHandler_elevator::Elevator_At_End;
    return res;
}

void MapEventHandler_elevator::Update(float delta)
{
    if(!IsRunning())
        return;

    GL_NETLizard_3D_Mesh *item = Item();
    // if actor on evelator when elevator is transform ending, stop update
    if(m_mask == MapEventHandler_elevator::Elevator_Front_And_Back)
    {
        if(m_elevatorState == MapEventHandler_elevator::Elevator_At_End)
        {
            NLRigidbody *actor = Actor();
            NLScene *scene = actor->Scene();
            NLSceneCamera *camera = scene->CurrentCamera();
            if(camera)
            {
                nl_vector3_t pos = actor->Position();
                matrix_transformv_self_row(camera->RenderMatrix(), &pos);
                bound_t box;
                NETLizard_GetSceneBound(item, &box);
                if(VECTOR3_Z(pos) > BOUND_MAX_Z(box) && bound_point_in_box2d(&box, &pos))
                {
                    return;
                }
            }
        }
    }

    bool is_downing = m_elevatorState == MapEventHandler_elevator::Elevator_At_End || m_elevatorState == MapEventHandler_elevator::Elevator_Moving_Back;
    if(m_invert)
        is_downing = !is_downing;
    int neg = is_downing ? -1 : 1;
    float length = m_unit * delta * neg;
    float z = item->position[2];
    float newZ = z + length;
    //qDebug() << m_invert << is_downing << newZ;
    MapEventHandler_elevator::Elevator_State_e newState = m_elevatorState;
    if(is_downing)
    {
        if(newZ < m_min)
        {
            newZ = m_min;
            newState = m_invert ? MapEventHandler_elevator::Elevator_At_End : MapEventHandler_elevator::Elevator_At_Start;
        }
        else
        {
            if(m_invert)
            {
                if(m_elevatorState == MapEventHandler_elevator::Elevator_At_Start && (m_mask & MapEventHandler_elevator::Elevator_Front))
                    newState = MapEventHandler_elevator::Elevator_Moving_Front;
            }
            else
            {
                if(m_elevatorState == MapEventHandler_elevator::Elevator_At_End && (m_mask & MapEventHandler_elevator::Elevator_Back))
                    newState = MapEventHandler_elevator::Elevator_Moving_Back;
            }
        }
    }
    else // up
    {
        if(newZ > m_max)
        {
            newZ = m_max;
            newState = m_invert ? MapEventHandler_elevator::Elevator_At_Start : MapEventHandler_elevator::Elevator_At_End;
        }
        else
        {
            if(m_invert)
            {
                if(m_elevatorState == MapEventHandler_elevator::Elevator_At_End && (m_mask & MapEventHandler_elevator::Elevator_Back))
                    newState = MapEventHandler_elevator::Elevator_Moving_Back;
            }
            else
            {
                if(m_elevatorState == MapEventHandler_elevator::Elevator_At_Start && (m_mask & MapEventHandler_elevator::Elevator_Front))
                    newState = MapEventHandler_elevator::Elevator_Moving_Front;
            }
        }
    }
    item->position[2] = newZ;

    m_elevatorState = newState;

    if(newState == MapEventHandler_elevator::Elevator_At_Start)
    {
        SetState(MapEventHandler::Handler_Finished);
    }

    if(m_mask == MapEventHandler_elevator::Elevator_Front)
    {
        if(newState == MapEventHandler_elevator::Elevator_At_End)
        {
            SetState(MapEventHandler::Handler_Finished);
        }
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

QObject * MapEventHandlerComponent::TeleportActorObject()
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
    m_model = 0;
    m_handlers.Clear();
    m_teleport.clear();
    m_elevator.clear();
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
            const NETLizard_Level_Elevator *elevator = nlGet3DGameElevator(m_model->game, level, -1, &count);
            if(elevator)
            {
                for(int i = 0; i < count; i++)
                {
                    const NETLizard_Level_Elevator *e = elevator + i;
                    if(!m_elevator.contains(e->switch_item))
                        m_elevator.insert(e->switch_item, MapEventHandlerComponent::MapElevatorList());
                    m_elevator[e->switch_item].push_back(e);
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
    const GL_NETLizard_3D_Mesh *mesh = m_model->item_meshes + item;
    if(mesh->item_type & NL_3D_ITEM_TYPE_SWITCH)
        return HandleElevator(item);
    else if(mesh->item_type & NL_3D_ITEM_TYPE_PORTAL)
        return HandleTeleport(item);
    return false;
}

bool MapEventHandlerComponent::HandleElevator(int item)
{
    if(m_elevator.isEmpty())
        return false;
    if(!m_elevator.contains(item))
        return false;
    const MapEventHandlerComponent::MapElevatorList &elevators = m_elevator[item];
    Q_FOREACH(const NETLizard_Level_Elevator *elevator, elevators)
    {
        int elevator_item = elevator->elevator_item;
        if(m_handlers.Exists(elevator_item))
            qDebug() << "rm -> " << m_handlers.Remove(elevator_item);
        int mask = 0;
        if(elevator->mask & 1)
            mask |= MapEventHandler_elevator::Elevator_Front;
        if(elevator->mask & 2)
            mask |= MapEventHandler_elevator::Elevator_Back;
        GL_NETLizard_3D_Mesh *mesh = m_model->item_meshes + elevator->elevator_item;
        MapEventHandler_elevator *handler = new MapEventHandler_elevator(elevator->min, elevator->max, static_cast<MapEventHandler_elevator::Elevator_Mask_e>(mask), elevator->invert ? true : false, mesh, m_teleportActor, false);
        qDebug() << "add -> " << m_handlers.Add(elevator_item, handler);
    }

    return true;
}

bool MapEventHandlerComponent::Collision(int item)
{
    if(!m_model)
        return false;
    if(item < 0)
        return false;
    const GL_NETLizard_3D_Mesh *mesh = m_model->item_meshes + item;
    //qDebug() << item << m_teleport.keys() << mesh->item_type;
    if(mesh->item_type & NL_3D_ITEM_TYPE_PORTAL)
        return HandleTeleport(item);
    return false;
}

bool MapEventHandlerComponent::HandleTeleport(int item)
{
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
    const NLSceneCamera *camera = SceneCamera();
    if(!camera)
        return;
    const NLMatrix4 *mat = camera->RenderMatrix();
    matrix_transformv_self_row(mat, &v);
}

void MapEventHandlerComponent::ConvToRenderVector3(vector3_t &v)
{
    const NLSceneCamera *camera = SceneCamera();
    if(!camera)
        return;
    const NLMatrix4 *mat = camera->RenderMatrix();
    matrix_transformv_self(mat, &v);
}
