#ifndef MIDIOUT_H
#define MIDIOUT_H

#include <RtMidi.h>

class MidiOut : public RtMidiOut
{
public:
    MidiOut();
    ~MidiOut();

    float volume() { return oVolume; }
    void setVolume(float vol);
    void sendNoteOff(int ch, int note, int velocity);
    void sendNoteOn(int ch, int note, int velocity);
    void sendNoteAftertouch(int ch, int note, int value);
    void sendController(int ch, int number, int value);
    void sendProgramChange(int ch, int number);
    void sendChannelAftertouch(int ch, int value);
    void sendPitchBend(int ch, int value);
    void sendAllNotesOff(int ch);
    void sendAllNotesOff();
    void sendAllSoundOff(int ch);
    void sendAllSoundOff();
    void sendResetAllControllers(int ch);
    void sendResetAllControllers();

private:
    float oVolume;
    std::vector<unsigned char> message;
};

#endif // MIDIOUT_H
