#include "Utils.h"

#include <thread>

#include <QDataStream>
#include <QTextStream>
#include <QProcess>

Utils::Utils()
{

}

QList<long> Utils::readCurFile(const QString &curFile, uint32_t resolution)
{
    QList<long> curs;
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

QList<long> Utils::readCurFile(const QByteArray &data, uint32_t resolution)
{
    QList<long> curs;
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
    param << vstPath << VST_INFO_FILE_PATH;

    int exitCode = QProcess::execute(VST_CHECKER_NAME, param);

    if (exitCode != 0)
        return false;

    QDir dir(VST_INFO_FILE_DIR);
    if (!dir.exists())
        dir.mkpath(VST_INFO_FILE_DIR);

    QFile file(VST_INFO_FILE_PATH);
    if (!file.open(QIODevice::ReadOnly))
        return false;

    QTextStream stream(&file);

    info->uniqueID = stream.readLine().toUInt();
    info->vstName = stream.readLine();
    info->vstvendor = stream.readLine();
    info->vstPath = vstPath;

    file.close();

    return true;
}
