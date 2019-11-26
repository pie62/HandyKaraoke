#include "SongDatabase.h"

#include "Midi/MidiFile.h"
#include "Midi/HNKFile.h"
#include "Config.h"

#include <QDir>
#include <QDirIterator>
#include <QFile>
#include <QSqlQuery>
#include <QTextStream>


SongDatabase::SongDatabase()
{
    song = new Song();
    searchType = SearchType::ByAll;

    bool hasDb = false;

    if (QFile::exists(Config::DATABASE_FILE_PATH)) {
        hasDb = true;
    } else {
        QDir dir(Config::DATABASE_DIR_PATH);
        if (!dir.exists())
            dir.mkpath(Config::DATABASE_DIR_PATH);
    }

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(Config::DATABASE_FILE_PATH);

    if (db.open()) {

        if (hasDb == false) {
            QString sql =
                    "CREATE TABLE songs ("
                        "id       TEXT    COLLATE NOCASE,"
                        "name     TEXT    COLLATE NOCASE,"
                        "artist   TEXT    COLLATE NOCASE,"
                        "keyname  TEXT,"
                        "tempo    INTEGER,"
                        "songtype TEXT,"
                        "lyrics   TEXT,"
                        "path     TEXT"
                    ")";

            QSqlQuery query(db);
            query.exec(sql);
            query.finish();
            query.clear();

            sql = "CREATE TABLE IF NOT EXISTS miscellaneous ("
                    "name TEXT,"
                    "value_str TEXT,"
                    "value_num INTEGER"
                  ")";

            query.exec(sql);
            query.finish();
            query.clear();

            createIndex();
        }
        db.close();
    }

    db.open();
}

SongDatabase::~SongDatabase()
{
    if (db.isOpen()) {
        db.close();
    }
    delete song;
}

bool SongDatabase::isNewVersion()
{
    QSqlQuery q(db);
    bool rs = q.exec("Select * from miscellaneous");
    q.finish();
    q.clear();

    return rs;
}

void SongDatabase::updateToNewVersion()
{
    QFile file(":/update-database.sql");
    file.open(QIODevice::ReadOnly);

    QTextStream in(&file);
    QString sqlString = in.readAll();

    file.close();

    QSqlQuery q(db);

    QStringList sqlList = sqlString.split(QChar(';'));
    for (const QString &sql : sqlList)
    {
        q.exec(sql);
        q.finish();
        q.clear();
    }

    q.exec("vacuum");
    q.finish();
    q.clear();
}

int SongDatabase::count()
{
    if (!db.isOpen())
        return 0;

    int c = 0;
    QSqlQuery q(db);
    q.exec("SELECT Count(*) FROM songs");
    if (q.next()) {
        c = q.value(0).toInt();
    }
    q.finish();
    q.clear();

    return c;
}

bool SongDatabase::isNCNPath(const QString &dir)
{
    QDir d;
    QString cur = dir + "/Cursor";
    QString lyr = dir + "/Lyrics";
    QString mid = dir + "/Song";

    if (d.exists(cur) && d.exists(lyr) && d.exists(mid)) {
        return true;
    } else {
        return false;
    }
}

QString SongDatabase::getCurFilePath(const QString &midFilePath)
{
    QString cur = midFilePath;

    cur = cur.replace("/Song/", "/Cursor/");

    cur = cur.replace(cur.length()-4, 4, ".CUR");
    if (QFile::exists(cur))
        return cur;

    cur = cur.replace(cur.length()-4, 4, ".cur");
    if (QFile::exists(cur))
        return cur;

    cur = cur.replace(cur.length()-4, 4, ".Cur");
    if (QFile::exists(cur))
        return cur;

    return "";
}

QString SongDatabase::getLyrFilePath(const QString &midFilePath)
{
    QString lyr = midFilePath;

    lyr = lyr.replace("/Song/", "/Lyrics/");

    lyr = lyr.replace(lyr.length()-4, 4, ".LYR");
    if (QFile::exists(lyr))
        return lyr;

    lyr = lyr.replace(lyr.length()-4, 4, ".lyr");
    if (QFile::exists(lyr))
        return lyr;

    lyr = lyr.replace(lyr.length()-4, 4, ".Lyr");
    if (QFile::exists(lyr))
        return lyr;

    return "";
}

