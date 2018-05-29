#include "Utils.h"

#include <thread>

#include <QDataStream>
#include <QTextStream>
#include <QProcess>
#include <QDir>

#include "Config.h"

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

QString Utils::LAST_OPEN_DIR = QDir::homePath();
