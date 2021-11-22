#include "nlproperties.h"

NLPropertyPair::NLPropertyPair()
    : QPair<QString, NLProperty>()
{

}

NLPropertyPair::NLPropertyPair(const QString &name, const NLProperty &value)
    : QPair<QString, NLProperty>(name, value)
{

}

NLPropertyPair::~NLPropertyPair()
{

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
