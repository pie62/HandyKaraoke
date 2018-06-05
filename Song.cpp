#include "Song.h"

Song::Song(QObject *parent) : QObject(parent)
{
    sId         = "";
    sName       = "";
    sArtist     = "";
    sKey        = "";
    sTempo      = 0;
    sBpmSpeed   = 0;
    sTranspose  = 0;
    sSongType   = "";
    sLyrics     = "";
    sPath       = "";
}

Song::Song(const Song &s)
{
    sId         = s.sId;
    sName       = s.sName;
    sArtist     = s.sArtist;
    sKey        = s.sKey;
    sTempo      = s.sTempo;
    sBpmSpeed   = s.sBpmSpeed;
    sTranspose  = s.sTranspose;
    sSongType   = s.sSongType;
    sLyrics     = s.sLyrics;
    sPath       = s.sPath;
}

Song &Song::operator =(const Song &s)
{
    sId         = s.sId;
    sName       = s.sName;
    sArtist     = s.sArtist;
    sKey        = s.sKey;
    sTempo      = s.sTempo;
    sBpmSpeed   = s.sBpmSpeed;
    sTranspose  = s.sTranspose;
    sSongType   = s.sSongType;
    sLyrics     = s.sLyrics;
    sPath       = s.sPath;

    return *this;
}

QString Song::detail()
{
    QString d = sId + "  " + sName + " - " + sArtist;
    d += "  (" + QString::number(sTempo + sBpmSpeed);
    d += sKey == "" ? ")" : "-" + sKey + ")";
    if (sBpmSpeed != 0)
    {
        QString s = sBpmSpeed > 0 ? "+" + QString::number(sBpmSpeed) : QString::number(sBpmSpeed);
        d += " (" + s + ")";
    }
    if (sTranspose != 0)
    {
        QString str = sTranspose > 0 ? "+" + QString::number(sTranspose) : QString::number(sTranspose);
        d += " (Key " + str + ")";
    }
    d += "  [" + sSongType + "]";
    return d;
}

QString Song::detailWithoutIDType()
{
    QString d = sName + " - " + sArtist;
    d += "  (" + QString::number(sTempo + sBpmSpeed);
    d += sKey == "" ? ")" : "-" + sKey + ")";
    if (sBpmSpeed != 0)
    {
        QString s = sBpmSpeed > 0 ? "+" + QString::number(sBpmSpeed) : QString::number(sBpmSpeed);
        d += " (" + s + ")";
    }
    if (sTranspose != 0)
    {
        QString str = sTranspose > 0 ? "+" + QString::number(sTranspose) : QString::number(sTranspose);
        d += " (Key " + str + ")";
    }

    return d;
}
