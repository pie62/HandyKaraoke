#ifndef SONGDATABASE_H
#define SONGDATABASE_H

#include "Song.h"

#include <QObject>
#include <QSqlDatabase>
#include <QThread>

class SongDatabase : public QObject
{
    Q_OBJECT
public:
    enum SearchType {
        ById,
        ByName,
        ByArtist
    };

    explicit SongDatabase(QObject *parent = 0);
    ~SongDatabase();

    int count() { return dCount; }
    Song* currentSong() { return song; }
    static bool isNCNPath(QString path);

    bool isOpenned() { return db.isOpen(); }
    bool isUpdatting() { return upTing; }
    int updateCount() { return upCount; }

signals:
    void updateStarted();
    void updateCountChanged(int c);
    void updatePositionChanged(int p);
    void updateSongNameChanged(QString n);
    void updateFinished();

public slots:
    void update();
    void updateInThread() { thread->start(); }
    bool insertNCN(const QString &ncnPath, const QString &songId, const QString &lyrFilePath, const QString &fileName);

    void setSearchType(SearchType t) { searchType = t; }
    Song *nextType(const QString &s);
    Song* search(const QString &s);
    Song* searchNext();
    Song* searchPrevious();

private:
    QSqlDatabase db;
    QThread *thread;
    Song *song;
    SearchType searchType;
    int dCount;

    int upCount = 0;
    bool upTing = false;
};

#endif // SONGDATABASE_H
