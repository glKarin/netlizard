#ifndef _KARIN_NLOBJECT_H
#define _KARIN_NLOBJECT_H

#include <QObject>
#include <QList>
#include <QVariant>

#define NLINTERFACE
#define NLGETTER(x)
#define NLSETTER(x)
#define NLPROPERTY(x)

class NLObjectContainer;
class NLScene;

typedef QString NLName;
typedef QVariantHash NLPropperties;
typedef QVariant NLPropperty;

class NLObject : public QObject
{
    Q_OBJECT
public:
    enum NLObject_Type
    {
        Type_General = 0,
        Type_Actor,
        Type_Component
    };

public:
    explicit NLObject(QObject *parent = 0);
    explicit NLObject(const NLPropperties &prop, QObject *parent = 0);
    explicit NLObject(NLScene *scene, QObject *parent = 0);
    explicit NLObject(NLScene *scene, const NLPropperties &prop, QObject *parent = 0);
    virtual ~NLObject();
    NLObject_Type Type() const;
    QString Name() const;
    NLObject * ParentObject();
    bool IsInited() const;
    NLObjectContainer * Container();
    NLPropperty GetProperty(const QString &name, const NLPropperty &def = QVariant()) const;
    template<class T> T GetProperty(const QString &name, const T &def = T());
    void SetProperty(const QString &name, const NLPropperty &value);
    NLPropperty & operator[](const QString &name);
    NLPropperty operator[](const QString &name) const;
    void SetScene(NLScene *scene);
    NLScene * Scene();

protected:
    void SetName(const NLName &name);
    void SetType(NLObject_Type type);
    void SetContainer(NLObjectContainer *container);
    virtual void Init();
    virtual void Destroy();
    virtual void Update(float delta);
    virtual void Reset();
    void InitProperty(const NLPropperties &prop);

signals:
    
public slots:

private:
    void Construct(const NLPropperties &prop = NLPropperties());

private:
    NLObject_Type m_type;
    bool m_inited;
    QString m_name;
    NLObjectContainer *m_container;
    NLPropperties m_property;
    NLScene *m_scene;

    friend class NLObjectContainer;
    
    Q_DISABLE_COPY(NLObject)
};

typedef QList<NLObject *> NLObjectList;

template <class T> T NLObject::GetProperty(const QString &name, const T &def)
{
    if(!m_property.contains(name))
        return def;
    return m_property.value(name).value<T>();
}

#endif // _KARIN_NLOBJECT_H
