#ifndef _KARIN_MAPEVENTHANDLERCOMPONENT_H
#define _KARIN_MAPEVENTHANDLERCOMPONENT_H

#include "nlcomponent.h"
#include "gl/nl_gl.h"

class NLRigidbody;
class NLSceneCamera;

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
};

#endif // _KARIN_MAPEVENTHANDLERCOMPONENT_H
