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
#include "nlmath.h"

MapEventHandlerContainer::MapEventHandlerContainer()
{

}

MapEventHandlerContainer::~MapEventHandlerContainer()
{
    Clear();
    DEBUG_DESTROY(MapEventHandlerContainer)
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
    DEBUG_DESTROY(MapEventHandler)
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
    NETLizard_GetMeshBound(item, &m_box);
}

MapEventHandler_elevator::~MapEventHandler_elevator()
{
    DEBUG_DESTROY(MapEventHandler_elevator)
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
                if(VECTOR3_Z(pos) >= BOUND_MIN_Z(m_box) && bound_point_in_box2d(&m_box, &pos))
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

    if(m_elevatorState == MapEventHandler_elevator::Elevator_At_Start)
    {
        SetState(MapEventHandler::Handler_Finished);
    }

    if(m_mask == MapEventHandler_elevator::Elevator_Front)
    {
        if(m_elevatorState == MapEventHandler_elevator::Elevator_At_End)
        {
            SetState(MapEventHandler::Handler_Finished);
        }
    }
}

MapEventHandler_fan::MapEventHandler_fan(int mask, bool invert, GL_NETLizard_3D_Mesh *item, NLRigidbody *actor, bool loop)
    : MapEventHandler(item, actor, loop),
      m_xUnit(90),
      m_yUnit(90),
      m_zUnit(90),
      m_mask(mask),
      m_invert(invert)
{

}

MapEventHandler_fan::~MapEventHandler_fan()
{
    DEBUG_DESTROY(MapEventHandler_fan)
}

void MapEventHandler_fan::Update(float delta)
{
    if(!IsRunning())
        return;

    GL_NETLizard_3D_Mesh *item = Item();

    int neg = m_invert ? -1 : 1;
    //qDebug() << m_invert << is_downing << newZ;

    if(m_mask & MapEventHandler_fan::Fan_Pitch)
    {
        float xRadius = m_xUnit * delta * neg;
        item->rotation[0] = NL::clamp_angle(item->rotation[0] + xRadius);
    }

    if(m_mask & MapEventHandler_fan::Fan_Yaw)
    {
        float yRadius = m_yUnit * delta * neg;
        item->rotation[1] = NL::clamp_angle(item->rotation[1] + yRadius);
    }

    if(m_mask & MapEventHandler_fan::Fan_Roll)
    {
        float zRadius = m_zUnit * delta * neg;
        item->rotation[2] = NL::clamp_angle(item->rotation[2] + zRadius);
    }
}

MapEventHandler_door::MapEventHandler_door(const float min[3], const float max[3], Door_Mask_e mask, MapEventHandler_door::Orientation_e orientation, float start2, float end2, GL_NETLizard_3D_Mesh *other, float start1, float end1, GL_NETLizard_3D_Mesh *item, NLRigidbody *actor, bool loop)
    : MapEventHandler(item, actor, loop),
      m_otherPart(other),
      m_doorState(MapEventHandler_door::Door_At_Start),
      m_otherDoorState(MapEventHandler_door::Door_At_Start),
      m_unit(500),
      m_mask(mask),
      m_orientation(orientation),
      m_start(start1),
      m_end(end1),
      m_start2(start2),
      m_end2(end2)
{
    vector3_t b[2] = {VECTOR3V(min), VECTOR3V(max)};
    bound_make(&m_box, b, b + 1);

    vector3_t ex = VECTOR3(0, 0, 0);
    if(m_orientation == MapEventHandler_door::Orientation_Horizontal_X)
        VECTOR3_Y(ex) = BOUND_MAX_Y(m_box) - BOUND_MIN_Y(m_box);
    else if(m_orientation == MapEventHandler_door::Orientation_Horizontal_Y)
        VECTOR3_X(ex) = BOUND_MAX_X(m_box) - BOUND_MIN_X(m_box);
    else
    {
        float f = (BOUND_MAX_Z(m_box) - BOUND_MIN_Z(m_box)) / 2;
        if((BOUND_MAX_X(m_box) - BOUND_MIN_X(m_box)) > (BOUND_MAX_Y(m_box) - BOUND_MIN_Y(m_box)))
            VECTOR3_Y(ex) = f;
        else
            VECTOR3_X(ex) = f;
    }
    bound_expand(&m_box, &ex);
}

