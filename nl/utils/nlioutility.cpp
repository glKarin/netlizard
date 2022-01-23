#include "nlioutility.h"

#include <QDebug>
#include <QFile>
#include <QDataStream>
#include <QTextStream>
#include <QTextCodec>

bool NLIOUtility::file_put_contents(const QString &file, const char *data, uint len, int flags)
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

char * NLIOUtility::file_get_contents(const QString &file, uint *len)
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
    uint rlen = 0;
    char buf[BUF_SIZE];
    while((l = is.readRawData(buf, BUF_SIZE)) > 0)
    {
        ba.append(buf, l);
        rlen += l;
    }
    f.close();
    char *res = NULL;
    int size = 0;
    if(rlen)
    {
        size = ba.size();
        res = new char[size];
        memcpy(res, ba.constData(), size);
    }
    if(len)
        *len = size;
    return res;
}

bool NLIOUtility::file_put_contents(const QString &file, const QString &str, const QString &code, int flags)
{
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
    return true;
}

QString NLIOUtility::file_get_contents(const QString &file, const QString &code)
{
    QFile f(file);
    if(!f.exists())
        return QString();
    if(!f.open(QIODevice::ReadOnly | QIODevice::Text))
        return QString();
    QTextStream is(&f);
    if(!code.isEmpty())
    {
        QTextCodec *codec = QTextCodec::codecForName(code.toLocal8Bit());
        if(codec)
            is.setCodec(codec);
    }
    QString res = is.readAll();
    f.close();
    return res;
}

bool NLIOUtility::file_put_contents(const QString &file, const QByteArray &data, int flags)
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
    uint len = data.length();
    int l = os.writeRawData(data.constData(), len);
    if(l != -1 && (uint)l == len)
        res = true;
    f.flush();
    f.close();
    return res;
}

QByteArray NLIOUtility::file_get_contents(const QString &file)
{
    QByteArray ba;
    QFile f(file);
    if(!f.exists())
        return ba;
    if(!f.open(QIODevice::ReadOnly))
        return ba;
    QDataStream is(&f);
    const int BUF_SIZE = 1024;
    int l;
    char buf[BUF_SIZE];
    while((l = is.readRawData(buf, BUF_SIZE)) > 0)
    {
        ba.append(buf, l);
    }
    f.close();
    return ba;
}
