#include "RtMidiUtils.h"

RtMidiUtils::RtMidiUtils()
{

}

vector<MidiPort> RtMidiUtils::getInputPorts()
{
    vector<MidiPort> ports;
    RtMidiIn *midiin = 0;

    try
    {
        midiin = new RtMidiIn();
    }
    catch (RtMidiError &error) {
        // Handle the exception here
        error.printMessage();
    }
    // Check inputs.
    unsigned int nPorts = midiin->getPortCount();
    std::string portName;
    for ( unsigned int i=0; i<nPorts; i++ ) {
        try {
            portName = midiin->getPortName(i);
        }
        catch ( RtMidiError &error ) {
            error.printMessage();
            goto cleanup;
        }

        MidiPort p;
        p.id = i+1;
        p.name = portName;
        ports.push_back(p);
    }

    // Clean up
    cleanup:
        delete midiin;

    return ports;
}

vector<MidiPort> RtMidiUtils::getOutputPorts()
{
    vector<MidiPort> ports;
    RtMidiOut *midiout = 0;

    // RtMidiOut constructor
    try {
        midiout = new RtMidiOut();
    }
    catch ( RtMidiError &error ) {
        error.printMessage();
        exit( EXIT_FAILURE );
    }

    // Check outputs.
    string portName;
    int nPorts = midiout->getPortCount();
    for ( unsigned int i=0; i<nPorts; i++ ) {
        try {
            portName = midiout->getPortName(i);
        }
        catch (RtMidiError &error) {
            error.printMessage();
            goto cleanup;
        }

        MidiPort p;
        p.id = i;
        p.name = portName;
        ports.push_back(p);
    }



    // Clean up
    cleanup:
        delete midiout;

    return ports;
}

std::string RtMidiUtils::listInputs()
{
    vector<MidiPort> out = getInputPorts();
    ostringstream oss;

    for(int i=0;i<out.size();i++)
    {
        oss << out[i].name << std::endl;
    }

    return oss.str();
}

std::string RtMidiUtils::listOutputs()
{
    vector<MidiPort> out = getOutputPorts();
    ostringstream oss;

    for(int i=0;i<out.size();i++)
    {
        oss << out[i].name << std::endl;
    }

    return oss.str();
}

MidiNote RtMidiUtils::noteToMidi(bool start, Note *n)
{
    MidiNote m;
    m.key = n->getKey();
    m.stamp = start ? n->getStart() : n->getStop();
    m.state = start;
    m.velocity = start ? n->getStartVelocity() : n->getStopVelocity();

    return m;
}

bool RtMidiUtils::isSharp(int key)
{
    int k = key%12;

    switch(k)
    {
    case 1:
    case 3:
    case 6:
    case 8:
    case 10:
        return true;
        break;
    default:
        return false;
        break;
    }
}

bool midiNoteComp(const MidiNote& a, const MidiNote& b)
{
    return a.stamp < b.stamp;
}
