#include "nlfuncs.h"

#include <QDebug>
#include <QMetaObject>
#include <QMetaProperty>

#include "nlobject.h"

namespace NL
{
static NLPropertyInfo make_property_info(const QString &name, int t, const QString &typeName, const QVariant &value, const QVariantHash &props = QVariantHash())
{
    QString type(typeName);
    QString widget;
    if(type == "NLVector3" || type == "vector3_s" || type == "vector3_t")
    {
        if(type != "NLVector3")
            type = "NLVector3";
        widget = "vector3";
    }
    else if(type == "int")
    {
        if(props.contains("enum"))
            widget = "combobox";
        else
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
    {
        if(t == QMetaType::QObjectStar || t == QMetaType::QWidgetStar || t == QMetaType::VoidStar)
        {
            widget = "label";
        }
        else
        {
            if(props.value("multiline").toBool())
                widget = "textedit";
            else if(props.value("file").toBool())
                widget = "filedialog";
            else
                widget = "lineedit";
        }
    }

    bool readonly = (name == "objectName" || name == "renderable")
            || (t == QMetaType::QObjectStar || t == QMetaType::QWidgetStar || t == QMetaType::VoidStar)
            ;

    //qDebug() << name << t << value << QMetaType::QObjectStar << t << QMetaType::VoidStar;
    return(NLPropertyInfo(name, value, type, widget, readonly, props));
}

NLPropertyInfoList object_propertics(const NLObject *obj)
{
    const QMetaObject *metaObj = obj->metaObject();
    NLPropertyInfoList ret;
    const NLProperties config = obj->PropertyConfig();

    for(int i = 0 /*metaObj->propertyOffset()*/; i < metaObj->propertyCount(); i++)
    {
        QMetaProperty p = metaObj->property(i);
        QString name(p.name());

        QVariantHash prop = config.value(name).toHash();
        NLPropertyInfo info = make_property_info(name, p.type(), p.typeName(), p.read(obj), prop);
        info.default_value = obj->GetInitProperty(name);

        ret.push_back(info);
    }

    // dynamic property
    const QList<QByteArray> list = obj->dynamicPropertyNames();
    Q_FOREACH(const QByteArray &ba, list)
    {
        QVariant p = obj->property(ba.constData());
        QString name(ba);

        QVariantHash prop = config.value(name).toHash();
        NLPropertyInfo info = make_property_info(name, p.type(), p.typeName(), p, prop);
        info.default_value = obj->GetInitProperty(name);

        ret.push_back(info);
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
