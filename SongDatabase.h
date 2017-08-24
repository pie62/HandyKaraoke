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

    int count();
    Song* currentSong() { return song; }
    bool isOpenned() { return db.isOpen(); }
    bool isUpdatting() { return upTing; }
    int updateCount() { return upCount; }

    static bool isNCNPath(const QString &dir);
    static QString getCurFilePath(const QString &midFilePath);
    static QString getLyrFilePath(const QString &midFilePath);

    QString ncnPath() { return _ncnPath; }
    bool setNcnPath(const QString &dir);

    UpdateType updateType() { return upType; }
    void setUpdateType(UpdateType type);

public slots:
    bool insertNCN(const QString &ncnPath, const QString &songId, const QString &midFilePath);

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
    QSqlDatabase db;
    Song *song;
    SearchType searchType;

    QString searchText = "";

    UpdateType upType = UpdateType::UpdateAll;
    QString _ncnPath = "";

    int upCount = 0;
    bool upTing = false;

    void createIndex();
    void dropIndex();
};

#endif // SONGDATABASE_H
