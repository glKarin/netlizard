#include "nlobject.h"

#include <QDebug>

#include "qdef.h"
#include "nlobjectpool.h"
#include "nlobjectcontainer.h"

NLObject::NLObject(const NLPropperties &prop, QObject *parent) :
    QObject(parent),
    m_type(NLObject::Type_General),
    m_inited(false),
    m_container(0),
    m_scene(0)
{
    setObjectName("NLObject");
    InitProperty(prop);
    SetName(NLObjectPool::Instance()->Attach(this));
}

NLObject::~NLObject()
{
    NLObjectPool::Instance()->Detach(this);
    Destroy();
    DEBUG_DESTROY_Q;
}

void NLObject::InitProperty(const NLPropperties &prop)
{
    Q_FOREACH(const QString &name, prop.keys())
    {
        SetProperty(name, prop.value(name));
    }
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
    m_inited = true;
}

void NLObject::Update(float delta)
{
    if(!IsInited())
        return;
    //qDebug() << objectName() + ": " + m_name + " -> update(" + QString::number(delta) + ")";
}

void NLObject::Reset()
{
    qDebug() << objectName() + ": " + m_name + " -> reset";
}

void NLObject::Destroy()
{
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
    m_property[name] = value;
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
