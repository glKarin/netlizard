#include "nlfuncs.h"

#include <QDebug>
#include <QMetaObject>
#include <QMetaProperty>

#include "nlobject.h"

namespace NL
{

NLProperties ObjectPropertics(const NLObject *obj)
{
    const QMetaObject *metaObj = obj->metaObject();
    NLProperties ret;
    for(int i = 0 /*metaObj->propertyOffset()*/; i < metaObj->propertyCount(); i++)
    {
        QMetaProperty p = metaObj->property(i);
        ret.Insert(p.name(), p.read(obj));
    }
    return ret;
}

}
