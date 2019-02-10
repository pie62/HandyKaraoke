#ifndef SONGDATABASE_H
#define SONGDATABASE_H

#include "Song.h"

#include <QObject>
#include <QSqlDatabase>
#include <QThread>


enum class SearchType {
    ByAll,
    ById,
    ByName,
    ByArtist
};

enum class UpdateType {
    UpdateAll,
    ImportNCN
};

class SongDatabase : public QThread
{
    Q_OBJECT
public:
    SongDatabase();
    ~SongDatabase();

    bool isNewVersion();
    void updateToNewVersion();

    int count();
    Song* currentSong() { return song; }
    QString searchText() { return _searchText; }

    QSqlDatabase* database() { return &db; }

    bool isOpenned() { return db.isOpen(); }
    bool isUpdatting() { return upTing; }
    int updateCount() { return upCount; }

    static bool isNCNPath(const QString &dir);
    static QString getCurFilePath(const QString &midFilePath);
    static QString getLyrFilePath(const QString &midFilePath);

    QString ncnPath() { return _ncnPath; }
    bool setNcnPath(const QString &dir);

    QString hnkPath() { return _hnkPath; }
    void setHNKPath(const QString &p) { _hnkPath = p; }

    QString karPath() { return _karPath; }
    void setKarPath(const QString &p) { _karPath = p; }

    UpdateType updateType() { return upType; }
    void setUpdateType(UpdateType type);

public slots:
    bool insertNCN(const QString &ncnPath, const QString &songId, const QString &midFilePath);
    bool insertHNK(const QString &hnkPath, const QString &songId, const QString &hnkFilePath);
    bool insertKAR(const QString &karPath, const QString &songId, const QString &karFilePath, const QString &fileName);

    void setSearchType(SearchType t) { searchType = t; }
    Song *nextType(const QString &s);
    Song* search(const QString &s);
    Song* searchNext();
    Song* searchPrevious();

signals:
    void updateCountChanged(int c);
    void updatePositionChanged(int p);
    void updateSongNameChanged(QString n);

protected:
    void run();

private:
    void createIndex();
    void dropIndex();

private:
    QSqlDatabase db;
    Song *song;
    SearchType searchType;

    QString _searchText = "";

    UpdateType upType = UpdateType::UpdateAll;
    QString _ncnPath = "";
    QString _hnkPath = "";
    QString _karPath = "";


    int upCount = 0;
    bool upTing = false;

    int currentResultIndex = -1;
};

#endif // SONGDATABASE_H
