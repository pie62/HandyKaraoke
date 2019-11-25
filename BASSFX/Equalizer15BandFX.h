#ifndef Equalizer15BandFX_H
#define Equalizer15BandFX_H

#include "FX.h"
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

class Equalizer15BandFX : public FX
{
private:
    HFX fx = 0;
    std::map<EQFrequency15Range, float> fxGain;

public:
    Equalizer15BandFX(DWORD stream = 0, int priority = 1);
    ~Equalizer15BandFX();

    void on();
    void off();

    std::map<EQFrequency15Range, float> gain();
    float gain(EQFrequency15Range freq);
    void setGain(EQFrequency15Range freq, float gain);

    void resetGain();

    QList<float> params();
    void setParams(const QList<float> &params);
    void setStreamHandle(DWORD stream);
    void setBypass(bool b);
    void reset();
};

#endif // Equalizer15BandFX_H