bool SongDatabase::setNcnPath(const QString &dir)
{
    if (isNCNPath(dir)) {
        _ncnPath = dir;
        return true;
    } else {
        return false;
    }
}

void SongDatabase::setUpdateType(UpdateType type)
{
    upType = type;
}

bool SongDatabase::insertNCN(const QString &ncnPath, const QString &songId, const QString &midFilePath)
{
    QString id = songId;

    QString curFilePath = getCurFilePath(midFilePath);
    QString lyrFilePath = getLyrFilePath(midFilePath);

    if (curFilePath == "" || lyrFilePath == "")
        return false;

    // Check mid file
    int bpm = MidiFile::firstBpm(midFilePath);
    if (bpm == 0)
        return false;


    // Read .LYR file
    QFile file(lyrFilePath);
    if (!file.open(QFile::ReadOnly))
        return false;

    QTextStream textStream(&file);
    textStream.setCodec("TIS-620");

    QString name = textStream.readLine();
    QString artist = textStream.readLine();
    QString key = textStream.readLine();
    QString type = "NCN";

    textStream.readLine();

    QString lyr = textStream.readLine() + " ";
    lyr += textStream.readLine() + " ";
    lyr += textStream.readLine() + " ";
    lyr += textStream.readLine();

    file.close();

    QString path = midFilePath;
    path = path.replace(ncnPath, "");

    // Insert to database
    QSqlQuery query(db);
    query.prepare("INSERT INTO songs VALUES "
                  "(?, ?, ?, ?, ?, ?, ?, ?);");
    query.bindValue(0, id);
    query.bindValue(1, name);
    query.bindValue(2, artist);
    query.bindValue(3, key);
    query.bindValue(4, bpm);
    query.bindValue(5, type);
    query.bindValue(6, lyr);
    query.bindValue(7, path);

    query.exec();
    query.finish();
    query.clear();

    return true;
}

bool SongDatabase::insertHNK(const QString &hnkPath, const QString &songId, const QString &hnkFilePath)
{
    QString id = songId;

    int bpm = HNKFile::bpm(hnkFilePath);
    if (bpm == 0)
        return false;

    // Read Lyrics
    QByteArray lyrData = HNKFile::lyrData(hnkFilePath);

    QTextStream textStream(&lyrData);
    textStream.setCodec("TIS-620");

    QString name = textStream.readLine();
    QString artist = textStream.readLine();
    QString key = textStream.readLine();
    QString type = "HNK";

    textStream.readLine();

    QString lyr = textStream.readLine() + " ";
    lyr += textStream.readLine() + " ";
    lyr += textStream.readLine() + " ";
    lyr += textStream.readLine();

    QString path = hnkFilePath;
    path = path.replace(hnkPath, "");

    // Insert to database
    QSqlQuery query(db);
    query.prepare("INSERT INTO songs VALUES "
                  "(?, ?, ?, ?, ?, ?, ?, ?);");
    query.bindValue(0, id);
    query.bindValue(1, name);
    query.bindValue(2, artist);
    query.bindValue(3, key);
    query.bindValue(4, bpm);
    query.bindValue(5, type);
    query.bindValue(6, lyr);
    query.bindValue(7, path);

    query.exec();
    query.finish();
    query.clear();

    return true;
}

bool SongDatabase::insertKAR(const QString &karPath, const QString &songId, const QString &karFilePath, const QString &fileName)
{
    QString id = songId;

    MidiFile mid;
    if (!mid.read(karFilePath))
        return false;

    int bpm = 120;
    if (mid.tempoEvents().size() > 0)
        bpm = mid.tempoEvents()[0]->bpm();

    QString name = fileName.section(".", 0, 0);
    QString type = "KAR";

    QString path = karFilePath;
    path = path.replace(karPath, "");

    QString lyr = "";
    QStringList lyrics = mid.lyrics().split(QRegExp("\n|\r\n|\r"));
    if (lyrics.size() > 0)
        lyr += lyrics[0] + " ";
    if (lyrics.size() > 1)
        lyr += lyrics[1] + " ";
    if (lyrics.size() > 2)
        lyr += lyrics[2] + " ";
    if (lyrics.size() > 3)
        lyr += lyrics[3];

    QSqlQuery query(db);
    query.prepare("INSERT INTO songs VALUES "
                  "(?, ?, ?, ?, ?, ?, ?, ?);");
    query.bindValue(0, id);
    query.bindValue(1, name);
    query.bindValue(2, "");
    query.bindValue(3, "");
    query.bindValue(4, bpm);
    query.bindValue(5, type);
    query.bindValue(6, lyr);
    query.bindValue(7, path);

    query.exec();
    query.finish();
    query.clear();

    return true;
}

