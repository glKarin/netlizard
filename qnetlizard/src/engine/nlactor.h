#ifndef _KARIN_NLACTOR_H
#define _KARIN_NLACTOR_H

#include "nlobject.h"

#include "nlactorcontainer.h"
#include "nlcomponentcontainer.h"
#include "nlscriptcontainer.h"

class NLRenderable;
class NLComponent;
class NLScript;

class NLActor : public NLObject
{
    Q_OBJECT
    Q_PROPERTY(void* renderable READ RenderablePtr FINAL)
    Q_PROPERTY(NLVector3 position READ Position WRITE SetPosition NOTIFY positionChanged FINAL)
    Q_PROPERTY(NLVector3 rotation READ Rotation WRITE SetRotation NOTIFY rotationChanged FINAL)
    Q_PROPERTY(NLVector3 scale READ Scale WRITE SetScale NOTIFY scaleChanged FINAL)

public:
    explicit NLActor(NLActor *parent = 0);
    explicit NLActor(const NLProperties &prop, NLActor *parent = 0);
    explicit NLActor(NLScene *scene, NLActor *parent = 0);
    explicit NLActor(NLScene *scene, const NLProperties &prop, NLActor *parent = 0);
    virtual ~NLActor();
    virtual void Reset();
    void SetRenderable(NLRenderable *renderable);
    NLRenderable * Renderable() { return m_renderable; }
    void * RenderablePtr() { return (void *)m_renderable; }
    NLActor * ParentActor();
    void SetParentActor(NLActor *actor);
    NLActorContainer * Container();
    NLSETTER(position) virtual void SetPosition(const NLVector3 &v);
    NLSETTER(rotation) virtual void SetRotation(const NLVector3 &v);
    NLSETTER(scale) virtual void SetScale(const NLVector3 &v);
    NLGETTER(position) NLVector3 Position() const { return m_position; }
    NLGETTER(rotation) NLVector3 Rotation() const { return m_rotation; }
    NLGETTER(scale) NLVector3 Scale() const { return m_scale; }
    NLGETTER(direction) NLVector3 Direction() const { return m_direction; }
    NLGETTER(up) NLVector3 Up() const { return m_up; }
    NLGETTER(right) NLVector3 Right() const { return m_right; }
    NLGETTER(matrix) const NLMatrix4 * LocalMatrix() const { return &m_matrix; }
    NLGETTER(globalMatrix) const NLMatrix4 * GlobalMatrix() const { return &m_globalMatrix; }
    NLGETTER(normalMatrix) const NLMatrix4 * NormalMatrix() const { return &m_normalMatrix; }
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
    NLActor * operator[](int index) { return GetChild(index); }
    NLActor * operator[](const NLName &name) { return GetChild(name); }
    template <class T>
    T * GetChild_T(const NLName &name);
    template <class T>
    T * GetChild_T(int index);
    template <class T>
    T * GetComponent_T(const NLName &name);
    template <class T>
    T * GetComponent_T(int index);
    template <class T>
    T * GetScript_T(const NLName &name);
    template <class T>
    T * GetScript_T(int index);
    friend NLActor & operator+(NLActor &actor, NLComponent *item) { actor.AddComponent(item); return actor; }
    friend NLActor & operator-(NLActor &actor, NLComponent *item) { actor.RemoveComponent(item); return actor; }
    virtual void Render();
    int ChildrenCount() const;
    int ComponentCount() const;
    bool HasChildren() const { return ChildrenCount() > 0; }
    bool HasComponents() const { return ComponentCount() > 0; }
    bool CanRender() const { return m_renderable != 0; }
    NLINTERFACE virtual NLActor * MoveOriginal(const NLVector3 &v); // original
    NLINTERFACE virtual NLActor * MoveDirection(float len, const NLVector3 &dir); // local
    NLINTERFACE virtual NLActor * MoveDirectionOriginal(float len, const NLVector3 &dir); // original
    int ChildrenTotalCount() const;

