#ifndef _KARIN_NLPROPERTIES_H
#define _KARIN_NLPROPERTIES_H

#include <QPair>
#include "nldef.h"
#include "template/nlsequencemap.h"

#define NLPROPERTIES_NAME(p, x) NLProperties(p).TrySet("name", #x)
#define NLPROPERTIY_NAME(x) NLProperties("name", #x)

class NLLIB_EXPORT NLPropertyPair : public QPair<QString, QVariant>
{
public:
    explicit NLPropertyPair();
    explicit NLPropertyPair(const QString &name, const QVariant &value);
    virtual ~NLPropertyPair();
    QString Name() const { return first; }
    QVariant Value() const { return second; }
    void SetName(const QString &name) { first = name; }
    void SetValue(const QVariant &val) { second = val; }
    operator QString() const { return first; }
    operator QVariant() const { return second; }
    template <class T>
    T Value_T() const;
    template <class T>
    void SetValue_T(const T &val);
};
typedef QList<NLPropertyPair> NLPropertyPairList;

class NLLIB_EXPORT NLProperties : public NLVariantSequenceHash
{
public:
    explicit NLProperties();
    explicit NLProperties(const NLVariantSequenceHash &hash);
    explicit NLProperties(const QString &name, const QVariant &value);
    virtual ~NLProperties();
    NLProperties & Insert(const QString &name, const QVariant &value) { insert(name, value); return *this; }
    NLProperties & Insert(const NLPropertyPair &p) { return Insert(p.first, p.second); }
    NLProperties & operator<<(const NLPropertyPair &p) {  return Insert(p); }
    NLProperties & operator+(const NLPropertyPair &p) { return Insert(p); }
    NLProperties & operator-(const QString &name) { return Remove(name); }
    NLProperties & Remove(const QString &name) { remove(name); return *this; }
    QVariant Get(const QString &name, const QVariant &def = QVariant()) const;
    template <class T>
    T Get_T(const QString &name, const T &def = T()) const;
    int Set(const QString &name, const QVariant &value);
    template <class T>
    int Set_T(const QString &name, const T &value);
    QVariant GetSet(const QString &name, const QVariant &def);
    template <class T>
    T GetSet_T(const QString &name, const T &def);
    NLProperties & operator()(const QString &name, const QVariant &value) { return Insert(name, value); }
    QVariant operator()(const QString &name) { return Get(name, QVariant()); }
    bool SetIfNoExists(const QString &name, const QVariant &value);
    template <class T>
    bool SetIfNoExists_T(const QString &name, const T &value);
    NLProperties & TrySet(const QString &name, const QVariant &value);
    template <class T>
    NLProperties & TrySet_T(const QString &name, const T &value);
    bool Replace(const QString &name, const QVariant &value);
    template <class T>
    bool Replace_T(const QString &name, const T &value);
};

struct NLLIB_EXPORT NLPropertyInfo
{
    QString name;
    QVariant value;
    QString type;
    QString widget;
    bool readonly;
    QVariant default_value;
    QVariantHash prop;
    QString label;
    QString description;

    NLPropertyInfo(const QString &name, const QVariant &value, const QString &type, const QString &widget, bool readonly = true, const QVariant &def_value = QVariant(), const QVariantHash &config = QVariantHash(), const QString &label = QString(), const QString &description = QString())
        : name(name),
          value(value),
          type(type),
          widget(widget),
          readonly(readonly),
          default_value(def_value),
          prop(config),
          label(label),
          description(description)
    {
        if(this->label.isEmpty())
            this->label = name;
    }

    NLPropertyInfo & operator()(const QString &name, const QVariant &val)
    {
        prop.insert(name, val);
        return *this;
    }
};
typedef QList<NLPropertyInfo> NLPropertyInfoList;

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
        insert(name, QVariant::fromValue(value));
        return 1;
    }
    const T &t = operator[](name).value<T>();
    if(t != value)
    {
        operator[](name) = QVariant::fromValue(value);
        return 2;
    }
    return 0;
}

template <class T>
bool NLProperties::SetIfNoExists_T(const QString &name, const T &value)
{
    if(!contains(name))
    {
        insert(name, QVariant::fromValue(value));
        return true;
    }
    return false;
}

template <class T>
NLProperties & NLProperties::TrySet_T(const QString &name, const T &value)
{
    SetIfNoExists_T<T>(name, value);
    return *this;
}

template <class T>
bool NLProperties::Replace_T(const QString &name, const T &value)
{
    if(contains(name))
    {
        if(operator[](name).value<T>() != value)
        {
            insert(name, QVariant::fromValue(value));
            return true;
        }
    }
    return false;
}

template <class T>
T NLProperties::GetSet_T(const QString &name, const T &def)
{
    if(!contains(name))
        Set_T<T>(name, def);
    return Get_T<T>(name);
}

Q_DECLARE_METATYPE(NLPropertyPair)
Q_DECLARE_METATYPE(NLPropertyPairList)
Q_DECLARE_METATYPE(NLProperties)

#endif // _KARIN_NLPROPERTIES_H