MapEventHandler_door::~MapEventHandler_door()
{
    DEBUG_DESTROY(MapEventHandler_door)
}

bool MapEventHandler_door::Start()
{
    bool res = MapEventHandler::Start();
    if(!res)
        return false;
    m_doorState = MapEventHandler_door::Door_Moving_Front;
    m_otherDoorState = MapEventHandler_door::Door_Moving_Front;
    return res;
}

void MapEventHandler_door::Update(float delta)
{
    if(!IsRunning())
        return;
    if(m_orientation == MapEventHandler_door::Orientation_Horizontal_X)
        UpdateHorizontalDoorX(delta);
    else if(m_orientation == MapEventHandler_door::Orientation_Horizontal_Y)
        UpdateHorizontalDoorY(delta);
    else
        UpdateVerticalDoor(delta);

    if(m_doorState == MapEventHandler_door::Door_At_Start && m_otherDoorState == MapEventHandler_door::Door_At_Start)
    {
        SetState(MapEventHandler::Handler_Finished);
    }
}

bool MapEventHandler_door::ActorInBox()
{
    NLRigidbody *actor = Actor();
    NLScene *scene = actor->Scene();
    NLSceneCamera *camera = scene->CurrentCamera();
    bool inBox = false;
    if(camera)
    {
        nl_vector3_t pos = actor->Position();
        matrix_transformv_self_row(camera->RenderMatrix(), &pos);
        inBox = bound_point_in_box(&m_box, &pos) ? true : false;
    }
    return inBox;
}

void MapEventHandler_door::UpdateDoor(float delta, int coord)
{
    const bool inBox = ActorInBox();

    if((m_mask & MapEventHandler_door::Door_1) && m_doorState != MapEventHandler_door::Door_At_Start)
    {
        GL_NETLizard_3D_Mesh *item = Item();

        bool is_downing = !inBox && (m_doorState == MapEventHandler_door::Door_At_End || m_doorState == MapEventHandler_door::Door_Moving_Back);
        int neg = is_downing ? -1 : 1;
        float length = m_unit * delta * neg;
        float z = item->position[coord];
        float newZ = z + length;
        MapEventHandler_door::Door_State_e newState = m_doorState;
        if(is_downing)
        {
            if(newZ < m_start)
            {
                newZ = m_start;
                newState = MapEventHandler_door::Door_At_Start;
            }
            else
            {
                //if(m_doorState == MapEventHandler_door::Door_At_End)
                    newState = MapEventHandler_door::Door_Moving_Back;
            }
        }
        else // up
        {
            if(newZ > m_end)
            {
                newZ = m_end;
                newState = MapEventHandler_door::Door_At_End;
            }
            else
            {
                //if(m_doorState == MapEventHandler_door::Door_At_Start)
                    newState = MapEventHandler_door::Door_Moving_Front;
            }
        }
        item->position[coord] = newZ;

        m_doorState = newState;
    }

    // other part
    if((m_mask & MapEventHandler_door::Door_2) && m_otherPart && (m_otherDoorState != MapEventHandler_door::Door_At_Start))
    {
        bool is_downing = !inBox && (m_otherDoorState == MapEventHandler_door::Door_At_End || m_otherDoorState == MapEventHandler_door::Door_Moving_Back);
        is_downing = !is_downing;
        int neg = is_downing ? -1 : 1;
        float length = m_unit * delta * neg;
        float z = m_otherPart->position[coord];
        float newZ = z + length;
        MapEventHandler_door::Door_State_e newState = m_otherDoorState;
        if(is_downing)
        {
            if(newZ < m_end2)
            {
                newZ = m_end2;
                newState = MapEventHandler_door::Door_At_End;
            }
            else
            {
                //if(m_otherDoorState == MapEventHandler_door::Door_At_Start)
                    newState = MapEventHandler_door::Door_Moving_Front;
            }
        }
        else // up
        {
            if(newZ > m_start2)
            {
                newZ = m_start2;
                newState = MapEventHandler_door::Door_At_Start;
            }
            else
            {
                //if(m_otherDoorState == MapEventHandler_door::Door_At_End)
                    newState = MapEventHandler_door::Door_Moving_Back;
            }
        }
        m_otherPart->position[coord] = newZ;

        m_otherDoorState = newState;
    }
}

