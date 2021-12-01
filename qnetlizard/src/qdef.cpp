#include "qdef.h"

#include <QDateTime>
#include <QLocale>
#include <QRegExp>

namespace NETLizard
{
QDateTime build_time()
{
    static QDateTime dt;
    if(!dt.isValid())
        dt = BUILD_TIME;
    return dt;
}

QString build_time_string(const QString &format)
{
    return build_time().toString(format);
}
}
