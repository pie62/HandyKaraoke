#ifndef Equalizer15BandFX_H
#define Equalizer15BandFX_H

#include <bass.h>

#include <map>

enum class EQFrequency15Range
{
    Frequency25Hz,
    Frequency40Hz,
    Frequency63Hz,
    Frequency100Hz,
    Frequency160Hz,
    Frequency250Hz,
    Frequency400Hz,
    Frequency630Hz,
    Frequency1000Hz,
    Frequency1600Hz,
    Frequency2500Hz,
    Frequency4000Hz,
    Frequency6300Hz,
    Frequency10000Hz,
    Frequency16000Hz
};

class Equalizer15BandFX
{
private:
    HSTREAM streamHandle;
    std::map<EQFrequency15Range, HFX> fxEQ;
    std::map<EQFrequency15Range, float> fxGain;

    bool fxOn;

public:
    Equalizer15BandFX(HSTREAM streamHandle = 0);
    ~Equalizer15BandFX();

    bool isOn() { return fxOn; }

    void setStreamHandle(HSTREAM streamHandle);

    void on();
    void off();

    std::map<EQFrequency15Range, float> gain();
    float gain(EQFrequency15Range freq);
    void setGain(EQFrequency15Range freq, float gain);

    void resetGain();
};

#endif // Equalizer15BandFX_H
