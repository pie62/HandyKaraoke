#include "Channel.h"

Channel::Channel()
{
    cPort = 0;
    cNumber = 0;
    cVolume = 127;
    cPan = 64;
    cReverb = 0;
    cChorus = 0;
    cInstrument = 0;
    cMute = false;
    cSolo = false;
}

Channel::Channel(int number, int outPort)
{
    cPort = outPort;
    cNumber = number;
    cVolume = 127;
    cPan = 64;
    cReverb = 0;
    cChorus = 0;
    cInstrument = 0;
    cMute = false;
    cSolo = false;
}
