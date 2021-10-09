#ifndef _KARIN_QDEF_H
#define _KARIN_QDEF_H

#define DEBUG_DESTROY_Q qDebug() << "Q ~" << objectName();

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

#endif // _KARIN_QDEF_H
