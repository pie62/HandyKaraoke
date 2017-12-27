#ifndef UTILS_H
#define UTILS_H

#include <QFile>

#ifdef _WIN32
typedef __int32 int32_t;
typedef unsigned __int32 uint32_t;
#endif

class Utils
{
public:
    Utils();

    static QList<long> readCurFile(const QString &curFile, uint32_t resolution);
    static QList<long> readCurFile(const QByteArray &data, uint32_t resolution);

    static QString readLyrics(const QString &lyrFile);
    static QString readLyrics(const QByteArray &data);
};

#endif // UTILS_H