Song* SongDatabase::nextType(const QString &s)
{
    Song *sg = song;
    switch (searchType) {
    case SearchType::ByAll:
        searchType = SearchType::ById;
        sg = search(s);
        break;
    case SearchType::ById:
        searchType = SearchType::ByName;
        sg = search(_searchText);
        break;
    case SearchType::ByName:
        searchType = SearchType::ByArtist;
        sg = search(_searchText);
        break;
    case SearchType::ByArtist:
        searchType = SearchType::ByAll;
        sg = search(_searchText);
        break;
    }
    return sg;
}

void setSong(Song *s, QSqlQuery *qry) {
    s->setId(qry->value(0).toString());
    s->setName(qry->value(1).toString());
    s->setArtist(qry->value(2).toString());
    s->setKey(qry->value(3).toString());
    s->setTempo(qry->value(4).toInt());
    s->setSongType(qry->value(5).toString());
    s->setLyrics(qry->value(6).toString());
    s->setPath(qry->value(7).toString());

    s->setBpmSpeed(0);
    s->setTranspose(0);
}

Song *SongDatabase::search(const QString &s)
{
    QString sql = "";
    _searchText = s;
    currentResultIndex = 0;

    switch (searchType) {
    case SearchType::ByAll:
        sql = "SELECT * FROM (SELECT * FROM songs WHERE id LIKE ? AND songtype != 'KAR' ORDER BY id LIMIT 1) "
              "UNION ALL "
              "SELECT * FROM (SELECT * FROM songs WHERE name LIKE ? AND songtype != 'KAR' ORDER BY name LIMIT 1) "
              "UNION ALL "
              "SELECT * FROM (SELECT * FROM songs WHERE artist LIKE ? AND songtype != 'KAR' ORDER BY artist LIMIT 1) "
              "UNION ALL "
              "SELECT * FROM (SELECT * FROM songs WHERE name LIKE ? AND songtype = 'KAR' ORDER BY name LIMIT 1) "
              "LIMIT 1";
        break;
    case SearchType::ById:
        sql = "SELECT * FROM songs WHERE id LIKE ? "
              "ORDER BY id, name, artist LIMIT 1";
        break;
    case SearchType::ByName:
        sql = "SELECT * FROM songs WHERE name LIKE ? "
              "ORDER BY name, artist, id LIMIT 1";
        break;
    case SearchType::ByArtist:
        sql = "SELECT * FROM songs WHERE artist LIKE ? "
              "ORDER BY artist, name, id LIMIT 1";
        break;
    }

    QSqlQuery query(db);
    if (searchType == SearchType::ByAll) {
        query.prepare(sql);
        query.bindValue(0, s + "%");
        query.bindValue(1, s + "%");
        query.bindValue(2, s + "%");
        query.bindValue(3, "%" + s + "%");
    } else {
        query.prepare(sql);
        query.bindValue(0, s + "%");
    }

    if (query.exec()) {
        if (query.next()) {
            setSong(song, &query);
        }
    }
    query.finish();
    query.clear();

    return song;
}

