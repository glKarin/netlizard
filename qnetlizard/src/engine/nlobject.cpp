#include "nlobject.h"

#include <QDebug>
#include <QMetaProperty>
#include <QMetaObject>

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
    Destroy(); // !! vitrual
    DEBUG_DESTROY_Q;
}

void NLObject::Construct(const NLProperties &prop)
{
    CopyProperty(prop);
    CLASS_NAME(NLObject);
    setObjectName(GetInitProperty_T<QString>("name", "NLObject"));
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
//    QString name(GetInitProperty_T<QString>("objectName"));
//    if(!name.isEmpty())
//        setObjectName(name);
}

void NLObject::SetName(const QString &name)
{
    if(m_name != name)
        m_name = name;
}

void NLObject::SetClassName(const QString &name)
{
    if(m_className != name)
        m_className = name;
}

void NLObject::SetType(NLObject_Type type)
{
    if(m_type != type)
        m_type = type;
}

void NLObject::Init()
{
    if(IsInited())
        return;
#ifdef _DEV_TEST
    qDebug() << objectName() + "(" + m_name + ") -> INITED";
#endif
    InitProperty();
    m_inited = true;
    emit initilized();
}

#if 0
void NLObject::Update(float delta)
{
    if(!IsActived())
        return;
#if 0
    qDebug() << objectName() + "(" + m_name + ") -> UPDATE(" + QString::number(delta) + ")";
#else
    Q_UNUSED(delta);
#endif
}
#endif

void NLObject::Reset()
{
#ifdef _DEV_TEST
    qDebug() << objectName() + "(" + m_name + ") -> RESET";
#endif
    emit reseted();
}

void NLObject::Destroy()
{
    if(!IsInited())
        return;
    emit destroying();
#ifdef _DEV_TEST
    qDebug() << objectName() + "(" + m_name + ") -> DESTROYED";
#endif
    if(m_container)
        m_container->Take(this);
    SetContainer(0);
    SetScene(0);
    setParent(0);
    m_inited = false;
}

NLObject * NLObject::ParentObject()
{
    QObject *p = parent();
    if(p)
        return dynamic_cast<NLObject *>(p);
    return 0;
}

void NLObject::SetContainer(NLObjectContainer *container)
{
    if(m_container != container)
    {
        if(!container || (container && !m_container))
           m_container = container;
    }
}

void NLObject::SetScene(NLScene *scene)
{
    if(m_scene != scene)
        m_scene = scene;
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

void NLObject::SetPropertyConfig(const NLProperties &props)
{
    m_propertyConfig = props;
}

NLPROPERTY_DECL_TRAIT(NLObject, NL::property_equals, propertyChanged)
