#include "nlproperties.h"

NLPropertyPair::NLPropertyPair()
    : QPair<QString, QVariant>()
{

}

NLPropertyPair::NLPropertyPair(const QString &name, const QVariant &value)
    : QPair<QString, QVariant>(name, value)
{

}

NLPropertyPair::~NLPropertyPair()
{

}



NLProperties::NLProperties()
    : NLVariantSequenceHash()
{
}

NLProperties::NLProperties(const QString &name, const QVariant &value)
    : NLVariantSequenceHash()
{
    insert(name, value);
}

NLProperties::~NLProperties()
{
}

QVariant NLProperties::Get(const QString &name, const QVariant &def) const
{
    if(!contains(name))
        return def;
    return value(name);
}

int NLProperties::Set(const QString &name, const QVariant &value)
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

bool NLProperties::SetIfNoExists(const QString &name, const QVariant &value)
{
    if(!contains(name))
    {
        insert(name, value);
        return true;
    }
    return false;
}

NLProperties & NLProperties::TrySet(const QString &name, const QVariant &value)
{
    SetIfNoExists(name, value);
    return *this;
}

bool NLProperties::Replace(const QString &name, const QVariant &value)
{
    if(contains(name))
    {
        if(operator[](name) != value)
        {
            insert(name, value);
            return true;
        }
    }
    return false;
}

QVariant NLProperties::GetSet(const QString &name, const QVariant &def)
{
    if(!contains(name))
        Set(name, def);
    return Get(name);
}
