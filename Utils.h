#ifndef UTILS_H
#define UTILS_H

#include <QFile>
#include <QMenu>
#include <QSignalMapper>

#include "Song.h"
#include "Midi/MidiSynthesizer.h"

#ifdef _WIN32
typedef __int32 int32_t;
typedef unsigned __int32 uint32_t;
#endif

class Utils
{
public:
    Utils();

    static QVector<long> readCurFile(const QString &curFile, uint32_t resolution);
    static QVector<long> readCurFile(const QByteArray &data, uint32_t resolution);

    static QString readLyrics(const QString &lyrFile);
    static QString readLyrics(const QByteArray &data);

    static uint concurentThreadsSupported();

    static bool vstInfo(const QString &vstPath, VSTNamePath *info);

    static bool savePlaylist(const QString &filePath, const QList<Song*> &songs);
    static bool loadPlaylist(const QString &filePath, QList<Song*> &songs);

    static QString LAST_OPEN_DIR;
};

#endif // UTILS_H
