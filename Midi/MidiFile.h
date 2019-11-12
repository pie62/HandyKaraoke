#ifndef MIDIFILE_H
#define MIDIFILE_H

#include "MidiEvent.h"

#include <QString>
#include <QVector>
#include <QFile>

class MidiFile
{
public:
    enum DivisionType {
        Invalid = -1,
        PPQ = 0,
        SMPTE24 = -24,
        SMPTE25 = -25,
        SMPTE30DROP = -29,
        SMPTE30 = -30
    };

    MidiFile();
    ~MidiFile();

    void clear();
    bool read(const QString &file, bool seekFileChunkID = false);
    bool read(QFile *in, bool seekFileChunkID = false);

    int formatType() { return fFormatType; }
    int numberOfTracks() { return fNumOfTracks; }
    int resorution() { return fResolution; }
    DivisionType divisionType() { return fDivision; }

    QString lyrics() { return fLyrics; }
    QVector<long> lyricsCursor() { return fLyricscursor; }

    QVector<MidiEvent*> events() { return fEvents; }
    QVector<MidiEvent*> tempoEvents() { return fTempoEvents; }
    QVector<MidiEvent*> lyricsEvents() { return fLyricsEvents; }
    QVector<MidiEvent*> controllerEvents() { return fControllerEvents; }
    QVector<MidiEvent*> programChangeEvents() { return fProgramChangeEvents; }
    QVector<MidiEvent*> timeSignatureEvents() { return fTimeSignatureEvents; }
    QVector<MidiEvent*> controllerAndProgramEvents();

    MidiEvent* createMidiEvent(int track, uint32_t tick, uint32_t delta, MidiEventType evType, int ch, int data1, int data2);
    MidiEvent* createMetaEvent(int track, uint32_t tick, uint32_t delta, int number, QByteArray data);
    MidiEvent* createSysExEvent(int track, uint32_t tick, uint32_t delta, QByteArray data);

    float    beatFromTick(uint32_t tick);
    float    timeFromTick(uint32_t tick, int bpmSpeed = 0);
    uint32_t tickFromTime(float time, int bpmSpeed = 0);
    uint32_t tickFromTimeMs(long msTime, int bpmSpeed = 0);
    uint32_t tickFromBeat(float beat);
    uint32_t tickFromBar(int barNumber);

    int barCount();

    static int firstBpm(const QString &file);
    static int firstBpm(QFile *in);

private:
    int fFormatType;
    int fNumOfTracks;
    int fResolution;
    DivisionType fDivision;

    QString fLyrics;
    QVector<long> fLyricscursor;

    QVector<MidiEvent*> fEvents;
    QVector<MidiEvent*> fTempoEvents;
    QVector<MidiEvent*> fLyricsEvents;
    QVector<MidiEvent*> fControllerEvents;
    QVector<MidiEvent*> fProgramChangeEvents;
    QVector<MidiEvent*> fTimeSignatureEvents;
};

#endif // MIDIFILE_H
