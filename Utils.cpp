#include "Utils.h"

#include <thread>

#include <QDataStream>
#include <QTextStream>
#include <QProcess>
#include <QDir>

#include "Config.h"
#include "Midi/MidiHelper.h"

Utils::Utils()
{

}

QVector<long> Utils::readCurFile(const QString &curFile, uint32_t resolution)
{
    QVector<long> curs;
    QFile f(curFile);
    f.open(QIODevice::ReadOnly);

    QDataStream in(&f);
    while (!in.atEnd()) {
        quint8 b1 = 0;
        quint8 b2 = 0;
        in >> b1;
        in >> b2;
        long cs = (b1 + (b2 << 8)) * resolution / 24;
        curs.append(cs);
    }
    f.close();
    return curs;
}

QVector<long> Utils::readCurFile(const QByteArray &data, uint32_t resolution)
{
    QVector<long> curs;
    QDataStream in(data);
    while (!in.atEnd()) {
        quint8 b1 = 0;
        quint8 b2 = 0;
        in >> b1;
        in >> b2;
        long cs = (b1 + (b2 << 8)) * resolution / 24;
        curs.append(cs);
    }
    return curs;
}

QString Utils::readLyrics(const QString &lyrFile)
{
    QFile f(lyrFile);
    f.open(QIODevice::ReadOnly);

    QTextStream textStream(&f);
    textStream.setCodec("TIS-620");

    textStream.readLine();
    textStream.readLine();
    textStream.readLine();
    textStream.readLine();

    QString lyr = textStream.readAll();

    f.close();

    return lyr;
}

QString Utils::readLyrics(const QByteArray &data)
{
    QTextStream textStream(data);
    textStream.setCodec("TIS-620");

    textStream.readLine();
    textStream.readLine();
    textStream.readLine();
    textStream.readLine();

    QString lyr = textStream.readAll();

    return lyr;
}

uint Utils::concurentThreadsSupported()
{
    return std::thread::hardware_concurrency();
}

bool Utils::vstInfo(const QString &vstPath, VSTNamePath *info)
{
    QStringList param;
    param << vstPath;

    QProcess process;
    process.start(VST_CHECKER_NAME, param);
    process.waitForFinished(-1);

    if (process.exitCode() != 0)
        return false;

    QString strout = process.readAllStandardOutput();

    #ifdef _WIN32
    QStringList vstStrInfo = strout.split("\r\n");
    #else
    QStringList vstStrInfo = strout.split("\n");
    #endif

    info->uniqueID = vstStrInfo[0].toUInt();
    info->vstName = vstStrInfo[1];
    info->vstvendor = vstStrInfo[2];
    info->vstPath = vstPath;

    return true;
}

bool Utils::savePlaylist(const QString &filePath, const QList<Song *> &songs)
{
    QFile file(filePath);

    if (!file.open(QIODevice::WriteOnly|QIODevice::Truncate))
        return false;

    QTextStream out(&file);
    out.setCodec("UTF-8");

    out << HANDY_PLAYLIST_HEADTEXT << endl;
    out << HANDY_PLAYLIST_VERSION_2 << endl;
    out << HANDY_PLAYLIST_NAME + QFileInfo(filePath).baseName() << endl;
    out << HANDY_PLAYLIST_COUNT + QString::number(songs.count()) << endl;

    out << endl;

    out << HANDY_PLAYLIST_SONGLIST << endl;

    for (int i=0; i<songs.count(); i++)
    {
        Song *s = songs[i];
        out << HANDY_PLAYLIST_SONG + QString::number(i+1) << endl;
        out << s->id() << endl;
        out << s->name() << endl;
        out << s->artist() << endl;
        out << s->key() << endl;
        out << s->tempo() << endl;
        out << s->bpmSpeed() << endl;
        out << s->transpose() << endl;
        out << s->songType() << endl;
        out << s->lyrics() << endl;
        out << s->path() << endl;
        out << s->cutStartBar() << endl;
        out << s->cutEndBar() << endl;
        out << endl;
    }

    out << HANDY_PLAYLIST_END_SONGLIST;

    file.close();

    return true;
}

bool Utils::loadPlaylist(const QString &filePath, QList<Song *> &songs)
{
    int version = 0;

    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly))
        return false;

    QTextStream in(&file);
    in.setCodec("UTF-8");

    if (in.readLine() != HANDY_PLAYLIST_HEADTEXT)
    {
        file.close();
        return false;
    }

    QString versionString = in.readLine();
    if (versionString == HANDY_PLAYLIST_VERSION_1)
        version = 1;
    else if (versionString == HANDY_PLAYLIST_VERSION_2)
        version = 2;
    else
    {
        file.close();
        return false;
    }

    if (in.readLine().indexOf(HANDY_PLAYLIST_NAME) != 0)
    {
        file.close();
        return false;
    }

    QString countText = in.readLine();
    if (countText.indexOf(HANDY_PLAYLIST_COUNT) != 0)
    {
        file.close();
        return false;
    }

    int count = countText.replace(HANDY_PLAYLIST_COUNT, "").toInt();

    in.readLine();

    if (in.readLine() != HANDY_PLAYLIST_SONGLIST)
    {
        file.close();
        return false;
    }

    bool valid = true;
    QList<Song*> songList;

    for (int i=0; i<count; i++)
    {
        QString songListText = in.readLine();
        if (songListText.indexOf(HANDY_PLAYLIST_SONG) != 0)
        {
            valid = false;
            break;
        }
        if (songListText.replace(HANDY_PLAYLIST_SONG, "").toInt() != (i+1))
        {
            valid = false;
            break;
        }

        Song *s = new Song();
        s->setId(in.readLine());
        s->setName(in.readLine());
        s->setArtist(in.readLine());
        s->setKey(in.readLine());
        s->setTempo(in.readLine().toInt());
        s->setBpmSpeed(in.readLine().toInt());
        s->setTranspose(in.readLine().toInt());
        s->setSongType(in.readLine());
        s->setLyrics(in.readLine());
        s->setPath(in.readLine());

        if (version >= 2)
        {
            s->setCutStartBar(in.readLine().toInt());
            s->setCutEndBar(in.readLine().toInt());
        }

        in.readLine();

        songList.append(s);
    }

    if (valid && (in.readLine() != HANDY_PLAYLIST_END_SONGLIST))
        valid = false;

    file.close();

    if (valid)
    {
        for (Song *s : songs)
            delete s;
        songs.clear();
        songs = songList;
        return true;
    }
    else
    {
        for (Song *s : songList)
            delete s;
        songList.clear();
        return false;
    }
}

QString Utils::LAST_OPEN_DIR = QDir::homePath();