    template <class T>
    bool ChildIsType(int index) const;
    template <class T>
    bool ChildIsType(const NLName &name) const;
    template <class T>
    int TypeChildCount() const;
    template <class T>
    bool HasTypeChild() const;
    template <class T>
    T * GetTypeChild(int index = 0);
    template <class T>
    QList<T *> GetTypeChildren();
    template <class T>
    bool RemoveTypeChild();
    template <class T>
    int RemoveTypeChildren();

    template <class T>
    bool ComponentIsType(int index) const;
    template <class T>
    bool ComponentIsType(const NLName &name) const;
    template <class T>
    int TypeComponentCount() const;
    template <class T>
    bool HasTypeComponent() const;
    template <class T>
    T * GetTypeComponent(int index = 0);
    template <class T>
    QList<T *> GetTypeComponents();
    template <class T>
    bool RemoveTypeComponent();
    template <class T>
    int RemoveTypeComponents();

    bool AddScript(NLScript *item);
    bool RemoveScript(NLScript *item);
    bool RemoveScript(int index);
    bool RemoveScript(const NLName &name);
    NLScript * GetScript(const NLName &name);
    NLScript * GetScript(int index);
    friend NLActor & operator+(NLActor &actor, NLScript *item) { actor.AddScript(item); return actor; }
    friend NLActor & operator-(NLActor &actor, NLScript *item) { actor.RemoveScript(item); return actor; }
    int ScriptCount() const;
    bool HasScripts() const { return ScriptCount() > 0; }

    template <class T>
    bool ScriptIsType(int index) const;
    template <class T>
    bool ScriptIsType(const NLName &name) const;
    template <class T>
    int TypeScriptCount() const;
    template <class T>
    bool HasTypeScript() const;
    template <class T>
    T * GetTypeScript(int index = 0);
    template <class T>
    QList<T *> GetTypeScripts();
    template <class T>
    bool RemoveTypeScript();
    template <class T>
    int RemoveTypeScripts();

    NLActor * CreateChild(const NLProperties &props = NLProperties());
    NLComponent * CreateComponent(const NLProperties &props = NLProperties());
    NLScript * CreateScript(const NLProperties &props = NLProperties());

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
    void childChanged(const NLActor *actor = 0);
    void componentChanged(const NLComponent *comp = 0);
    void scriptChanged(const NLScript *script = 0);
    
public slots:

private:
    void Construct();
    void UpdateMatrix();
    void UpdateLocalMatrix();
    void UpdateGlobalMatrix();
    void UpdateNormalMatrix();
    void UpdateChildrenMatrix();
    void TellChildRemoved() { emit childChanged(); }
    void TellComponentRemoved() { emit componentChanged(); }
    void TellScriptRemoved() { emit scriptChanged(); }

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
    NLScriptContainer *m_scripts;

