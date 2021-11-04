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
    explicit NLActor(const NLProperties &prop, NLActor *parent = 0);
    explicit NLActor(NLScene *scene, NLActor *parent = 0);
    explicit NLActor(NLScene *scene, const NLProperties &prop, NLActor *parent = 0);
    virtual ~NLActor();
    virtual void Reset();
    void SetRenderable(NLRenderable *renderable);
    NLRenderable * Renderable();
    NLActor * ParentActor();
    void SetParentActor(NLActor *actor);
    NLActorContainer * Container();
    NLSETTER(position) virtual void SetPosition(const NLVector3 &v);
    NLSETTER(rotation) virtual void SetRotation(const NLVector3 &v);
    NLSETTER(scale) virtual void SetScale(const NLVector3 &v);
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
    NLINTERFACE virtual NLActor * Move(const NLVector3 &v);
    NLINTERFACE virtual NLActor * Turn(const NLVector3 &v);
    NLINTERFACE virtual NLActor * Zoom(const NLVector3 &v);
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
    int ChildrenCount() const;
    int ComponentCount() const;
    bool HasChildren() const;
    bool HasComponent() const;
    bool CanRender() const;
    NLINTERFACE virtual NLActor * MoveOriginal(const NLVector3 &v);

protected:
    virtual bool keyev(int key, bool pressed, int modifier);
    virtual bool mouseev(int mouse, bool pressed, int x, int y, int modifier);
    virtual bool motionev(int mouse, bool pressed, int x, int y, int oldx, int oldy, int modifier);
    virtual bool wheelev(int mouse, int orientation, int delta, int x, int y, int modifier);
    virtual void Init();
    virtual void Destroy();
    virtual void Update(float delta);
    void SetContainer(NLActorContainer *container);
    virtual void InitProperty();
    void SetUp(const NLVector3 &up);
    void SetDirection(const NLVector3 &dir);
    void SetRight(const NLVector3 &r);
    void UpdateDirection();
    virtual void UpdateUp();
    
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

private:
    NLRenderable *m_renderable;
    NLComponentContainer *m_components;
    NLActorContainer *m_children;
    NLPROPERTY(NLVector3, position) NLVector3 m_position;
    NLPROPERTY(NLVector3, rotation) NLVector3 m_rotation;
    NLPROPERTY(NLVector3, scale) NLVector3 m_scale;
    NLVector3 m_direction;
    NLVector3 m_up;
    NLVector3 m_right;
    NLMatrix4 m_matrix; // local
    NLMatrix4 m_globalMatrix;
    NLMatrix4 m_normalMatrix; // normal

    friend class NLActorContainer;
    
    Q_DISABLE_COPY(NLActor)
};

typedef QList<NLActor *> NLActorList;

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
