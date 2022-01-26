#include "nlfuncs.h"

#include <QDebug>
#include <QMetaObject>
#include <QMetaProperty>

#include "nlobject.h"
#include "nlscene.h"
#include "nlactor.h"
#include "nlcomponent.h"
#include "nlforce.h"
#include "nlscript.h"
#include "nlrenderable.h"

namespace NL
{
static NLPropertyInfo make_property_info(const QObject *object, const QString &name, int t, const QString &typeName, const QVariant &value, const QVariant &defValue = QVariant(), const QVariantHash &props = QVariantHash(), bool readonly = false)
{
    if(typeName == "QVariant")
    {
        return make_property_info(object, name, value.type(), value.typeName(), value, defValue, props, readonly);
    }

    QString type(typeName);
    QString widget;
    bool editDirectly = true;

    if(type == "int")
    {
        if(props.contains("enum"))
            widget = "combobox";
        else if(props.contains("option"))
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
    else if(t == QMetaType::QObjectStar || t == QMetaType::QWidgetStar || t == QMetaType::VoidStar)
    {
        widget = "memory";
        editDirectly = false;
    }
    else
    {
        if(type == "NLVector3" || type == "vector3_s" || type == "vector3_t")
        {
            if(type != "NLVector3")
                type = "NLVector3";
            widget = "vector3";
        }
        else if(type == "NLVariantGeneralPointer" || type == "NLVariantVoidPointer")
        {
            if(type != "NLVariantVoidPointer")
                type = "NLVariantGeneralPointer";
            widget = "memory";
            editDirectly = false;
        }
        else if(type == "NLRenderable*" || type == "NLObject*" || type == "NLActor*" || type == "NLComponent*" || type == "NLScript*" || type == "NLScene*" || type == "NLSceneCamera*" || type == "NLForce*")
        {
            if(type == "NLRenderable*" && name == "renderable" && nlinstanceofv(object, const NLActor))
            {
                widget = "formgroup";
                editDirectly = false;
            }
            else
            {
                widget = "memory";
                editDirectly = false;
            }
        }
        else if(type.contains("*"))
        {
            widget = "memory";
            editDirectly = false;
        }
    }

    //qDebug() << name << t << value << QMetaType::QObjectStar << type << QMetaType::VoidStar << value.typeName() << widget;
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
        bool readonly = !p.isWritable() || p.isConstant();

        QVariantHash prop = config.value(name).toHash();
        NLPropertyInfo info = make_property_info(obj, name, p.type(), p.typeName(), p.read(obj), obj->GetInitProperty(name), prop, readonly);
        info.user = false;

        ret.push_back(info);
    }

    // dynamic property
    const QList<QByteArray> list = obj->dynamicPropertyNames();
    Q_FOREACH(const QByteArray &ba, list)
    {
        QVariant p = obj->property(ba.constData());
        QString name(ba);

        QVariantHash prop = config.value(name).toHash();
        NLPropertyInfo info = make_property_info(obj, name, p.type(), p.typeName(), p, obj->GetInitProperty(name), prop);
        info.user = true;

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
        bool readonly = !p.isWritable() || p.isConstant();

        QVariantHash prop = config.value(name).toHash();
        QVariant v = p.read(obj);
        NLPropertyInfo info = make_property_info(obj, name, p.type(), p.typeName(), v, v, prop, readonly);
        info.user = false;

        ret.push_back(info);
    }

    // dynamic property
    const QList<QByteArray> list = obj->dynamicPropertyNames();
    Q_FOREACH(const QByteArray &ba, list)
    {
        QVariant p = obj->property(ba.constData());
        QString name(ba);

        QVariantHash prop = config.value(name).toHash();
        NLPropertyInfo info = make_property_info(obj, name, p.type(), p.typeName(), p, p, prop);
        info.user = true;

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

QVariant object_to_qvaraint(NLObject *nlo)
{
    switch(nlo->Type())
    {
#define CASE_EQUALS(type, T) \
    case NLObject::type: \
        return QVariant::fromValue<T *>(static_cast<T *>(nlo));

    CASE_EQUALS(Type_Actor, NLActor)
    CASE_EQUALS(Type_Component, NLComponent)
    CASE_EQUALS(Type_Script, NLScript)
    CASE_EQUALS(Type_Force, NLForce)

#undef CASE_EQUALS
    default:
        return QVariant();
    }
}
}
