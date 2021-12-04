#include "mapeventhandlercomponent.h"

#include <QDebug>

#include "settings.h"
#include "qdef.h"
#include "nlscene.h"
#include "nlrigidbody.h"
#include "nlscenecamera.h"
#include "matrix.h"

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

}

bool MapEventHandlerComponent::Collision(int item)
{
    //qDebug() << item << m_teleport.keys();
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
