#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QKeyEvent>
#include <QLocale>
#include <QSettings>
#include <QTranslator>

#ifdef _WIN32
#include <QWinTaskbarButton>
#include <QWinTaskbarProgress>
#include <winsparkle.h>
#endif

#include <Background.h>
#include <LyricsWidget.h>
#include <Detail.h>
#include <ChannelMixer.h>

#include "SongDatabase.h"

#include "Midi/MidiPlayer.h"

#include "Dialogs/SynthMixerDialog.h"
#include "Dialogs/SecondMonitorDialog.h"


class MedleyLoader;


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    SongDatabase* database() { return db; }
    MidiPlayer* midiPlayer() { return player; }
    Background *backgroundWidget() { return bgWidget; }
    LyricsWidget* lyricsWidget() { return lyrWidget; }
    LyricsWidget* secondLyrics() { return secondLyr; }
    SecondMonitorDialog* secondMonitorDlg() { return secondMonitor; }

    bool removeFromPlaylist() { return remove_playlist; }
    bool autoPlayNext() { return auto_playnext; }
    int searchTimeout() { return search_timeout / 1000; }
    int playlistTimout() { return playlist_timeout / 1000; }
    void setRemoveFromPlaylist(bool r) { remove_playlist = r; }
    void setAutoPlayNext(bool p) { auto_playnext = p; }
    void setSearchTimeout(int s) { search_timeout = s*1000; }
    void setPlaylistTimeout(int s) { playlist_timeout = s*1000; }

    // Synth Mixer
    SynthMixerDialog* synthMixerDialog() { return synthMix; }

public slots:
    void play(int index, int position = 0);
    void pause();
    void resume();
    void stop();
    void playNext();
    void playPrevious();

signals:
    void resized(const QSize &s);

protected:
    void showEvent(QShowEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void resizeEvent(QResizeEvent *event);
    void closeEvent(QCloseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

private:
    void loadNextMedley(Song *song);
    void addToPlaylist(Song *song);
    void removeFromPlaylist(int index);
    void swapInPlaylist(int index, int toIndex);
    static void updateShutdownRequest();

private slots:
    void showFrameSearch();
    void showPlaylist();
    void showSongDetail();
    void showChMix();
    void onChMixMouseLeaved();
    void onChMixLockChanged(bool lock);
    void hideUIFrame();

    void showCurrentTime();
    void setFrameSearch(Song* s);

    void showContextMenu(const QPoint &pos);
    void showSettingsDialog();
    void showEqDialog();
    void showChorusDialog();
    void showReverbDialog();
    void showMapMidiChannelDialog();
    void minimizeWindow();
    void showMapSFDialog();
    void showSecondMonitor();
    void showFullScreenOrNormal();
    void setThaiLang();
    void setEngLang();
    void showCheckUpdateDialog();
    void showAboutDialog();
    void showAboutQtDialog();

    void showBusGroupDialog();
    void showSpeakerDialog();
    void showVSTDirDialog();

    void onPositiomTimerTimeOut();
    void onLyricsTimerTimeOut();
    void onPlayerDurationMSChanged(qint64 d);
    void onPlayerPositionMSChanged(qint64 p);
    void onPlayerDurationTickChanged(int d);

    void onSliderPositionPressed();
    void onSliderPositionReleased();

    void on_btnVolumeMute_clicked();
    void on_btnPlay_clicked();
    void onSliderVolumeValueChanged(int value);

    void switchMedleyLyrics();
    void switchMedleyLyrics2();
    void onNextMedleyStarted();
    void onNextMedleyAfterStarted();
    void onPlayerThreadFinished();

    void onDbUpdateChanged(int v);
    void onDetailTimerTimeout();

    void addBpmSpeed(int speed);
    void preSetBpmSpeed(int speed);
    void preSetTranspose(int transpose);

    void sendDrumPads(QKeyEvent *key, bool noteOn);

    void savePlaylist();
    void loadPlaylist();

private:
    Ui::MainWindow *ui;
    QSettings *settings;
    SongDatabase *db;
    QTimer *timer1, *timer2, *positionTimer, *lyricsTimer;
    QTimer *detailTimer;

    QList<Song*> playlist;
    MidiPlayer *player;
    Song playingSong;
    int playingIndex = -1;
    bool playAfterSeek = false;
    bool searchBoxChangeBpm = false;

    bool nextMedleyRequested = false;

    MedleyLoader *medleyLoader = nullptr;

    Background *bgWidget = nullptr;
    LyricsWidget *lyrWidget, *secondLyr = nullptr;
    Detail *updateDetail;

//    int bgType = 0;
//    QString bgImg = "", bgColor = "#525252";
    bool remove_playlist = true;
    bool auto_playnext = true;
    int search_timeout = 5000;
    int playlist_timeout = 5000;
    int songDetail_timeout = 4000;

    QLocale locale;

    SecondMonitorDialog *secondMonitor = nullptr;

    // Synth Mixer
    SynthMixerDialog *synthMix;

    #ifdef _WIN32
    QWinTaskbarButton *taskbarButton;
    #endif

    bool firstShow = true;

    QString currentLang = "th";
    QTranslator translator;

};

#endif // MAINWINDOW_H
