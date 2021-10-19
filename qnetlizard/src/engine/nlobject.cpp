#include "nlobject.h"

#include <QDebug>

#include "qdef.h"
#include "nlobjectpool.h"
#include "nlobjectcontainer.h"

NLObject::NLObject(QObject *parent) :
    QObject(parent),
    m_type(NLObject::Type_General),
    m_inited(false),
    m_container(0),
    m_scene(0),
    m_enabled(true)
{
    Construct();
}

NLObject::NLObject(const NLPropperties &prop, QObject *parent) :
    QObject(parent),
    m_type(NLObject::Type_General),
    m_inited(false),
    m_container(0),
    m_scene(0),
    m_enabled(true)
{
    Construct(prop);
}

NLObject::NLObject(NLScene *scene, QObject *parent) :
    QObject(parent),
    m_type(NLObject::Type_General),
    m_inited(false),
    m_container(0),
    m_scene(scene),
    m_enabled(true)
{
    Construct();
}

NLObject::NLObject(NLScene *scene, const NLPropperties &prop, QObject *parent) :
    QObject(parent),
    m_type(NLObject::Type_General),
    m_inited(false),
    m_container(0),
    m_scene(scene),
    m_enabled(true)
{
    Construct(prop);
}

NLObject::~NLObject()
{
    NLObjectPool::Instance()->Detach(this);
    Destroy();
    DEBUG_DESTROY_Q;
}

void NLObject::Construct(const NLPropperties &prop)
{
    setObjectName("NLObject");
    QObject *p = parent();
    if(p)
    {
        NLObject *obj = dynamic_cast<NLObject *>(p);
        if(obj)
            SetScene(obj->Scene());
    }
    CopyProperty(prop);
    SetName(NLObjectPool::Instance()->Attach(this));
}

void NLObject::CopyProperty(const NLPropperties &prop)
{
    Q_FOREACH(const QString &name, prop.keys())
    {
        SetProperty(name, prop.value(name));
    }
}

void NLObject::InitProperty()
{
    SetEnabled(GetProperty<bool>("enabled", true));
}

NLObject::NLObject_Type NLObject::Type() const
{
    return m_type;
}

QString NLObject::Name() const
{
    return m_name;
}

void NLObject::SetName(const QString &name)
{
    if(m_name != name)
        m_name = name;
}

void NLObject::SetType(NLObject_Type type)
{
    if(m_type != type)
        m_type = type;
}

void NLObject::Init()
{
    qDebug() << objectName() + ": " + m_name + " -> inited";
    InitProperty();
    m_inited = true;
    emit initilized();
}

void NLObject::Update(float delta)
{
    if(!IsActived())
        return;
    //qDebug() << objectName() + ": " + m_name + " -> update(" + QString::number(delta) + ")";
}

void NLObject::Reset()
{
    qDebug() << objectName() + ": " + m_name + " -> reset";
    emit reseted();
}

void NLObject::Destroy()
{
    emit destroying();
    qDebug() << objectName() + ": " + m_name + " -> destroyed";
    m_inited = false;
}

bool NLObject::IsInited() const
{
    return m_inited;
}

NLObject * NLObject::ParentObject()
{
    QObject *p = parent();
    if(p)
        return dynamic_cast<NLObject *>(p);
    return 0;
}

NLObjectContainer * NLObject::Container()
{
    return m_container;
}

void NLObject::SetContainer(NLObjectContainer *container)
{
    m_container = container;
}

NLPropperty NLObject::GetProperty(const QString &name, const NLPropperty &def) const
{
    if(!m_property.contains(name))
        return def;
    return m_property.value(name);
}

void NLObject::SetProperty(const QString &name, const NLPropperty &value)
{
    if(!m_property.contains(name))
    {
        m_property.insert(name, value);
        return;
    }
    if(m_property[name] != value)
    {
        m_property[name] = value;
        emit propertyChanged(name, value);
    }
}

NLPropperty & NLObject::operator[](const QString &name)
{
    return m_property[name];
}

NLPropperty NLObject::operator[](const QString &name) const
{
    return GetProperty(name);
}

NLScene * NLObject::Scene()
{
    return m_scene;
}

void NLObject::SetScene(NLScene *scene)
{
    m_scene = scene;
}

bool NLObject::IsEnabled() const
{
    return m_enabled;
}

void NLObject::SetEnabled(bool enabled)
{
    if(m_enabled != enabled)
    {
        m_enabled = enabled;
        emit enabledChanged(m_enabled);
    }
}

bool NLObject::IsActived() const
{
    return m_inited && m_enabled;
}
