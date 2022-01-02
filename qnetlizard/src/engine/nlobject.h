#ifndef _KARIN_NLOBJECT_H
#define _KARIN_NLOBJECT_H

#include <QObject>

#include "nldef.h"
#include "nlproperties.h"
#include "nlfuncs.h"

#define CLASS_NAME(x) SetClassName(#x)

class NLObjectContainer;
class NLScene;

class NLObject : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool enabled READ IsEnabled WRITE SetEnabled NOTIFY enabledChanged FINAL)

public:
    enum NLObject_Type
    {
        Type_General = 0,
        Type_Actor,
        Type_Component,
        Type_Script,
        Type_Force
    };

public:
    explicit NLObject(QObject *parent = 0);
    explicit NLObject(const NLProperties &prop, QObject *parent = 0);
    explicit NLObject(NLScene *scene, QObject *parent = 0);
    explicit NLObject(NLScene *scene, const NLProperties &prop, QObject *parent = 0);
    virtual ~NLObject();
    virtual bool IsActived() const { return m_inited && m_enabled; }
    virtual void Reset();
    NLObject_Type Type() const { return m_type; }
    QString Name() const { return m_name; }
    QString ClassName() const { return m_className; }
    NLObject * ParentObject();
    bool IsInited() const { return m_inited; }
    NLObjectContainer * Container() { return m_container; }
    NLProperty GetProperty(const QString &name, const NLProperty &def = QVariant()) const;
    template<class T> T GetProperty_T(const QString &name, const T &def = T());
    void SetProperty(const QString &name, const NLProperty &value);
    void CoverProperty(const QString &name, const NLProperty &value);
    template<class T> void SetProperty_T(const QString &name, const T &value);
    template<class T> void CoverProperty_T(const QString &name, const T &value);
    NLProperty & operator[](const QString &name) { return m_property[name]; } // init property
    NLProperty operator[](const QString &name) const { return GetProperty(name); }
    void SetScene(NLScene *scene);
    NLScene * Scene() { return m_scene; }
    virtual bool IsEnabled() const { return m_enabled; }
    virtual void SetEnabled(bool enabled);
    void RemoveProperty(const QString &name);
    bool HasProperty(const QString &name);
    NLProperty GetInitProperty(const QString &name, const NLProperty &def = QVariant()) const { return m_property.Get(name, def); }
    template<class T> T GetInitProperty_T(const QString &name, const T &def = T());
    NLProperties PropertyConfig() const { return m_propertyConfig; }

protected:
    void SetName(const NLName &name);
    void SetClassName(const NLName &name);
    void SetType(NLObject_Type type);
    void SetContainer(NLObjectContainer *container);
    virtual void Init();
    virtual void Destroy();
    virtual void Update(float delta);
    virtual void InitProperty();
    void SetPropertyConfig(const NLProperties &props);

signals:
    void enabledChanged(bool enabled);
    void initilized();
    void destroying();
    void reseted();
    void propertyChanged(const QString &name, const NLProperty &value = NLProperty());
    
public slots:

private:
    void CopyProperty(const NLProperties &prop);
    void Construct(const NLProperties &prop = NLProperties());

private:
    NLObject_Type m_type;
    bool m_inited;
    QString m_name;
    QString m_className;
    NLObjectContainer *m_container;
    NLProperties m_property;
    NLScene *m_scene;
    bool m_enabled;
    NLProperties m_propertyConfig;

    friend class NLObjectContainer;
    
    Q_DISABLE_COPY(NLObject)
};

typedef QList<NLObject *> NLObjectList;

template <class T> T NLObject::GetProperty_T(const QString &name, const T &def)
{
    const QByteArray ba = name.toLocal8Bit();
    NLProperty p = property(ba.constData());
    if(!p.isValid())
        return def;
    return p.value<T>();
}

template<class T> void NLObject::SetProperty_T(const QString &name, const T &value)
{
    bool has = HasProperty(name);
    if(has && GetProperty(name) == value)
    {
        return;
    }
    const QByteArray ba = name.toLocal8Bit();
    setProperty(ba.constData(), value);
    emit propertyChanged(name, NLProperty::fromValue(value));
}

template<class T> void NLObject::CoverProperty_T(const QString &name, const T &value)
{
    const QByteArray ba = name.toLocal8Bit();
    setProperty(ba.constData(), value);
    emit propertyChanged(name, NLProperty::fromValue(value));
}

template <class T> T NLObject::GetInitProperty_T(const QString &name, const T &def)
{
    return m_property.Get_T<T>(name, def);
}

//void NLObject::SetProperty_T(const QString &name, const NLVector3 &value)
//{
//    bool has = HasProperty(name);
//    NLVector3 old = GetProperty(name).value<NLVector3>();
//    if(has && vector3_equals(&old, &value))
//    {
//        return;
//    }
//    QByteArray ba = name.toLocal8Bit();
//    NLProperty n = NLProperty::fromValue<NLVector3>(value);
//    setProperty(ba.constData(), n);
//    emit propertyChanged(name, n);
//}

#endif // _KARIN_NLOBJECT_H
