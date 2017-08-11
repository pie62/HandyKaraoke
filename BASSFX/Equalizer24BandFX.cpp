#include "Equalizer24BandFX.h"

Equalizer24BandFX::Equalizer24BandFX(HSTREAM streamHandle)
{
    this->streamHandle = streamHandle;

    fxOn = false;

    fxGain[EQFrequency24Range::Frequency80Hz]    = 0;
    fxGain[EQFrequency24Range::Frequency100Hz]   = 0;
    fxGain[EQFrequency24Range::Frequency125Hz]   = 0;
    fxGain[EQFrequency24Range::Frequency160Hz]   = 0;
    fxGain[EQFrequency24Range::Frequency200Hz]   = 0;
    fxGain[EQFrequency24Range::Frequency250Hz]   = 0;
    fxGain[EQFrequency24Range::Frequency315Hz]   = 0;
    fxGain[EQFrequency24Range::Frequency400Hz]   = 0;
    fxGain[EQFrequency24Range::Frequency500Hz]   = 0;
    fxGain[EQFrequency24Range::Frequency630Hz]   = 0;
    fxGain[EQFrequency24Range::Frequency800Hz]   = 0;
    fxGain[EQFrequency24Range::Frequency1000Hz]  = 0;
    fxGain[EQFrequency24Range::Frequency1250Hz]  = 0;
    fxGain[EQFrequency24Range::Frequency1600Hz]  = 0;
    fxGain[EQFrequency24Range::Frequency2000Hz]  = 0;
    fxGain[EQFrequency24Range::Frequency2500Hz]  = 0;
    fxGain[EQFrequency24Range::Frequency3150Hz]  = 0;
    fxGain[EQFrequency24Range::Frequency4000Hz]  = 0;
    fxGain[EQFrequency24Range::Frequency5000Hz]  = 0;
    fxGain[EQFrequency24Range::Frequency6300Hz]  = 0;
    fxGain[EQFrequency24Range::Frequency8000Hz]  = 0;
    fxGain[EQFrequency24Range::Frequency10000Hz] = 0;
    fxGain[EQFrequency24Range::Frequency12500Hz] = 0;
    fxGain[EQFrequency24Range::Frequency16000Hz] = 0;
}

Equalizer24BandFX::~Equalizer24BandFX()
{
    if (fxOn)
        off();

    fxGain.clear();
    fxEQ.clear();
}

void Equalizer24BandFX::setStreamHandle(HSTREAM streamHandle)
{
    if (fxOn)
    {
        off();
        this->streamHandle = streamHandle;
        on();
    }
    else
    {
        this->streamHandle = streamHandle;
    }
}

void Equalizer24BandFX::on()
{
    if (fxOn)
        return;

    fxOn = true;

    if (streamHandle == 0)
        return;

    // -------------------------

    BASS_DX8_PARAMEQ eq;
    eq.fBandwidth = 18;

    float eqfreq[24] = { 80, 100, 125, 160,
                         200, 250, 315, 400, 500, 630, 800, 1000, 1250, 1600,
                         2000, 2500, 3150, 4000, 5000, 6300, 8000, 10000, 12500, 16000 };
    fxEQ.clear();
    int i = 0;
    for (auto const& x : fxGain)
    {
        eq.fCenter = eqfreq[i];
        eq.fGain = x.second;

        HFX fx = BASS_ChannelSetFX(streamHandle, BASS_FX_DX8_PARAMEQ, 1);

        BASS_FXSetParameters(fx, &eq);

        fxEQ[x.first] = fx;

        i++;
    }
}

void Equalizer24BandFX::off()
{
    if (!fxOn)
        return;

    fxOn = false;

    if (streamHandle == 0)
        return;

    //========================

    for (auto const& x : fxEQ)
    {
        BASS_ChannelRemoveFX(streamHandle, x.second);
    }

    fxEQ.clear();
}

std::map<EQFrequency24Range, float> Equalizer24BandFX::gain()
{
    return fxGain;
}

float Equalizer24BandFX::gain(EQFrequency24Range freq)
{
    return fxGain[freq];
}

void Equalizer24BandFX::setGain(EQFrequency24Range freq, float gain)
{
    float g = 0;
    if (gain > 15)
        g = 15;
    else if (gain < -15)
        g = -15;
    else
        g = gain;

    fxGain[freq] = g;

    if (!fxOn || streamHandle == 0)
        return;

    BASS_DX8_PARAMEQ eq;
    if (BASS_FXGetParameters(fxEQ[freq], &eq))
    {
        eq.fGain = g;
        BASS_FXSetParameters(fxEQ[freq], &eq);
    }
}

void Equalizer24BandFX::resetGain()
{
    std::map<EQFrequency24Range, float> fg;
    fg = fxGain;

    for (auto const& x : fg)
    {
        setGain(x.first, 0);
    }

    fg.clear();
}

