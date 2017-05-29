#ifndef MIDIPLAYER_H
#define MIDIPLAYER_H

#include "MidiFile.h"
#include "MidiOut.h"
#include "Channel.h"
#include "MidiSynthesizer.h"

#include <QObject>
#include <QThread>
#include <QElapsedTimer>


class MidiPlayer : public QObject
{
    Q_OBJECT
public:
    explicit MidiPlayer(QObject *parent = 0);
    ~MidiPlayer();

    QStringList midiDevices();
    MidiSynthesizer* midiSynthesizer() { return synth; }
    int midiOut() { return defaultPort; }
    bool isPlaying() { return playing; }
    bool isStoped() { return stoped; }
    bool isPaused() { return (!playing && !stoped) ? true : false; }
    uint32_t resolution() { return midi->resorution(); }
    int volume() { return midi_volume; }
    int duration() { return midi_duration; }  // Tick
    qint64 durationMS() { return midi_duration_ms; }
    int position();  // Tick
    qint64 positionMS();

signals:
    void finished();
    void playingEvent(MidiEvent *e);

public slots:
    bool setMidiOut(int portNumer);  // Default port, use "-1" for soundfont
    bool load(QString file);
    //void setSoundFonts(QStringList files);
    //void unsetSoundFonts();
    void play();
    void pause();
    void resume();
    void stop();
    void setVolume(int v);
    void setPosition(int tick);

private:
    MidiSynthesizer *synth;
    QList<MidiOut*> midi_mapper;
    MidiFile *midi;
    int defaultPort = 0;
    QList<Channel*> channels;

    int midi_bpm = 120;
    int midi_speed = 0;
    int midi_volume = 100;
    int midi_position = 0; // tick
    int midi_duration = 0;
    qint64 midi_position_ms = 0;
    qint64 midi_duration_ms = 0;

    long startEventTime = 0;
    int playedIndex = 0;
    bool useSolo = false;
    bool playing = false;
    bool stoped = true;
    bool playAfterSeek = false;

    QThread *thread;
    QElapsedTimer *eTimer;

private slots:
    void playEvents();
    void sendEvent(MidiEvent *e);
    void sendAllNotesOff(int ch);
    void sendAllNotesOff();
    void resetAllControllers();
    float timeFromTick(uint32_t tick);
    qint32 tickFromTime(float msTime);
};

#endif // MIDIPLAYER_H
