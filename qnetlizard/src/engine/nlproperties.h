#ifndef _KARIN_NLPROPERTIES_H
#define _KARIN_NLPROPERTIES_H

#include <QVariantHash>
#include <QPair>

typedef QVariant NLProperty;

struct NLPropertyInfo
{
    QString name;
    NLProperty value;
    QString type;
    QString widget;
    bool readonly;
    NLProperty default_value;
    QVariantMap prop;

    NLPropertyInfo(const QString &name, const NLProperty &value, const QString &type, const QString &widget, bool readonly = true, const NLProperty &def_value = NLProperty())
        : name(name),
          value(value),
          type(type),
          widget(widget),
          readonly(readonly),
          default_value(def_value)
    {
    }

    NLPropertyInfo & operator()(const QString &name, const QVariant &val)
    {
        prop.insert(name, val);
        return *this;
    }
};
typedef QList<NLPropertyInfo> NLPropertyInfoList;

class NLPropertyPair : public QPair<QString, NLProperty>
{
public:
    explicit NLPropertyPair();
    explicit NLPropertyPair(const QString &name, const NLProperty &value);
    virtual ~NLPropertyPair();
    QString Name() const { return first; }
    NLProperty Value() const { return second; }
    void SetName(const QString &name) { first = name; }
    void SetValue(const NLProperty &val) { second = val; }
    operator QString() const { return first; }
    operator NLProperty() const { return second; }
    template <class T>
    T Value_T() const;
    template <class T>
    void SetValue_T(const T &val);
};

class NLProperties : public QVariantHash
{
public:
    explicit NLProperties();
    explicit NLProperties(const QString &name, const NLProperty &value);
    virtual ~NLProperties();
    NLProperties & Insert(const QString &name, const NLProperty &value) { insert(name, value); return *this; }
    NLProperties & Insert(const NLPropertyPair &p) { return Insert(p.first, p.second); }
    NLProperties & operator<<(const NLPropertyPair &p) {  return Insert(p); }
    friend NLProperties & operator+(NLProperties &props, const NLPropertyPair &p) { return props.Insert(p); }
    friend NLProperties & operator-(NLProperties &props, const QString &name) { return props.Remove(name); }
    NLProperties & Remove(const QString &name) { remove(name); return *this; }
    NLProperty Get(const QString &name, const NLProperty &def = NLProperty()) const;
    template <class T>
    T Get_T(const QString &name, const T &def = T()) const;
    int Set(const QString &name, const NLProperty &value);
    template <class T>
    int Set_T(const QString &name, const T &value);
    NLProperty GetSet(const QString &name, const NLProperty &def);
    template <class T>
    T GetSet_T(const QString &name, const T &def);
    NLProperties & operator()(const QString &name, const NLProperty &value) { return Insert(name, value); }
    NLProperty operator()(const QString &name) { return Get(name, NLProperty()); }
};

template <class T>
inline T NLPropertyPair::Value_T() const
{
    return second.value<T>();
}

template <class T>
inline void NLPropertyPair::SetValue_T(const T &val)
{
    second.setValue<T>(val);
}

template <class T>
T NLProperties::Get_T(const QString &name, const T &def) const
{
    if(!contains(name))
        return def;
    return value(name).value<T>();
}

template <class T>
int NLProperties::Set_T(const QString &name, const T &value)
{
    if(!contains(name))
    {
        insert(name, NLProperty::fromValue(value));
        return 1;
    }
    const T &t = operator[](name).value<T>();
    if(t != value)
    {
        operator[](name) = NLProperty::fromValue(value);
        return 2;
    }
    return 0;
}

template <class T>
T NLProperties::GetSet_T(const QString &name, const T &def)
{
    if(!contains(name))
        Set_T<T>(name, def);
    return Get_T<T>(name);
}

#endif // _KARIN_NLPROPERTIES_H
