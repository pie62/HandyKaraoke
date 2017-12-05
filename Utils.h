#ifndef UTILS_H
#define UTILS_H

#include <cstdint>
#include <QFile>

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
