#ifndef _KARIN_NLACTOR_H
#define _KARIN_NLACTOR_H

#include "nlobject.h"

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
    virtual void Reset();
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
    NLGETTER(right) NLVector3 Right() const;
    NLGETTER(matrix) const NLMatrix4 * LocalMatrix() const;
    NLGETTER(globalMatrix) const NLMatrix4 * GlobalMatrix() const;
    NLGETTER(normalMatrix) const NLMatrix4 * NormalMatrix() const;
    NLGETTER(matrix) const NLMatrix4 * Matrix() const { return LocalMatrix(); }
    NLINTERFACE NLActor * Move(const NLVector3 &v);
    NLINTERFACE NLActor * Turn(const NLVector3 &v);
    NLINTERFACE NLActor * Zoom(const NLVector3 &v);
    bool AddComponent(NLComponent *item);
    bool RemoveComponent(NLComponent *item);
    bool RemoveComponent(int index);
    bool RemoveComponent(const NLName &name);
    bool AddChild(NLActor *actor);
    bool RemoveChild(NLActor *actor);
    bool RemoveChild(int index);
    bool RemoveChild(const NLName &name);
    NLActor * GetChild(const NLName &name);
    NLActor * GetChild(int index);
    NLComponent * GetComponent(const NLName &name);
    NLComponent * GetComponent(int index);
    NLActor & operator<<(NLActor *actor);
    NLActor * operator[](int index);
    NLActor * operator[](const NLName &name);
    template <class T>
    T * GetChild_T(const NLName &name);
    template <class T>
    T * GetChild_T(int index);
    template <class T>
    T * GetComponent_T(const NLName &name);
    template <class T>
    T * GetComponent_T(int index);
    friend NLActor & operator+(NLActor &actor, NLComponent *item);
    friend NLActor & operator-(NLActor &actor, NLComponent *item);
    virtual void Render();

protected:
    virtual bool keyev(int key, bool pressed, int modifier);
    virtual bool mouseev(int mouse, bool pressed, int x, int y, int modifier);
    virtual bool motionev(int mouse, bool pressed, int x, int y, int oldx, int oldy, int modifier);
    virtual bool wheelev(int mouse, int orientation, int delta, int x, int y, int modifier);
    virtual void Init();
    virtual void Destroy();
    virtual void Update(float delta);
    void SetContainer(NLActorContainer *container);
    void SetZIsUp(bool b);
    void SetFixedUp(bool b);
    virtual void InitProperty();
    
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

private:
    NLRenderable *m_renderable;
    NLComponentContainer *m_components;
    NLActorContainer *m_children;
    bool m_zIsUp;
    bool m_fixedUp;
    NLVector3 m_position;
    NLVector3 m_rotation;
    NLVector3 m_scale;
    NLVector3 m_direction;
    NLVector3 m_up;
    NLVector3 m_right;
    NLMatrix4 m_matrix; // local
    NLMatrix4 m_globalMatrix;
    NLMatrix4 m_normalMatrix; // normal

    friend class NLActorContainer;
    
    Q_DISABLE_COPY(NLActor)
};

template <class T>
T * NLActor::GetChild_T(const NLName &name)
{
    NLActor *obj = GetChild(name);
    if(!obj)
        return 0;
    return dynamic_cast<T *>(obj);
}

template <class T>
T * NLActor::GetChild_T(int index)
{
    NLActor *obj = GetChild(index);
    if(!obj)
        return 0;
    return dynamic_cast<T *>(obj);
}

template <class T>
T * NLActor::GetComponent_T(const NLName &name)
{
    NLComponent *obj = GetComponent(name);
    if(!obj)
        return 0;
    return dynamic_cast<T *>(obj);
}

template <class T>
T * NLActor::GetComponent_T(int index)
{
    NLComponent *obj = GetComponent(index);
    if(!obj)
        return 0;
    return dynamic_cast<T *>(obj);
}

#endif // _KARIN_NLACTOR_H
