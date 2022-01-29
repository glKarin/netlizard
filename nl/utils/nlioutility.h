#ifndef _KARIN_NLIOUTILITY_H
#define _KARIN_NLIOUTILITY_H

#include <QtGlobal>
#include <QString>

#include "common/nlinc.h"

class NLLIB_EXPORT NLIOUtility
{
public:
    static bool file_put_contents(const QString &file, const char *data, uint len, int flags = 0);
    static char * file_get_contents(const QString &file, uint *len);
    static bool file_put_contents(const QString &file, const QByteArray &data, int flags = 0);
    static QByteArray file_get_contents(const QString &file);
    static bool file_put_contents(const QString &file, const QString &str, const QString &codec = QString(), int flags = 0);
    static QString file_get_contents(const QString &file, const QString &codec = QString());

private:
    NLIOUtility();
};

#endif // _KARIN_NLIOUTILITY_H
