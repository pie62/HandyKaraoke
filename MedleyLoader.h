#ifndef MEDLEYLOADER_H
#define MEDLEYLOADER_H

#include <QThread>

class Song;
class SongDatabase;
class MidiPlayer;
class LyricsWidget;

class MedleyLoader : public QThread
{
    Q_OBJECT
public:
    explicit MedleyLoader(QObject *parent = nullptr,
                          Song *song = nullptr,
                          SongDatabase *songDB = nullptr,
                          MidiPlayer *player = nullptr,
                          LyricsWidget *lyrWidget = nullptr,
                          LyricsWidget *lyrWidget2 = nullptr);

protected:
    void run();

private:
    Song *_song;
    SongDatabase *_songDb;
    MidiPlayer *_player;
    LyricsWidget *_lyrWidget, *_lyrWidget2;
};

#endif // MEDLEYLOADER_H