void MapEventHandler_door::UpdateHorizontalDoorX(float delta)
{
    UpdateDoor(delta, 0);
}

void MapEventHandler_door::UpdateHorizontalDoorY(float delta)
{
    UpdateDoor(delta, 1);
}

void MapEventHandler_door::UpdateVerticalDoor(float delta)
{
    UpdateDoor(delta, 2);
}


MapEventHandler_ladder::MapEventHandler_ladder(MapEventHandler_ladder::Ladder_Movment_e movment, GL_NETLizard_3D_Mesh *item, NLRigidbody *actor, bool loop)
    : MapEventHandler(item, actor, loop),
      m_unit(500),
      m_movment(movment)
{
}

MapEventHandler_ladder::~MapEventHandler_ladder()
{
    DEBUG_DESTROY(MapEventHandler_ladder)
}

void MapEventHandler_ladder::Update(float delta)
{
    if(!IsRunning())
        return;
    NLRigidbody *actor = Actor();
    NLScene *scene = actor->Scene();
    NLSceneCamera *camera = scene->CurrentCamera();
    if(camera)
    {
        nl_vector3_t pos = actor->Position();
        matrix_transformv_self_row(camera->RenderMatrix(), &pos);
        VECTOR3_Z(pos) += (m_unit * delta * (m_movment == MapEventHandler_ladder::Ladder_Move_Down ? -1 : 1));
        matrix_transformv_self(camera->RenderMatrix(), &pos);
        actor->SetPosition(pos);
    }
    SetState(MapEventHandler::Handler_Finished);
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
    m_door.clear();
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
                    m_teleport.insert(t->item, t);
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
            const NETLizard_Level_Door *door = nlGet3DGameDoor(m_model->game, level, -1, &count);
            if(door)
            {
                for(int i = 0; i < count; i++)
                {
                    const NETLizard_Level_Door *d = door + i;
                    for(unsigned j = 0; j < countof(d->item); j++)
                    {
                        if(d->item[j].item >= 0)
                            m_door.insert(d->item[j].item, d);
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
    const GL_NETLizard_3D_Mesh *mesh = m_model->item_meshes + item;
    if(mesh->item_type & NL_3D_ITEM_TYPE_SWITCH)
        return HandleElevator(item);
    else if(mesh->item_type & NL_3D_ITEM_TYPE_PORTAL)
    {
        if((mesh->item_type & NL_3D_ITEM_TYPE_FAN_Z_AXIS) == 0) // egypt 3d teleporter
            return HandleTeleport(item);
        else
            return HandleFan(item);
    }
    else if((mesh->item_type & NL_3D_ITEM_TYPE_FAN_Y_AXIS) || (mesh->item_type & NL_3D_ITEM_TYPE_FAN_Z_AXIS) || (mesh->item_type & NL_3D_ITEM_TYPE_FAN_X_AXIS))
        return HandleFan(item);
    else if((mesh->item_type & NL_3D_ITEM_TYPE_DOOR_VERTICAL) || (mesh->item_type & NL_3D_ITEM_TYPE_DOOR_HORIZONTAL))
        return HandleDoor(item);
    return false;
}

bool MapEventHandlerComponent::Collision(int item)
{
    if(!m_model)
        return false;
    if(item < 0)
        return false;
    const GL_NETLizard_3D_Mesh *mesh = m_model->item_meshes + item;
    if(mesh->item_type & NL_3D_ITEM_TYPE_PORTAL)
        return HandleTeleport(item);
    else if((mesh->item_type & NL_3D_ITEM_TYPE_DOOR_VERTICAL) || (mesh->item_type & NL_3D_ITEM_TYPE_DOOR_HORIZONTAL))
        return HandleDoor(item);
    else if(mesh->item_type & NL_3D_ITEM_TYPE_LADDER)
        return HandleLadder(item);
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
            m_handlers.Remove(elevator_item);
        int mask = 0;
        if(elevator->mask & 1)
            mask |= MapEventHandler_elevator::Elevator_Front;
        if(elevator->mask & 2)
            mask |= MapEventHandler_elevator::Elevator_Back;
        GL_NETLizard_3D_Mesh *mesh = m_model->item_meshes + elevator->elevator_item;
        MapEventHandler_elevator *handler = new MapEventHandler_elevator(elevator->min, elevator->max, static_cast<MapEventHandler_elevator::Elevator_Mask_e>(mask), elevator->invert ? true : false, mesh, m_teleportActor, false);
        m_handlers.Add(elevator_item, handler);
    }

    return true;
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

bool MapEventHandlerComponent::HandleFan(int item)
{
    if(m_handlers.Exists(item))
    {
        m_handlers.Remove(item);
        return true;
    }
    GL_NETLizard_3D_Mesh *mesh = m_model->item_meshes + item;
    int mask = 0;
    if(mesh->item_type & NL_3D_ITEM_TYPE_FAN_Y_AXIS)
        mask |= MapEventHandler_fan::Fan_Roll;
    if(mesh->item_type & NL_3D_ITEM_TYPE_FAN_Z_AXIS)
        mask |= MapEventHandler_fan::Fan_Yaw;
    if(mesh->item_type & NL_3D_ITEM_TYPE_FAN_X_AXIS)
        mask |= MapEventHandler_fan::Fan_Pitch;
    MapEventHandler_fan *handler = new MapEventHandler_fan(mask, false, mesh, m_teleportActor, false);
    m_handlers.Add(item, handler);

    return true;
}

bool MapEventHandlerComponent::HandleDoor(int item)
{
    if(m_door.isEmpty())
        return false;
    if(!m_door.contains(item))
        return false;
    const NETLizard_Level_Door *door = m_door[item];
    int door_item = door->item[0].item;
    if(m_handlers.Exists(door_item))
        m_handlers.Remove(door_item);
    int mask = MapEventHandler_door::Door_1;
    GL_NETLizard_3D_Mesh *mesh = m_model->item_meshes + door_item;
    GL_NETLizard_3D_Mesh *otherMesh = 0;
    if(door->item[1].item >= 0)
    {
        otherMesh = m_model->item_meshes + door->item[1].item;
        mask |= MapEventHandler_door::Door_2;
    }
    MapEventHandler_door::Orientation_e orientation;
    switch(door->orientation)
    {
    case 2:
        orientation = MapEventHandler_door::Orientation_Horizontal_X;
        break;
    case 3:
        orientation = MapEventHandler_door::Orientation_Horizontal_Y;
        break;
    case 1:
    default:
        orientation = MapEventHandler_door::Orientation_Vertical;
        break;
    }

    const float min[3] = {door->box.min[0], door->box.min[1], door->box.min[2]};
    const float max[3] = {door->box.max[0], door->box.max[1], door->box.max[2]};
    MapEventHandler_door *handler = new MapEventHandler_door(min, max, static_cast<MapEventHandler_door::Door_Mask_e>(mask), orientation, door->item[1].start, door->item[1].end, otherMesh, door->item[0].start, door->item[0].end, mesh, m_teleportActor, false);
    m_handlers.Add(door_item, handler);

    return true;
}

bool MapEventHandlerComponent::HandleLadder(int item)
{
    if(m_handlers.Exists(item))
        m_handlers.Remove(item);
    GL_NETLizard_3D_Mesh *mesh = m_model->item_meshes + item;
    NLScene *scene = Scene();
    if(!scene->KeyState(Qt::Key_W) && !scene->KeyState(Qt::Key_S) && !scene->KeyState(Qt::Key_A) && !scene->KeyState(Qt::Key_D))
        return false;
    MapEventHandler_ladder::Ladder_Movment_e m = scene && scene->KeyState(Qt::Key_Q) ? MapEventHandler_ladder::Ladder_Move_Down : MapEventHandler_ladder::Ladder_Move_Up;
    MapEventHandler_ladder *handler = new MapEventHandler_ladder(m, mesh, m_teleportActor, false);
    m_handlers.Add(item, handler);

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
