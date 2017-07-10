#ifndef CHANNEL_H
#define CHANNEL_H


class Channel
{
    int cPort;
    int cNumber;
    int cVolume;
    int cPan;
    int cReverb;
    int cChorus;
    int cInstrument;
    bool cMute;
    bool cSolo;

public:
    Channel();
    Channel(int number, int outPort);

    int port()      { return cPort; }
    int number()    { return cNumber; }
    int volume()    { return cVolume; }
    int pan()       { return cPan; }
    int reverb()    { return cReverb; }
    int chorus()    { return cChorus; }
    int instrument(){ return cInstrument; }
    bool isMute()   { return cMute; }
    bool isSolo()   { return cSolo; }

    void setPort(int port)  { cPort = port; }
    void setNumber(int n)   { cNumber = n; }
    void setVolume(int v)   { cVolume = v; }
    void setPan(int p)      { cPan = p; }
    void setReverb(int r)   { cReverb = r; }
    void setChorus(int c)   { cChorus = c; }
    void setInstrument(int i) { cInstrument = i; }
    void setMute(bool m)    { cMute = m; }
    void setSolo(bool s)    { cSolo = s; }
};

#endif // CHANNEL_H
