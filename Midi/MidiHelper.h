#ifndef MIDIHELPER_H
#define MIDIHELPER_H

#include <QObject>

class MidiHelper
{
public:
    MidiHelper();

    static QStringList GMInstrumentNumberNames();
    static QStringList drumKitNumberNames();
    static QStringList snareNumberName();

    static QStringList GMInstrumentNames();
    static QStringList drumKitNames();
    static QStringList snareName();

    static QStringList instrumentGroupNames();
};

#endif // MIDIHELPER_H
