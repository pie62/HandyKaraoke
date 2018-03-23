#ifndef MIDISENDER_H
#define MIDISENDER_H

#include "MidiOut.h"
#include "Channel.h"
#include "MidiSynthesizer.h"

#include <QObject>
#include <QMap>

class MidiSender : public QObject
{
    Q_OBJECT
public:
    explicit MidiSender(QObject *parent = nullptr);
    ~MidiSender();

public slots:

signals:

private:
    MidiSynthesizer     *_midiSynth;
    QMap<int, MidiOut*> _midiOuts;
    Channel             _midiChannels[16];

    int     _midiPortNum = 0;
    int     _volume = 100;

    bool    _useSolo = false;

    bool    _lockDrum  = false;
    bool    _lockSnare = false;
    bool    _lockBass  = false;
    int     _lockDrumNumber  = 0;
    int     _lockSnareNumber = 38;
    int     _lockBassBumber  = 32;
};

#endif // MIDISENDER_H
