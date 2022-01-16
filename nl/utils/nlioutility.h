#ifndef _KARIN_NLIOUTILITY_H
#define _KARIN_NLIOUTILITY_H

#include <QtGlobal>
#include <QString>

#include "engine/nldef.h"

class QString;

class NLLIB_EXPORT NLIOUtility
{
public:
    static int file_put_contents(const QString &file, const char *data, quint64 len, int flags = 0);
    static char * file_get_contents(const QString &file, quint64 *len);
    static int file_put_contents(const QString &file, const QString &str, const QString &codec = QString(), int flags = 0);

private:
    NLIOUtility();
};

#endif // _KARIN_NLIOUTILITY_H
