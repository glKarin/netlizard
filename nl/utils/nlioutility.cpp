#include "nlioutility.h"

#include <QDebug>
#include <QFile>
#include <QDataStream>
#include <QTextStream>
#include <QTextCodec>

int NLIOUtility::file_put_contents(const QString &file, const char *data, quint64 len, int flags)
{
    bool res;

    res = false;
    QFile f(file);
    QIODevice::OpenMode mode = QIODevice::WriteOnly;
    if(flags & 1)
        mode |= QIODevice::Append;
    if(!f.open(mode))
        return false;
    QDataStream os(&f);
    int l = os.writeRawData(data, len);
    if(l != -1 && (uint)l == len)
        res = true;
    f.flush();
    f.close();
    return res;
}

char * NLIOUtility::file_get_contents(const QString &file, quint64 *len)
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

int NLIOUtility::file_put_contents(const QString &file, const QString &str, const QString &code, int flags)
{
    bool res;

    res = false;
    QFile f(file);
    QIODevice::OpenMode mode = QIODevice::WriteOnly | QIODevice::Text;
    if(flags & 1)
        mode |= QIODevice::Append;

    if(!f.open(mode))
        return false;
    QTextStream os(&f);
    if(!code.isEmpty())
    {
        QTextCodec *codec = QTextCodec::codecForName(code.toLocal8Bit());
        if(codec)
            os.setCodec(codec);
    }
    os << str;
    f.flush();
    f.close();
    return res;
}
