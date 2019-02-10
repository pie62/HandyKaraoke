#ifndef MIDISEQUENCER2_H
#define MIDISEQUENCER2_H

#include <QObject>
#include <QTimer>

#include "MidiFile2.h"

class MidiSequencer2 : public QObject
{
    Q_OBJECT
public:
    explicit MidiSequencer2(QObject *parent = nullptr);
    ~MidiSequencer2();

    MidiFile2* midiFile() { return _midi; }

    bool isSeqFinished() { return _finished; }
    bool isSeqPlaying() { return _playing; }
    bool isSeqStopped() { return _stopped; }
    bool isSeqPaused() { return (!_playing && !_stopped) ? true : false; }

    int bpmSpeed() { return _midiSpeed; }
    int currentBpm() { return _midiBpm + _midiSpeed; }

    int beatCount();
    int currentBeat();
    int positionTick();
    int durationTick();
    long positionMs();
    long durationMs();

    void setPositionTick(int t);
    void setBpmSpeed(int sp);

    bool load(const QString &file, bool seekFileChunkID = false);

public slots:
    void start();
    void stop(bool resetPos = false);

signals:
    void bpmChanged(int bpm);
    void playingEvent(MidiEvent *e);

private slots:
    void onTimerTick();

private:
    float calcIntervalTime();

private:
    MidiFile2 *_midi;
    QTimer *_timer;

    QList<int> _playedIndexs;

    int     _midiBpm = 120;
    int     _midiSpeed = 0;

    int     _positionTick = 0;

    bool    _finished = false;
    bool    _stopped = true;
    bool    _playing = false;
};

#endif // MIDISEQUENCER2_H
