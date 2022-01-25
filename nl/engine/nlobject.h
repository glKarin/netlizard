#ifndef _KARIN_NLOBJECT_H
#define _KARIN_NLOBJECT_H

#include <QObject>

#include "nldef.h"
#include "nlproperties.h"
#include "trait/nlpropertytrait.h"

#define CLASS_NAME(x) SetClassName(#x)

class NLObjectContainer;
class NLScene;

class NLLIB_EXPORT NLObject : public QObject
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
        Type_Force,
        Type_Renderer
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
    void SetScene(NLScene *scene);
    NLScene * Scene() { return m_scene; }
    virtual bool IsEnabled() const { return m_enabled; }
    virtual void SetEnabled(bool enabled);
    QVariant GetInitProperty(const QString &name, const QVariant &def = QVariant()) const { return m_property.Get(name, def); }
    template<class T> T GetInitProperty_T(const QString &name, const T &def = T());
    NLProperties PropertyConfig() const { return m_propertyConfig; }
    NLPROPERTY_DEF_TRAIT

protected:
    void SetName(const QString &name);
    void SetClassName(const QString &name);
    void SetType(NLObject_Type type);
    void SetContainer(NLObjectContainer *container);
    virtual void Init();
    virtual void Destroy();
    virtual void Update(float delta) = 0;
    virtual void InitProperty();
    void SetPropertyConfig(const NLProperties &props);

signals:
    void enabledChanged(bool enabled);
    void initilized();
    void destroying();
    void reseted();
    void propertyChanged(const QString &name, const QVariant &value = QVariant(), int action = 0);
    
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

Q_DECLARE_METATYPE(NLObject*)
typedef QList<NLObject *> NLObjectList;

template <class T> T NLObject::GetInitProperty_T(const QString &name, const T &def)
{
    return m_property.Get_T<T>(name, def);
}

NLPROPERTY_TEMPLATE_DECL_TRAIT(NLObject, propertyChanged)

#endif // _KARIN_NLOBJECT_H
