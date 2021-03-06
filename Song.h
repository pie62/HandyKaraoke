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
    int     transpose() { return sTranspose;  }
    QString songType()  { return sSongType; }
    QString lyrics()    { return sLyrics.replace("\n", " "); }
    QString path()      { return sPath; }

    int     cutStartBar()   { return sCutStartBar; }
    int     cutEndBar()     { return sCutEndBar; }

    void setId          (const QString &id)     { sId = id; }
    void setName        (const QString &name)   { sName = name; }
    void setArtist      (const QString &artist) { sArtist = artist; }
    void setKey         (const QString &key)    { sKey = key; }
    void setTempo       (int tempo)             { sTempo = tempo; }
    void setBpmSpeed    (int bpm)               { sBpmSpeed = bpm; }
    void setTranspose   (int t)                 { sTranspose = t; }
    void setSongType    (const QString &type)   { sSongType = type; }
    void setLyrics      (const QString &lyr)    { sLyrics = lyr; }
    void setPath        (const QString &path)   { sPath = path; }

    void setCutStartBar (int bar)               { sCutStartBar = bar; }
    void setCutEndBar   (int bar)               { sCutEndBar = bar; }

    QString detail();
    QString detailWithoutIDType();

signals:

public slots:

private:
    QString sId;
    QString sName;
    QString sArtist;
    QString sKey;
    int     sTempo;
    int     sBpmSpeed;
    int     sTranspose;
    QString sSongType;
    QString sLyrics;
    QString sPath;

    int     sCutStartBar;
    int     sCutEndBar;
};

#endif // SONG_H
