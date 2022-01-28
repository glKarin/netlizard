#ifndef _KARIN_NLFUNCS_H
#define _KARIN_NLFUNCS_H

#include "engine/nlproperties.h"

class NLObject;
class NLScene;

namespace NL
{
template <class T> T clamp(T t, T min, T max);

NLLIB_EXPORT NLPropertyInfoList object_propertics(const NLObject *obj);
NLLIB_EXPORT NLPropertyInfoList scene_propertics(const NLScene *obj);
NLLIB_EXPORT bool property_equals(const QVariant &a, const QVariant &b);
NLLIB_EXPORT QVariant object_to_qvaraint(NLObject *nlo);
NLLIB_EXPORT int qvaraint_to_pointer(const QVariant &value, void *&ptr, QString &typeName);
NLLIB_EXPORT void * qvaraint_to_void_pointer(const QVariant &value, QString *typeName = 0);
template <class T>
T qvaraint_pointer_cast(const QVariant &value, QString *typeName = 0);
}

template <class T>
T NL::qvaraint_pointer_cast(const QVariant &v, QString *typeName)
{
    T ret = 0;
    QString name;
    if(v.isValid())
    {
        int type = v.userType();
        name = QMetaType::typeName(type);
        if(v.canConvert<T>())
            ret = v.value<T>();
        else
        {
            if(type == QMetaType::VoidStar)
            {
                ret = reinterpret_cast<T>(v.value<void *>());
            }
            else if(type == QMetaType::QObjectStar)
            {
                QObject *obj = v.value<QObject *>();
                ret = dynamic_cast<T>(obj);
                if(obj && !ret)
                    name = QString();
            }
            else if(type == QMetaType::QWidgetStar)
            {
                QWidget *wid = v.value<QWidget *>();
                ret = dynamic_cast<T>(wid);
                if(wid && !ret)
                    name = QString();
            }
            else
            {
                if(name.endsWith("*"))
                {
                    ret = reinterpret_cast<T>(*(void **)(v.data()));
                }
                else
                    name = QString();
            }
        }
    }
    if(typeName)
        *typeName = name;
    return ret;
}

template <class T> T NL::clamp(T t, T min, T max)
{
    return qMax(min, qMin(t, max));
}

#endif // _KARIN_NLFUNCS_H
