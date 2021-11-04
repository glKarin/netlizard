#ifndef _KARIN_NLPROPERTIES_H
#define _KARIN_NLPROPERTIES_H

#include <QVariantHash>
#include <QPair>

typedef QVariant NLProperty;
class NLPropertyPair : public QPair<QString, NLProperty>
{
public:
    explicit NLPropertyPair();
    explicit NLPropertyPair(const QString &name, const NLProperty &value);
    virtual ~NLPropertyPair();
    QString Name() const;
    NLProperty Value() const;
    void SetName(const QString &name);
    void SetValue(const NLProperty &val);
    operator QString() const;
    operator NLProperty() const;
    template <class T>
    T Value_T() const;
    template <class T>
    void SetValue_T(const T &val);
};

class NLProperties : public QVariantHash
{
public:
    explicit NLProperties();
    virtual ~NLProperties();
    NLProperties & Insert(const QString &name, const NLProperty &value);
    NLProperties & Insert(const NLPropertyPair &p);
    NLProperties & operator<<(const NLPropertyPair &p);
    friend NLProperties & operator+(NLProperties &props, const NLPropertyPair &p);
    friend NLProperties & operator-(NLProperties &props, const QString &name);
    NLProperties & Remove(const QString &name);
    NLProperty Get(const QString &name, const NLProperty &def = NLProperty()) const;
    template <class T>
    T Get_T(const QString &name, const T &def = T()) const;
    int Set(const QString &name, const NLProperty &value);
    template <class T>
    int Set_T(const QString &name, const T &value);
    NLProperty GetSet(const QString &name, const NLProperty &def);
    template <class T>
    T GetSet_T(const QString &name, const T &def);
    NLProperties & operator()(const QString &name, const NLProperty &value);
    NLProperty operator()(const QString &name);
};

template <class T>
T NLPropertyPair::Value_T() const
{
    return second.value<T>();
}

template <class T>
void NLPropertyPair::SetValue_T(const T &val)
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
