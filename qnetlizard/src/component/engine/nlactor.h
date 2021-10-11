#ifndef _KARIN_NLACTOR_H
#define _KARIN_NLACTOR_H

#include "nlobject.h"

#include "lib/vector3.h"
#include "lib/mesa_gl_math.h"

typedef vector3_s NLVector3;
typedef GLmatrix NLMatrix4;

class NLActorContainer;
class NLRenderable;
class NLComponentContainer;
class NLComponent;

class NLActor : public NLObject
{
    Q_OBJECT
public:
    explicit NLActor(NLActor *parent = 0);
    explicit NLActor(const NLPropperties &prop, NLActor *parent = 0);
    explicit NLActor(NLScene *scene, NLActor *parent = 0);
    explicit NLActor(NLScene *scene, const NLPropperties &prop, NLActor *parent = 0);
    virtual ~NLActor();
    void SetRenderable(NLRenderable *renderable);
    NLRenderable * Renderable();
    NLActor * ParentActor();
    void SetParentActor(NLActor *actor);
    NLActorContainer * Container();
    NLSETTER(position) void SetPosition(const NLVector3 &v);
    NLSETTER(rotation) void SetRotation(const NLVector3 &v);
    NLSETTER(scale) void SetScale(const NLVector3 &v);
    NLGETTER(position) NLVector3 Position() const;
    NLGETTER(rotation) NLVector3 Rotation() const;
    NLGETTER(scale) NLVector3 Scale() const;
    NLGETTER(direction) NLVector3 Direction() const;
    NLGETTER(up) NLVector3 Up() const;
    NLGETTER(matrix) const NLMatrix4 * LocalMatrix() const;
    NLGETTER(globalMatrix) const NLMatrix4 * GlobalMatrix() const;
    NLGETTER(normalMatrix) const NLMatrix4 * NormalMatrix() const;
    NLGETTER(matrix) const NLMatrix4 * Matrix() const { return LocalMatrix(); }
    NLINTERFACE NLActor * Move(const NLVector3 &v);
    NLINTERFACE NLActor * Turn(const NLVector3 &v);
    NLINTERFACE NLActor * Zoom(const NLVector3 &v);


protected:
    virtual bool keyev(int key, bool pressed, int modify);
    virtual bool mouseev(int mouse, bool pressed, int x, int y, int modify);
    virtual bool motionev(int mouse, bool pressed, int x, int y, int oldx, int oldy, int modify);
    virtual bool wheelev(int orientation, int delta, int x, int y, int modify);
    virtual void Init();
    virtual void Destroy();
    virtual void Update(float delta);
    virtual void Render();
    virtual void Reset();
    void SetContainer(NLActorContainer *container);
    bool AddComponent(NLComponent *item);
    bool RemoveComponent(NLComponent *item);
    
signals:
    void positionChanged(const NLVector3 &pos);
    void rotationChanged(const NLVector3 &rot);
    void scaleChanged(const NLVector3 &scale);
    
public slots:

private:
    void Construct();
    void UpdateMatrix();
    void UpdateLocalMatrix();
    void UpdateGlobalMatrix();
    void UpdateNormalMatrix();
    void UpdateChildrenMatrix();
    void UpdateDirection();
    float ClampAngle(float angle);

private:
    NLRenderable *m_renderable;
    NLComponentContainer *m_components;
    NLActorContainer *m_children;
    bool m_zIsUp;
    NLVector3 m_position;
    NLVector3 m_rotation;
    NLVector3 m_scale;
    NLVector3 m_direction;
    NLVector3 m_up;
    NLVector3 m_left;
    NLMatrix4 m_matrix; // local
    NLMatrix4 m_globalMatrix;
    NLMatrix4 m_normalMatrix; // normal

    friend class NLActorContainer;
    
    Q_DISABLE_COPY(NLActor)
};

#endif // _KARIN_NLACTOR_H
