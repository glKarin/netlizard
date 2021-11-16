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

NLObject::NLObject(const NLProperties &prop, QObject *parent) :
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

NLObject::NLObject(NLScene *scene, const NLProperties &prop, QObject *parent) :
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

void NLObject::Construct(const NLProperties &prop)
{
    CopyProperty(prop);
    setObjectName(GetProperty_T<QString>("name", "NLObject"));
    QObject *p = parent();
    if(p)
    {
        NLObject *obj = dynamic_cast<NLObject *>(p);
        if(obj)
            SetScene(obj->Scene());
    }
    SetName(NLObjectPool::Instance()->Attach(this));
}

void NLObject::CopyProperty(const NLProperties &prop)
{
    Q_FOREACH(const QString &name, prop.keys())
    {
        m_property.Insert(name, prop.value(name));
    }
}

void NLObject::InitProperty()
{
    SetEnabled(GetInitProperty_T<bool>("enabled", true));
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

NLProperty NLObject::GetProperty(const QString &name, const NLProperty &def) const
{
    QByteArray ba = name.toLocal8Bit();
    NLProperty p = property(ba.constData());
    if(!p.isValid())
        return def;
    return p;
}

void NLObject::SetProperty(const QString &name, const NLProperty &value)
{
    QByteArray ba = name.toLocal8Bit();
    setProperty(ba.constData(), value);
    qDebug() << name << property(ba.constData());
    int r = 2;
    if(r == 2)
        emit propertyChanged(name, value);
}

void NLObject::RemoveProperty(const QString &name)
{
    QByteArray ba = name.toLocal8Bit();
    setProperty(ba.constData(), NLProperty());
    int r = 2;
    if(r == 2)
        emit propertyChanged(name);
}

NLProperty & NLObject::operator[](const QString &name)
{
    //QByteArray ba = name.toLocal8Bit();
    //return property(ba.constData());
    return m_property[name];
}

NLProperty NLObject::operator[](const QString &name) const
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
        emit propertyChanged("enabled", m_enabled);
    }
}

bool NLObject::IsActived() const
{
    return m_inited && m_enabled;
}

NLProperty NLObject::GetInitProperty(const QString &name, const NLProperty &def) const
{
    return m_property.Get(name, def);
}
