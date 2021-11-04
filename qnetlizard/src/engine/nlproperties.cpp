#include "nlproperties.h"

NLPropertyPair::NLPropertyPair()
    : QPair<QString, NLProperty>()
{

}

NLPropertyPair::NLPropertyPair(const QString &name, const NLProperty &value)
{

}

NLPropertyPair::~NLPropertyPair()
{

}

QString NLPropertyPair::Name() const
{
    return first;
}

NLProperty NLPropertyPair::Value() const
{
    return second;
}

NLPropertyPair::operator QString() const
{
    return first;
}

NLPropertyPair::operator NLProperty() const
{
    return second;
}

void NLPropertyPair::SetName(const QString &name)
{
    first = name;
}

void NLPropertyPair::SetValue(const NLProperty &val)
{
    second = val;
}




NLProperties::NLProperties()
    : QVariantHash()
{
}

NLProperties::NLProperties(const QString &name, const NLProperty &value)
    : QVariantHash()
{
    insert(name, value);
}

NLProperties::~NLProperties()
{
}

NLProperties & NLProperties::Insert(const QString &name, const NLProperty &value)
{
    insert(name, value);
    return *this;
}

NLProperties & NLProperties::Insert(const NLPropertyPair &p)
{
    return Insert(p.first, p.second);
}

NLProperties & NLProperties::operator<<(const NLPropertyPair &p)
{
    return Insert(p);
}

NLProperties & operator+(NLProperties &props, const NLPropertyPair &p)
{
    return props.Insert(p);
}

NLProperties & operator-(NLProperties &props, const QString &name)
{
    return props.Remove(name);
}

NLProperties & NLProperties::Remove(const QString &name)
{
    remove(name);
    return *this;
}

NLProperty NLProperties::Get(const QString &name, const NLProperty &def) const
{
    if(!contains(name))
        return def;
    return value(name);
}

int NLProperties::Set(const QString &name, const NLProperty &value)
{
    if(!contains(name))
    {
        insert(name, value);
        return 1;
    }
    if(operator[](name) != value)
    {
        operator[](name) = value;
        return 2;
    }
    return 0;
}


NLProperty NLProperties::GetSet(const QString &name, const NLProperty &def)
{
    if(!contains(name))
        Set(name, def);
    return Get(name);
}

NLProperties & NLProperties::operator()(const QString &name, const NLProperty &value)
{
    return Insert(name, value);
}

NLProperty NLProperties::operator()(const QString &name)
{
    return Get(name, NLProperty());
}
