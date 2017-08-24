#ifndef UTILS_H
#define UTILS_H

#include <QFile>

class Utils
{
public:
    Utils();

    static QList<long> readCurFile(const QString &curFile, uint32_t resolution);
    static QString readLyrics(const QString &lyrFile);
};

#endif // UTILS_H
