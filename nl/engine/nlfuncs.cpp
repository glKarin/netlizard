#include "nlfuncs.h"

#include <QDebug>
#include <QMetaObject>
#include <QMetaProperty>

#include "nlobject.h"
#include "nlscene.h"

namespace NL
{
static NLPropertyInfo make_property_info(const QString &name, int t, const QString &typeName, const QVariant &value, const QVariant &defValue = QVariant(), const QVariantHash &props = QVariantHash())
{
    QString type(typeName);
    QString widget;
    bool readonly = false;

    if(type == "int")
    {
        if(props.contains("enum"))
            widget = "combobox";
        if(props.contains("option"))
            widget = "checkbox";
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
    else if(type == "QColor")
        widget = "colordialog";
    else if(type == "QString")
    {
        if(props.value("multiline").toBool())
            widget = "textedit";
        else if(props.value("file").toBool())
            widget = "filedialog";
        else
            widget = "lineedit";
    }
    else
    {
        if(t == QMetaType::QObjectStar || t == QMetaType::QWidgetStar || t == QMetaType::VoidStar)
        {
            widget = "";
            readonly = true;
        }
        else
        {
            if(type == "NLVector3" || type == "vector3_s" || type == "vector3_t")
            {
                if(type != "NLVector3")
                    type = "NLVector3";
                widget = "vector3";
            }
            else if(type == "NLRenderable*" || type == "NLObject*" || type == "NLActor*" || type == "NLComponent*" || type == "NLScript*" || type == "NLScene*" || type == "NLSceneCamera*" || type == "NLForce*")
            {
                widget = "";
                readonly = true;
            }
        }
    }

    //qDebug() << name << t << value << QMetaType::QObjectStar << t << QMetaType::VoidStar << value.typeName();
    return(NLPropertyInfo(name, value, type, widget, readonly, defValue, props));
}

NLPropertyInfoList object_propertics(const NLObject *obj)
{
    const QMetaObject *metaObj = obj->metaObject();
    NLPropertyInfoList ret;
    const NLProperties config = obj->PropertyConfig();

    for(int i = 0; i < metaObj->propertyCount(); i++)
    {
        QMetaProperty p = metaObj->property(i);
        QString name(p.name());

        QVariantHash prop = config.value(name).toHash();
        NLPropertyInfo info = make_property_info(name, p.type(), p.typeName(), p.read(obj), obj->GetInitProperty(name), prop);

        ret.push_back(info);
    }

    // dynamic property
    const QList<QByteArray> list = obj->dynamicPropertyNames();
    Q_FOREACH(const QByteArray &ba, list)
    {
        QVariant p = obj->property(ba.constData());
        QString name(ba);

        QVariantHash prop = config.value(name).toHash();
        NLPropertyInfo info = make_property_info(name, p.type(), p.typeName(), p, obj->GetInitProperty(name), prop);

        ret.push_back(info);
    }

    return ret;
}

NLPropertyInfoList scene_propertics(const NLScene *obj)
{
    const QMetaObject *metaObj = obj->metaObject();
    NLPropertyInfoList ret;
    int offset = obj->PropertyNames().indexOf("fps");
    const NLProperties config = obj->PropertyConfig();

    for(int i = offset; i < metaObj->propertyCount(); i++)
    {
        QMetaProperty p = metaObj->property(i);
        QString name(p.name());

        QVariantHash prop = config.value(name).toHash();
        QVariant v = p.read(obj);
        NLPropertyInfo info = make_property_info(name, p.type(), p.typeName(), v, v, prop);

        ret.push_back(info);
    }

    // dynamic property
    const QList<QByteArray> list = obj->dynamicPropertyNames();
    Q_FOREACH(const QByteArray &ba, list)
    {
        QVariant p = obj->property(ba.constData());
        QString name(ba);

        QVariantHash prop = config.value(name).toHash();
        NLPropertyInfo info = make_property_info(name, p.type(), p.typeName(), p, p, prop);

        ret.push_back(info);
    }

    return ret;
}

bool property_equals(const QVariant &a, const QVariant &b)
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
