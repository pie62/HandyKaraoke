#ifndef MIDIPLAYER_H
#define MIDIPLAYER_H

#include "MidiOut.h"
#include "Channel.h"
#include "MidiSequencer.h"
#include "MidiSynthesizer.h"

#include <QObject>

class MidiPlayer : public QObject
{
    Q_OBJECT
public:
    explicit MidiPlayer(QObject *parent = nullptr);
    ~MidiPlayer();

    static std::vector<std::string> midiDevices();
    static bool isSnareNumber(int num);
    static bool isBassInstrument(int ints);
    static int getNumberBeatInBar(int numerator, int denominator);

    MidiSynthesizer* midiSynthesizer() { return _midiSynth; }
    Channel* midiChannel() { return _midiChannels; }
    int midiOutPortNumber() { return _midiPortNum; }
    int volume() { return _volume; }
    int transpose() { return _midiTranspose; }

    int  lockDrumNumber()  { return _lockDrumNumber; }
    int  lockSnareNumber() { return _lockSnareNumber; }
    int  lockBassNumber()  { return _lockBassBumber; }
    bool isLockDrum()   { return _lockDrum; }
    bool isLockSnare()  { return _lockSnare; }
    bool isLockBass()   { return _lockBass; }

    long durationMs();
    long positionMs();
    int durationTick();
    int positionTick();
    int bpmSpeed();
    int currentBpm();
    int currentBeat();
    int beatCount();

public slots:
    void sendEvent(MidiEvent *e);

signals:
    void loaded();
    void sendedEvent(MidiEvent *e);
    void bpmChanged(int bpm);

private:
    std::vector<MidiSequencer*> _midiSeq;
    QMap<int, MidiOut*> _midiOuts;
    MidiSynthesizer     *_midiSynth;
    Channel             _midiChannels[16];
    int                 _midiPortNum = 0;
    int                 _volume = 100;
    int                 _midiTranspose = 0;
    int                 _seqIndex = 0;
    bool                _useMedley = false;
    bool                _useSolo = false;

    MidiEvent   _tempEvent;
    MidiEvent*  _playingEventPtr = nullptr;

    bool    _lockDrum  = false;
    bool    _lockSnare = false;
    bool    _lockBass  = false;
    int     _lockDrumNumber  = 0;
    int     _lockSnareNumber = 38;
    int     _lockBassBumber  = 32;


    void sendEventToDevices(MidiEvent *e);
    void sendAllNotesOff(int ch);
    void sendAllNotesOff();
    void sendResetAllControllers(int ch);
    void sendResetAllControllers();

    int getNoteNumberToPlay(int ch, int defaultNote);
};

#endif // MIDIPLAYER_H
