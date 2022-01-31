#include "nlfuncs.h"

#include <QDebug>
#include <QMetaObject>
#include <QMetaProperty>

#include "common/nldef.h"
#include "engine/nlscene.h"
#include "engine/nlcomponent.h"
#include "engine/nlforce.h"
#include "engine/nlscript.h"
#include "engine/nlrenderable.h"
#include "engine/nlrigidbody.h"
#include "common/nlglobals.h"

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

    QByteArray ba = typeName.toLocal8Bit();
    //qDebug() << name << t << value << QMetaType::QObjectStar << type << QMetaType::VoidStar << value.typeName() << widget << QMetaType::type(ba.constData()) << QVariant(QMetaType::type(ba.constData()), (void *)0);
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
    int at = a.type();
    int bt = b.type();
    if(at == QVariant::UserType && bt == QVariant::UserType)
    {
        at = a.userType();
        bt = b.userType();
        if(at == bt)
        {
            QString typeName(QMetaType::typeName(at));
            NLVariantCompare_f func = NLEngineGlobals::Instance()->variant_compare_func(typeName);
            if(func)
                return func(a, b);
        }
    }
    return a == b; // internal type and user pointer
}

QVariant object_to_qvaraint(NLObject *nlo)
{
    if(!nlo)
        return QVariant();
    QByteArray className = nlo->ClassName().toLocal8Bit();
    int type = QMetaType::type(className + "*");
    if(type != 0)
        return QVariant(type, (void *)&nlo);

    switch(nlo->Type())
    {
#define CASE_EQUALS(type, T) \
    case NLObject::type: \
        return QVariant::fromValue<T *>(static_cast<T *>(nlo));

    case NLObject::Type_Actor:
        if(nlinstanceofv(nlo, NLRigidbody))
            return QVariant::fromValue<NLRigidbody *>(static_cast<NLRigidbody *>(nlo));
        else
            return QVariant::fromValue<NLActor *>(static_cast<NLActor *>(nlo));
    CASE_EQUALS(Type_Component, NLComponent)
    CASE_EQUALS(Type_Script, NLScript)
    CASE_EQUALS(Type_Force, NLForce)
    CASE_EQUALS(Type_Renderer, NLRenderable)

#undef CASE_EQUALS
    default:
        return QVariant();
    }
}

int qvaraint_to_pointer(const QVariant &v, void *&ptr, QString &typeName)
{
    if(!v.isValid())
        return QVariant::Invalid;
    int type = v.userType();
    QString name(QMetaType::typeName(type));
    if(type == QMetaType::VoidStar)
    {
        ptr = v.value<void *>();
    }
    else if(type == QMetaType::QObjectStar)
    {
        ptr = v.value<QObject *>();
    }
    else if(type == QMetaType::QWidgetStar)
    {
        ptr = v.value<QWidget *>();
    }
    else
    {
        if(name.endsWith("*"))
        {
            ptr = *(void **)(v.data());
        }
        else
            type = QVariant::Invalid;
    }
    if(type != QVariant::Invalid)
        typeName = name;
    return type;
}

void * qvaraint_to_void_pointer(const QVariant &v, QString *typeName)
{
    QString name;
    void *ptr = 0;
    /*int type = */qvaraint_to_pointer(v, ptr, name);
    if(typeName)
        *typeName = name;
    return ptr;
}
}
