#ifndef _KARIN_QDEF_H
#define _KARIN_QDEF_H

#define DEBUG_DESTROY_Q qDebug() << "Q ~" + objectName();
#define DEBUG_DESTROY(name) qDebug() << QString("~") + (name);

#define LIB_NAME "libnetlizard"
#define APP_NAME "QNETLizard"
#define APP_DEV "Karin"
#define APP_VER "2.0.0harmattan9"
#define APP_CODE "ct3d"
#define APP_RELEASE 1

#define APP_GITHUB "https://github.com/glKarin/netlizard"

#ifndef instanceof
#define instanceof(obj, T) ((dynamic_cast<T *>(&obj)) != 0)
#define instanceofv(obj, T) ((dynamic_cast<T *>(obj)) != 0)
#endif

#define SINGLE_INSTANCE_DEF(C) static C * Instance();
#define SINGLE_INSTANCE_DECL(C) C * C::Instance() \
{\
    static C _instance; \
    return &_instance; \
}
#define SINGLE_INSTANCE_OBJ(C) C::Instance()

#endif // _KARIN_QDEF_H