Song *SongDatabase::searchNext()
{
    QString sql = "";

    switch (searchType) {
    case SearchType::ByAll:
        sql = "SELECT * FROM (SELECT * FROM songs WHERE id LIKE ? AND songtype != 'KAR' ORDER BY id, name, artist) "
              "UNION ALL "
              "SELECT * FROM (SELECT * FROM songs WHERE name LIKE ? AND songtype != 'KAR' ORDER BY name, artist, id) "
              "UNION ALL "
              "SELECT * FROM (SELECT * FROM songs WHERE artist LIKE ? AND songtype != 'KAR' ORDER BY artist, name, id) "
              "UNION ALL "
              "SELECT * FROM (SELECT * FROM songs WHERE name LIKE ? AND songtype = 'KAR' ORDER BY name, artist, id) ";
        break;
    case SearchType::ById:
        sql = "SELECT * FROM songs WHERE id LIKE ? "
              "ORDER BY id, name, artist";
        break;
    case SearchType::ByName:
        sql = "SELECT * FROM songs WHERE name LIKE ? "
              "ORDER BY name, artist, id";
        break;
    case SearchType::ByArtist:
        sql = "SELECT * FROM songs WHERE artist LIKE ? "
              "ORDER BY artist, name, id";
        break;
    }

    QSqlQuery q(db);
    q.prepare(sql);

    if (searchType == SearchType::ByAll) {
        q.bindValue(0, _searchText + "%");
        q.bindValue(1, _searchText + "%");
        q.bindValue(2, _searchText + "%");
        q.bindValue(3, "%" + _searchText + "%");
    } else {
        q.bindValue(0, _searchText + "%");
    }

    if (q.exec() && q.seek(currentResultIndex + 1)) {
        currentResultIndex++;
        setSong(song, &q);
    }

    q.finish();
    q.clear();

    return song;
}

Song *SongDatabase::searchPrevious()
{
    QString sql = "";

    switch (searchType) {
    case SearchType::ByAll:
        sql = "SELECT * FROM (SELECT * FROM songs WHERE id LIKE ? AND songtype != 'KAR' ORDER BY id, name, artist) "
              "UNION ALL "
              "SELECT * FROM (SELECT * FROM songs WHERE name LIKE ? AND songtype != 'KAR' ORDER BY name, artist, id) "
              "UNION ALL "
              "SELECT * FROM (SELECT * FROM songs WHERE artist LIKE ? AND songtype != 'KAR' ORDER BY artist, name, id) "
              "UNION ALL "
              "SELECT * FROM (SELECT * FROM songs WHERE name LIKE ? AND songtype = 'KAR' ORDER BY name, artist, id) ";
        break;
    case SearchType::ById:
        sql = "SELECT * FROM songs WHERE id LIKE ? "
              "ORDER BY id, name, artist";
        break;
    case SearchType::ByName:
        sql = "SELECT * FROM songs WHERE name LIKE ? "
              "ORDER BY name, artist, id";
        break;
    case SearchType::ByArtist:
        sql = "SELECT * FROM songs WHERE artist LIKE ? "
              "ORDER BY artist, name, id";
        break;
    }

    QSqlQuery q(db);
    q.prepare(sql);

    if (searchType == SearchType::ByAll) {
        q.bindValue(0, _searchText + "%");
        q.bindValue(1, _searchText + "%");
        q.bindValue(2, _searchText + "%");
        q.bindValue(3, "%" + _searchText + "%");
    } else {
        q.bindValue(0, _searchText + "%");
    }

    if (q.exec() && q.seek(currentResultIndex - 1)) {
        currentResultIndex--;
        setSong(song, &q);
    }

    q.finish();
    q.clear();

    return song;
}

bool SongDatabase::removeCurrentSong(bool removeFromStorage)
{
   QString sql = "DELETE FROM songs WHERE id = ? AND name = ? AND songtype = ? AND path = ?";

   QSqlQuery q(db);
   q.prepare(sql);
   q.bindValue(0, song->id());
   q.bindValue(1, song->name());
   q.bindValue(2, song->songType());
   q.bindValue(3, song->path());

   if (!q.exec())
       return false;

   q.finish();
   q.clear();

   currentResultIndex--;

   if (removeFromStorage)
   {
       if (song->songType() == "NCN")
       {
           QString midFilePath = _ncnPath + song->path();
           QString curFilePath = getCurFilePath(midFilePath);
           QString lyrFilePath = getLyrFilePath(midFilePath);

           QFile f(midFilePath);
           f.remove();

           f.setFileName(curFilePath);
           f.remove();

           f.setFileName(lyrFilePath);
           f.remove();
       }
       else if (song->songType() == "HNK")
       {
           QString path = _hnkPath + song->path();
           QFile f(path);
           f.remove();
       }
       else if (song->songType() == "KAR")
       {
           QString path = _karPath + song->path();
           QFile f(path);
           f.remove();
       }
   }

   return true;
}

