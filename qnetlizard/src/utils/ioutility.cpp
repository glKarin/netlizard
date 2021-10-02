#include "ioutility.h"

#include <QFile>
#include <QDataStream>

IOUtility::IOUtility()
{
}

int IOUtility::file_put_contents(const QString &file, const char *data, quint64 len, int flags)
{
    bool res;

    res = false;
    QFile f(file);
    if(!f.open((flags & 1) ? QIODevice::Append : QIODevice::WriteOnly))
        return false;
    QDataStream os(&f);
    int l = os.writeRawData(data, len);
    if(l != -1 && l == len)
        res = true;
    f.flush();
    f.close();
    return res;
}

char * IOUtility::file_get_contents(const QString &file, quint64 *len)
{
    QFile f(file);
    if(!f.exists())
        return NULL;
    if(!f.open(QIODevice::ReadOnly))
        return NULL;
    QDataStream is(&f);
    QByteArray ba;
    const int BUF_SIZE = 1024;
    int l;
    char buf[BUF_SIZE];
    while((l = is.readRawData(buf, BUF_SIZE)) > 0)
    {
        ba.append(buf, l);
    }
    f.close();
    int size = ba.length();
    char *res = new char[size];
    memcpy(res, ba.constData(), size);
    if(len)
        *len = size;
    return res;
}
