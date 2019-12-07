#include "MedleyLoader.h"

#include "Config.h"

#include "Utils.h"
#include "Song.h"
#include "SongDatabase.h"
#include "Midi/MidiPlayer.h"
#include "Midi/HNKFile.h"
#include "Widgets/LyricsWidget.h"


MedleyLoader::MedleyLoader(QObject *parent,
                           Song *song,
                           SongDatabase *songDB,
                           MidiPlayer *player,
                           LyricsWidget *lyrWidget,
                           LyricsWidget *lyrWidget2)
    : QThread(parent)
{
    _song = song;
    _songDb = songDB;
    _player = player;
    _lyrWidget = lyrWidget;
    _lyrWidget2 = lyrWidget2;
}

void MedleyLoader::run()
{
    if (_song->songType() == "NCN")
    {
        QString midPath = _songDb->ncnPath() + _song->path();
        _player->loadNextMedley(midPath, 4, 12, _song->bpmSpeed(), _song->transpose());

        QString curPath = _songDb->getCurFilePath(midPath);
        if (curPath == "" || !QFile::exists(curPath)) {
            return;
        }

        QString lyrPath = _songDb->getLyrFilePath(midPath);
        if (lyrPath == "" || !QFile::exists(lyrPath)) {
            return;
        }

        _lyrWidget->setLyricsTemp(Utils::readLyrics(lyrPath),
            Utils::readCurFile(curPath, _player->midiFileTemp()->resorution()));
    }
    else if (_song->songType() == "HNK")
    {
        QString hnkPath = _songDb->hnkPath() + _song->path();

        QFile mid(TEMP_MIDI_DIR_PATH);
        if (mid.exists())
            mid.remove();

        mid.open(QFile::ReadWrite);
        mid.write(HNKFile::midData(hnkPath));
        mid.close();

        _player->loadNextMedley(TEMP_MIDI_DIR_PATH, 4, 12, _song->bpmSpeed(), _song->transpose());

        mid.remove();

        _lyrWidget->setLyricsTemp(Utils::readLyrics(HNKFile::lyrData(hnkPath)),
            Utils::readCurFile(HNKFile::curData(hnkPath), _player->midiFileTemp()->resorution()));
    }
    else if (_song->songType() == "KAR")
    {
        QString karPath = _songDb->karPath() + _song->path();

        _player->loadNextMedley(karPath, 4, 12, _song->bpmSpeed(), _song->transpose());

        _lyrWidget->setLyricsTemp(_player->midiFileTemp()->lyrics(), _player->midiFileTemp()->lyricsCursor());
    }  else {
        return;
    }

    if (_lyrWidget2 != nullptr) {
        _lyrWidget2->setLyricsTemp(_lyrWidget->lyrTempData(), _lyrWidget->curTempData());
    }
}
