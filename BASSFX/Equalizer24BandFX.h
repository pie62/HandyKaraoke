#ifndef EQUALIZER24BANDFX_H
#define EQUALIZER24BANDFX_H

#include <bass.h>

#include <map>

enum class EQFrequency24Range
{
    Frequency80Hz,      Frequency100Hz,     Frequency125Hz,
    Frequency160Hz,     Frequency200Hz,     Frequency250Hz,
    Frequency315Hz,     Frequency400Hz,     Frequency500Hz,
    Frequency630Hz,     Frequency800Hz,     Frequency1000Hz,
    Frequency1250Hz,    Frequency1600Hz,    Frequency2000Hz,
    Frequency2500Hz,    Frequency3150Hz,    Frequency4000Hz,
    Frequency5000Hz,    Frequency6300Hz,    Frequency8000Hz,
    Frequency10000Hz,   Frequency12500Hz,   Frequency16000Hz
};

class Equalizer24BandFX
{
private:
    HSTREAM streamHandle;
    std::map<EQFrequency24Range, HFX> fxEQ;
    std::map<EQFrequency24Range, float> fxGain;

    bool fxOn;
public:
    Equalizer24BandFX(HSTREAM streamHandle = 0);
    ~Equalizer24BandFX();

    bool isOn() { return fxOn; }

    void setStreamHandle(HSTREAM streamHandle);

    void on();
    void off();

    std::map<EQFrequency24Range, float> gain();
    float gain(EQFrequency24Range freq);
    void setGain(EQFrequency24Range freq, float gain);

    void resetGain();
};

#endif // EQUALIZER24BANDFX_H
