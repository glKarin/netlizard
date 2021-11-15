#ifndef _KARIN_NLOBJECT_H
#define _KARIN_NLOBJECT_H

#include <QObject>

#include "nldef.h"
#include "nlproperties.h"

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
        Type_Force
    };

public:
    explicit NLObject(QObject *parent = 0);
    explicit NLObject(const NLProperties &prop, QObject *parent = 0);
    explicit NLObject(NLScene *scene, QObject *parent = 0);
    explicit NLObject(NLScene *scene, const NLProperties &prop, QObject *parent = 0);
    virtual ~NLObject();
    virtual bool IsActived() const;
    virtual void Reset();
    NLObject_Type Type() const;
    QString Name() const;
    NLObject * ParentObject();
    bool IsInited() const;
    NLObjectContainer * Container();
    NLProperty GetProperty(const QString &name, const NLProperty &def = QVariant()) const;
    template<class T> T GetProperty_T(const QString &name, const T &def = T());
    void SetProperty(const QString &name, const NLProperty &value);
    template<class T> void SetProperty_T(const QString &name, const T &value);
    NLProperty & operator[](const QString &name);
    NLProperty operator[](const QString &name) const;
    void SetScene(NLScene *scene);
    NLScene * Scene();
    bool IsEnabled() const;
    void SetEnabled(bool enabled);

protected:
    void SetName(const NLName &name);
    void SetType(NLObject_Type type);
    void SetContainer(NLObjectContainer *container);
    virtual void Init();
    virtual void Destroy();
    virtual void Update(float delta);
    virtual void InitProperty();
    NLProperty GetInitProperty(const QString &name, const NLProperty &def = QVariant()) const;
    template<class T> T GetInitProperty_T(const QString &name, const T &def = T());

signals:
    void enabledChanged(bool enabled);
    void initilized();
    void destroying();
    void reseted();
    void propertyChanged(const QString &name, const NLProperty &value);
    
public slots:

private:
    void CopyProperty(const NLProperties &prop);
    void Construct(const NLProperties &prop = NLProperties());

private:
    NLObject_Type m_type;
    bool m_inited;
    QString m_name;
    NLObjectContainer *m_container;
    NLProperties m_property;
    NLScene *m_scene;
    bool m_enabled;

    friend class NLObjectContainer;
    
    Q_DISABLE_COPY(NLObject)
};

typedef QList<NLObject *> NLObjectList;

template <class T> T NLObject::GetProperty_T(const QString &name, const T &def)
{
    QByteArray ba = name.toLocal8Bit();
    NLProperty p = property(ba.constData());
    if(!p.isValid())
        return def;
    return p.value<T>();
}

template<class T> void NLObject::SetProperty_T(const QString &name, const T &value)
{
    QByteArray ba = name.toLocal8Bit();
    setProperty(ba.constData(), value);
    int r = 2;
    if(r == 2)
        emit propertyChanged(name, NLProperty::fromValue(value));
}

template <class T> T NLObject::GetInitProperty_T(const QString &name, const T &def)
{
    return m_property.Get_T<T>(name, def);
}

#endif // _KARIN_NLOBJECT_H
