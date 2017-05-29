#ifndef MIDIPLAYER_H
#define MIDIPLAYER_H

#include "QMidi/QMidiOut.h"
#include <bass.h>
#include <bassmidi.h>
#include <QObject>
#include <QTimer>
#include <QDebug>

class MidiPlayer : public QObject
{
    Q_OBJECT
public:
    explicit MidiPlayer(QObject *parent = 0);
    ~MidiPlayer();

    QMap<QString, QString> midiDevices() { return pMidiDevices; }
    QString midiOut() { return midi_out; }
    bool isPlaying() { return BASS_ChannelIsActive(midi) == BASS_ACTIVE_PLAYING ? true : false; }
    bool isPaused() { return BASS_ChannelIsActive(midi) == BASS_ACTIVE_PAUSED ? true : false; }
    bool isStoped() { return BASS_ChannelIsActive(midi) == BASS_ACTIVE_STOPPED ? true : false; }
    uint32_t resolution();
    int volume() { return midi_volume; }
    int duration() { return midi_duration; }
    int position() { return midi_position; }


    // For bass callback
    void midiFilter(BASS_MIDI_EVENT *event);
    void emitFinished() { emit finished(); }
    //void emitPositionChanged(int p) { emit positionChanged(p); midi_position = p; }
    //void emitPositionMSChanged(qint64 p) { emit positionMSChanged(p); }

signals:
    void durationChanged(int d);
    void positionChanged(int p);
    void durationMSChanged(qint64 d);
    void positionMSChanged(qint64 p);
    void finished();

public slots:
    void setMidiOut(QString dv);  // Default port, use "SoundFont" for soundfont
    bool load(QString file);
    void setSoundFonts(QStringList files);
    void unsetSoundFonts();
    void play();
    void pause();
    void resume();
    void stop();
    void setVolume(int v);
    void setPosition(int tick);

private:
    QTimer *timer;
    QMap<QString, QString> pMidiDevices;
    //QMidiOut *midi_device;
    HSTREAM midi;
    QList<HSOUNDFONT> fonts;
    QString midi_out;
    int midi_volume;
    int midi_duration;
    int midi_position;


    // BASS callback
    static void CALLBACK endSync(HSYNC handle, DWORD channel, DWORD data, void *user);
    static BOOL CALLBACK midiFilterProc(HSTREAM handle, DWORD track, BASS_MIDI_EVENT *event, BOOL seeking, void *user);


private slots:
    void onTimerTimeOut();

};

#endif // MIDIPLAYER_H
