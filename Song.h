#ifndef SONG_H
#define SONG_H

#include <QObject>

class Song : public QObject
{
    Q_OBJECT
public:
    explicit Song(QObject *parent = 0);
    Song(const Song &s);
    Song& operator = (const Song &s);

    QString id()        { return sId; }
    QString name()      { return sName; }
    QString artist()    { return sArtist; }
    QString key()       { return sKey; }
    int     tempo()     { return sTempo; }
    int     bpmSpeed()  { return sBpmSpeed; }
    QString songType()  { return sSongType; }
    QString lyrics()    { return sLyrics.replace("\n", " "); }
    QString path()      { return sPath; }

    void setId          (const QString &id)     { sId = id; }
    void setName        (const QString &name)   { sName = name; }
    void setArtist      (const QString &artist) { sArtist = artist; }
    void setKey         (const QString &key)    { sKey = key; }
    void setTempo       (int tempo)             { sTempo = tempo; }
    void setBpmSpeed    (int bpm)               { sBpmSpeed = bpm; }
    void setSongType    (const QString &type)   { sSongType = type; }
    void setLyrics      (const QString &lyr)    { sLyrics = lyr; }
    void setPath        (const QString &path)   { sPath = path; }

    QString detail();

signals:

public slots:

private:
    QString sId;
    QString sName;
    QString sArtist;
    QString sKey;
    int     sTempo;
    int     sBpmSpeed;
    QString sSongType;
    QString sLyrics;
    QString sPath;
};

#endif // SONG_H
