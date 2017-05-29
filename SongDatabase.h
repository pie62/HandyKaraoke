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

signals:
    void updateStarted();
    void updateLengthChanged(int l);
    void updatePositionChanged(int p);
    void updateSongNameChanged(QString n);
    void updateFinished();

public slots:
    void update();
    void updateInThread() { thread->start(); }
    void setSearchType(SearchType t) { searchType = t; }
    Song *nextType(QString s);
    Song* search(QString s);
    Song* searchNext();
    Song* searchPrevious();

private:
    QSqlDatabase db;
    QThread *thread;
    Song *song;
    SearchType searchType;
    int dCount;
};

#endif // SONGDATABASE_H