    friend class NLActorContainer;
    friend class NLComponentContainer;
    friend class NLScriptContainer;
    friend class NLScript;
    friend class NLComponent;
    
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

template <class T>
bool NLActor::ChildIsType(int index) const
{
    if(!m_children)
        return false;
    return m_children->IsType<T>(index);
}

template <class T>
bool NLActor::ChildIsType(const NLName &name) const
{
    if(!m_children)
        return false;
    return m_children->IsType<T>(name);
}

template <class T>
int NLActor::TypeChildCount() const
{
    if(!m_children)
        return 0;
    return m_children->TypeCount<T>();
}

template <class T>
bool NLActor::HasTypeChild() const
{
    if(!m_children)
        return false;
    return m_children->HasType<T>();
}

template <class T>
T * NLActor::GetTypeChild(int index)
{
    if(!m_children)
        return 0;
    return m_children->GetType<T>(index);
}

template <class T>
QList<T *> NLActor::GetTypeChildren()
{
    if(!m_children)
        return 0;
    return m_children->GetTypes<T>();
}

template <class T>
bool NLActor::RemoveTypeChild()
{
    if(!m_children)
        return false;
    T *r = m_children->GetType<T>();
    if(!r)
        return false;
    return RemoveChild(r);
}

template <class T>
int NLActor::RemoveTypeChildren()
{
    if(!m_children)
        return 0;
    QList<T *> list = m_children->GetTypes<T>();
    int c = 0;
    Q_FOREACH(T *obj, list)
    {
        if(RemoveChild(obj))
            c++;
    }
    return c;
}

template <class T>
bool NLActor::ComponentIsType(int index) const
{
    if(!m_components)
        return false;
    return m_components->IsType<T>(index);
}

template <class T>
bool NLActor::ComponentIsType(const NLName &name) const
{
    if(!m_components)
        return false;
    return m_components->IsType<T>(name);
}

template <class T>
int NLActor::TypeComponentCount() const
{
    if(!m_components)
        return 0;
    return m_components->TypeCount<T>();
}

template <class T>
bool NLActor::HasTypeComponent() const
{
    if(!m_components)
        return false;
    return m_components->HasType<T>();
}

template <class T>
T * NLActor::GetTypeComponent(int index)
{
    if(!m_components)
        return 0;
    return m_components->GetType<T>(index);
}

template <class T>
QList<T *> NLActor::GetTypeComponents()
{
    if(!m_components)
        return 0;
    return m_components->GetTypes<T>();
}

template <class T>
bool NLActor::RemoveTypeComponent()
{
    if(!m_components)
        return false;
    T *r = m_components->GetType<T>();
    if(!r)
        return false;
    return RemoveComponent(r);
}

template <class T>
int NLActor::RemoveTypeComponents()
{
    if(!m_components)
        return 0;
    QList<T *> list = m_components->GetTypes<T>();
    int c = 0;
    Q_FOREACH(T *obj, list)
    {
        if(RemoveComponent(obj))
            c++;
    }
    return c;
}

template <class T>
T * NLActor::GetScript_T(const NLName &name)
{
    NLScript *obj = GetScript(name);
    if(!obj)
        return 0;
    return dynamic_cast<T *>(obj);
}

template <class T>
T * NLActor::GetScript_T(int index)
{
    NLScript *obj = GetScript(index);
    if(!obj)
        return 0;
    return dynamic_cast<T *>(obj);
}

template <class T>
bool NLActor::ScriptIsType(int index) const
{
    if(!m_scripts)
        return false;
    return m_scripts->IsType<T>(index);
}

template <class T>
bool NLActor::ScriptIsType(const NLName &name) const
{
    if(!m_scripts)
        return false;
    return m_scripts->IsType<T>(name);
}

template <class T>
int NLActor::TypeScriptCount() const
{
    if(!m_scripts)
        return 0;
    return m_scripts->TypeCount<T>();
}

template <class T>
bool NLActor::HasTypeScript() const
{
    if(!m_scripts)
        return false;
    return m_scripts->HasType<T>();
}

template <class T>
T * NLActor::GetTypeScript(int index)
{
    if(!m_scripts)
        return 0;
    return m_scripts->GetType<T>(index);
}

template <class T>
QList<T *> NLActor::GetTypeScripts()
{
    if(!m_scripts)
        return 0;
    return m_scripts->GetTypes<T>();
}

template <class T>
bool NLActor::RemoveTypeScript()
{
    if(!m_scripts)
        return false;
    T *r = m_scripts->GetType<T>();
    if(!r)
        return false;
    return RemoveScript(r);
}

template <class T>
int NLActor::RemoveTypeScripts()
{
    if(!m_scripts)
        return 0;
    QList<T *> list = m_scripts->GetTypes<T>();
    int c = 0;
    Q_FOREACH(T *obj, list)
    {
        if(RemoveScript(obj))
            c++;
    }
    return c;
}

#endif // _KARIN_NLACTOR_H
