#include "nlfuncs.h"

#include <QDebug>
#include <QMetaObject>
#include <QMetaProperty>

#include "nlobject.h"

namespace NL
{

NLPropertyInfoList object_propertics(const NLObject *obj)
{
    const QMetaObject *metaObj = obj->metaObject();
    NLPropertyInfoList ret;
    for(int i = 0 /*metaObj->propertyOffset()*/; i < metaObj->propertyCount(); i++)
    {
        QMetaProperty p = metaObj->property(i);
        QString name(p.name());
        QString type(p.typeName());
        QString widget;
        if(type == "NLVector3" || type == "vector3_s" || type == "vector3_t")
        {
            if(type != "NLVector3")
                type = "NLVector3";
            widget = "vector3";
        }
        else if(type == "int")
        {
            widget = "spinbox";
        }
        else if(type == "double" || type == "float")
        {
            if(type != "float")
                type = "float";
            widget = "spinbox";
        }
        else if(type == "bool")
            widget = "checkbox";
        else
            widget = "lineedit";

        bool readonly = (name == "objectName" || name == "renderable");

        //qDebug() << name << p.read(obj).type() << p.read(obj).userType();
        ret.push_back(NLPropertyInfo(name, p.read(obj), type, widget, readonly, obj->GetInitProperty(name)));
    }
    return ret;
}

bool property_equals(const NLProperty &a, const NLProperty &b)
{
    QVariant::Type at = a.type();
    QVariant::Type bt = b.type();
    if(at == bt && at == QVariant::UserType)
    {
        QString atype(a.typeName());
        QString btype(b.typeName());
        if(atype == btype)
        {
            if(atype == "NLVector3" || atype == "vector3_t" || atype == "vector3_s")
            {
                NLVector3 av = a.value<NLVector3>();
                NLVector3 bv = b.value<NLVector3>();
                return vector3_equals(&av, &bv) ? true : false;
            }
        }
    }
    return a == b; // TODO
}

}
