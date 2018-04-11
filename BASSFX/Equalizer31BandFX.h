#ifndef EQUALIZER31BANDFX_H
#define EQUALIZER31BANDFX_H

#include "FX.h"

#include <map>

enum class EQFrequency31Range
{
    Frequency20Hz,      Frequency25Hz,      Frequency31d5Hz,
    Frequency40Hz,      Frequency50Hz,      Frequency63Hz,
    Frequency80Hz,      Frequency100Hz,     Frequency125Hz,
    Frequency160Hz,     Frequency200Hz,     Frequency250Hz,
    Frequency315Hz,     Frequency400Hz,     Frequency500Hz,
    Frequency630Hz,     Frequency800Hz,     Frequency1000Hz,
    Frequency1250Hz,    Frequency1600Hz,    Frequency2000Hz,
    Frequency2500Hz,    Frequency3150Hz,    Frequency4000Hz,
    Frequency5000Hz,    Frequency6300Hz,    Frequency8000Hz,
    Frequency10000Hz,   Frequency12500Hz,   Frequency16000Hz,
    Frequency20000Hz
};

class Equalizer31BandFX : public FX
{
private:
    std::map<EQFrequency31Range, HFX> fxEQ;
    std::map<EQFrequency31Range, float> fxGain;

public:
    Equalizer31BandFX(DWORD stream = 0, int priority = 1);
    ~Equalizer31BandFX();

    void on();
    void off();

    std::map<EQFrequency31Range, float> gain();
    float gain(EQFrequency31Range freq);
    void setGain(EQFrequency31Range freq, float gain);

    void resetGain();

    QList<float> params();
    void setParams(const QList<float> &params);
    void setStreamHandle(DWORD stream);
    void setBypass(bool b);
    void reset();
};

#endif // EQUALIZER31BANDFX_H
