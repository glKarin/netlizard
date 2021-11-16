#include "nlfuncs.h"

#include <QDebug>
#include <QMetaObject>
#include <QMetaProperty>

#include "nlobject.h"

namespace NL
{

NLPropertyInfoList ObjectPropertics(const NLObject *obj)
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
        else if(type == "int" || type == "float")
            widget = "spinbox";
        else if(type == "bool")
            widget = "checkbox";
        else
            widget = "lineedit";

        bool readonly = (name == "objectName");

        ret.push_back(NLPropertyInfo(p.name(), p.read(obj), type, widget, readonly));
    }
    return ret;
}

}