void SongDatabase::run()
{
    if (!db.isOpen()) {
        return;
    }

    if (!isNCNPath(_ncnPath)) {
        return;
    }

    int count = 0;

    // Count mid file in NCN
    QDir dir(_ncnPath + "/Song");
    QDir hnkDir(_hnkPath);
    QDir karDir(_karPath);

    QDirIterator iter1(dir.path() ,QStringList() << "*.mid" << "*.MID",
                    QDir::Files, QDirIterator::Subdirectories);
    while (iter1.hasNext()) {
        iter1.next();
        count++;
    }

    QDirIterator iter2(hnkDir.path() ,QStringList() << "*.hnk" << "*.HNK",
                    QDir::Files, QDirIterator::Subdirectories);
    while (iter2.hasNext()) {
        iter2.next();
        count++;
    }

    QDirIterator iter3(karDir.path() ,QStringList() << "*.kar" << "*.KAR" << "*.mid" << "*.MID",
                    QDir::Files, QDirIterator::Subdirectories);
    while (iter3.hasNext()) {
        iter3.next();
        count++;
    }

    upCount = count;
    emit updateCountChanged(count);

    upTing = true;

    QSqlQuery q(db);
    q.exec("DELETE FROM songs");
    q.exec("vacuum");
    q.finish();
    q.clear();

    db.transaction();

    dropIndex();


    // Update NCN
    int i = 0;
    int erCount = 0;
    QDirIterator it(dir.path() ,QStringList() << "*.mid" << "*.MID",
                     QDir::Files, QDirIterator::Subdirectories);
    while (it.hasNext()) {

        it.next();

        i++;
        emit updatePositionChanged(i);
        emit updateSongNameChanged(it.fileName());

        QString id = it.fileName().section(".", 0, 0);

        bool result = insertNCN(_ncnPath, id, it.filePath());

        if (!result)
            erCount ++;
    }


    // Update HNK
    QDirIterator it2(hnkDir.path() ,QStringList() << "*.hnk" << "*.HNK",
                     QDir::Files, QDirIterator::Subdirectories);
    while (it2.hasNext()) {

        it2.next();

        i++;
        emit updatePositionChanged(i);
        emit updateSongNameChanged(it2.fileName());

        QString hnkId = it2.fileName().section(".", 0, 0);

        bool result = insertHNK(_hnkPath, hnkId, it2.filePath());

        if (!result)
            erCount ++;
    }

    // Update KAR
    QDirIterator it3(karDir.path() ,QStringList() << "*.kar" << "*.KAR" << "*.mid" << "*.MID",
                     QDir::Files, QDirIterator::Subdirectories);
    while (it3.hasNext()) {

        it3.next();

        i++;
        emit updatePositionChanged(i);
        emit updateSongNameChanged(it3.fileName());

        QString karId = "-------";

        bool result = insertKAR(_karPath, karId, it3.filePath(), it3.fileName());

        if (!result)
            erCount ++;
    }


    db.commit();

    createIndex();

    upTing = false;
}

void SongDatabase::createIndex()
{
    QSqlQuery query(db);
    QString sql;

    sql = "CREATE INDEX id_idx ON songs(id); ";
    query.exec(sql);
    query.finish();
    query.clear();

    sql = "CREATE INDEX name_idx ON songs(name); ";
    query.exec(sql);
    query.finish();
    query.clear();

    sql = "CREATE INDEX artist_idx ON songs(artist); ";
    query.exec(sql);
    query.finish();
    query.clear();

    sql = "CREATE INDEX compound_idx ON songs(id,name,artist); ";
    query.exec(sql);
    query.finish();
    query.clear();
}

void SongDatabase::dropIndex()
{
    QSqlQuery q(db);
    QString sql;

    sql = "DROP INDEX id_idx; ";
    q.exec(sql);
    q.finish();
    q.clear();

    sql = "DROP INDEX name_idx; ";
    q.exec(sql);
    q.finish();
    q.clear();

    sql = "DROP INDEX artist_idx; ";
    q.exec(sql);
    q.finish();
    q.clear();

    sql = "DROP INDEX compound_idx; ";
    q.exec(sql);
    q.finish();
    q.clear();
}
