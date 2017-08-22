#ifndef RTMIDIUTILS_H
#define RTMIDIUTILS_H

#include <iostream>
#include <vector>
#include <sstream>

#include "RtMidi.h"
#include "model/music/note.h"

using namespace std;

struct MidiNote{
    bool state;
    int key;
    int velocity;
    int stamp;
    int delta;
};

struct MidiPort{
    int id;
    string name;
};

bool midiNoteComp(const MidiNote& a, const MidiNote& b);

class RtMidiUtils
{
public:
    RtMidiUtils();
    static vector<MidiPort> getOutputPorts();
    static vector<MidiPort> getInputPorts();
    static std::string listInputs();
    static std::string listOutputs();
    static MidiNote noteToMidi(bool start, Note *n);
    static bool isSharp(int key);

private:
};

#endif // RTMIDIUTILS_H
