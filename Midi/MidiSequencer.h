#ifndef MIDISEQUENCER_H
#define MIDISEQUENCER_H

#include <QThread>
#include <QElapsedTimer>

#include "MidiFile.h"


class MidiSequencer : public QThread
{
    Q_OBJECT
public:
    explicit MidiSequencer(QObject *parent = nullptr);
    ~MidiSequencer();

    void setMidi(MidiFile *midi);
    void stop(bool resetPos = false);

public slots:

signals:
    void playingEvent(MidiEvent *e);

protected:
    void run();

private:
    MidiFile *_midi;
    QElapsedTimer *_eTimer;

    bool    _stopped = true;
    bool    _playing = false;
    bool    _changeProgramDrum = false;
    bool    _midiChangeBpmSpeed = false;

    int     _midiSpeed = 0;
    int     _midiSpeedTemp = 0;
    int     _playedIndex = 0;

    long    _startPlayTime = 0;
    long    _startPlayIndex = 0;
    long    _positionTick = 0;
    long    _positionMs = 0;

    int     _lockBpmNumber = 120;
    bool    _lockBpm = true;
};

#endif // MIDISEQUENCER_H
