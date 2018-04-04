#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "SongDatabase.h"
#include "Midi/MidiPlayer.h"

#include <LyricsWidget.h>
#include <Detail.h>
#include <ChannelMixer.h>

#include "Dialogs/SynthMixerDialog.h"
#include "Dialogs/Equalizer31BandDialog.h"
#include "Dialogs/ReverbDialog.h"
#include "Dialogs/ChorusDialog.h"
#include "Dialogs/SecondMonitorDialog.h"

#include <QMainWindow>
#include <QTimer>
#include <QKeyEvent>
#include <QLocale>
#include <QSettings>

#ifdef _WIN32
#include <QWinTaskbarButton>
#include <QWinTaskbarProgress>
#endif


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
    void setBackgroundColor(const QString &colorName);
    void setBackgroundImage(const QString &img);

    // Synth Mixer
    SynthMixerDialog* synthMixerDialog() { return synthMix; }
    // Synth effect dialog
    Equalizer31BandDialog* equalizer31BandDialog() { return eq31Dlg; }
    ReverbDialog* reverbDialog() { return reverbDlg; }
    ChorusDialog* chorusDialog() { return chorusDlg; }

public slots:
    void play(int index);
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
    bool changingBpmSpeed = false;

    LyricsWidget *lyrWidget, *secondLyr = nullptr;
    Detail *updateDetail;

    int bgType = 0;
    QString bgImg = "", bgColor = "#525252";
    bool remove_playlist = true;
    bool auto_playnext = true;
    int search_timeout = 5000;
    int playlist_timeout = 5000;
    int songDetail_timeout = 5000;

    QLocale locale;

    SecondMonitorDialog *secondMonitor = nullptr;

    // Synth Mixer
    SynthMixerDialog *synthMix;
    // Synth effect dialog
    Equalizer31BandDialog *eq31Dlg;
    ReverbDialog *reverbDlg;
    ChorusDialog *chorusDlg;

    #ifdef _WIN32
    QWinTaskbarButton *taskbarButton;
    #endif

private slots:
    void showFrameSearch();
    void showFramePlaylist();
    void showSongDetail();
    void showChMix();
    void onChMixMouseLeaved();
    void onChMixLockChanged(bool lock);
    void hideUIFrame();

    void showCurrentTime();
    void setFrameSearch(Song* s);

    void showContextMenu(const QPoint &pos);
    void showSettingsDialog();
    void minimizeWindow();
    void showMapSFDialog();
    void showSecondMonitor();
    void showFullScreenOrNormal();
    void showAboutDialog();
    void showAboutQtDialog();

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

    void onPlayerThreadFinished();

    void onDbUpdateChanged(int v);
    void onDetailTimerTimeout();

    void addBpmSpeed(int speed);

    void sendDrumPads(QKeyEvent *key, bool noteOn);
};

#endif // MAINWINDOW_H
