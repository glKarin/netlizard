#ifndef _KARIN_QDEF_H
#define _KARIN_QDEF_H

#ifdef _DEV_TEST
#define DEBUG_DESTROY_Q qDebug() << "Q ~" + objectName();
#define DEBUG_DESTROY(name) qDebug() << QString("~") + (name);
#else
#define DEBUG_DESTROY_Q
#define DEBUG_DESTROY(name)
#endif

#define LIB_NAME "libnetlizard"
#define APP_NAME "QNETLizard"
#define APP_DEV "Karin"
#define APP_VER "2.0.0harmattan9"
#define APP_CODE "ct3d"
#define APP_RELEASE 1
#define APP_EMAIL "beyondk2000@gmail.com"

#define APP_GITHUB "https://github.com/glKarin/netlizard"

#define qClamp(t, min, max) qMax(min, qMin(t, max))

#ifndef instanceof
#define instanceof(obj, T) ((dynamic_cast<T *>(&obj)) != 0)
#define instanceofv(obj, T) ((dynamic_cast<T *>(obj)) != 0)
#endif

#ifndef countof
#define countof(arr) (sizeof(arr) / sizeof(arr[0]))
#define lengthof countof
#endif

#define SINGLE_INSTANCE_DEF(C) static C * Instance();
#define SINGLE_INSTANCE_DECL(C) C * C::Instance() \
{\
    static C _instance; \
    return &_instance; \
}
#define SINGLE_INSTANCE_OBJ(C) C::Instance()

#define _B(x) "<b>"x"</b>"
#define _I(x) "<i>"x"</i>"
#define _F(x, color) "<font color=\""color"\">"x"</font>"
#define _S(x, size) "<font size=\""size"\">"x"</font>"

#define BUILD_TIME QLocale(QLocale::English).toDateTime(__DATE__ " " __TIME__, "MMM dd yyyy hh:mm:ss")
#define BUILD_TIMEZONE "+0800" // zh_cn

#define BITS(x) (1 << (x))
#define BITS_ALL (~0U)

#define SWAP(a, b, T) \
{ \
    T __x = (a); \
    a = (b) ; \
    b = __x; \
}

#define SWAPV(a, b, T) \
{ \
    T __x = *(a); \
    *(a) = *(b) ; \
    *(b) = __x; \
}

#define PTR_MOVE(dst, src) \
{ \
    (dst) = (src); \
    (src) = NULL; \
}

#define PTR_DELETE(T, x) \
if(x) { \
    T *_Tx = x; \
    x = 0; \
    delete _Tx; \
}

#endif // _KARIN_QDEF_H
