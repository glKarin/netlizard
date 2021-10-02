#ifndef _KARIN_IOUTILITY_H
#define _KARIN_IOUTILITY_H

#include <QtGlobal>

class QString;

class IOUtility
{
public:
    static int file_put_contents(const QString &file, const char *data, quint64 len, int flags = 0);
    static char * file_get_contents(const QString &file, quint64 *len);

private:
    IOUtility();
};

#endif // _KARIN_IOUTILITY_H
