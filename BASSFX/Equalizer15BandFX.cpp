#include "Equalizer15BandFX.h"

Equalizer15BandFX::Equalizer15BandFX(HSTREAM streamHandle)
{
    this->streamHandle = streamHandle;

    fxOn = false;

    fxGain[EQFrequency15Range::Frequency25Hz]    = 0;
    fxGain[EQFrequency15Range::Frequency40Hz]    = 0;
    fxGain[EQFrequency15Range::Frequency63Hz]    = 0;
    fxGain[EQFrequency15Range::Frequency100Hz]   = 0;
    fxGain[EQFrequency15Range::Frequency160Hz]   = 0;
    fxGain[EQFrequency15Range::Frequency250Hz]   = 0;
    fxGain[EQFrequency15Range::Frequency400Hz]   = 0;
    fxGain[EQFrequency15Range::Frequency630Hz]   = 0;
    fxGain[EQFrequency15Range::Frequency1000Hz]  = 0;
    fxGain[EQFrequency15Range::Frequency1600Hz]  = 0;
    fxGain[EQFrequency15Range::Frequency2500Hz]  = 0;
    fxGain[EQFrequency15Range::Frequency4000Hz]  = 0;
    fxGain[EQFrequency15Range::Frequency6300Hz]  = 0;
    fxGain[EQFrequency15Range::Frequency10000Hz] = 0;
    fxGain[EQFrequency15Range::Frequency16000Hz] = 0;
}

Equalizer15BandFX::~Equalizer15BandFX()
{
    if (fxOn)
        off();

    fxGain.clear();
    fxEQ.clear();
}

void Equalizer15BandFX::setStreamHandle(HSTREAM streamHandle)
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

void Equalizer15BandFX::on()
{
    if (fxOn)
        return;

    fxOn = true;

    if (streamHandle == 0)
        return;

    // -------------------------

    BASS_DX8_PARAMEQ eq;
    eq.fBandwidth = 18;

    float eqfreq[15] = { 25, 40, 63, 100, 160, 250, 400, 630, 1000, 1600, 2500, 4000, 6300, 10000, 16000};
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

void Equalizer15BandFX::off()
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

std::map<EQFrequency15Range, float> Equalizer15BandFX::gain()
{
    return fxGain;
}

float Equalizer15BandFX::gain(EQFrequency15Range freq)
{
    return fxGain[freq];
}

void Equalizer15BandFX::setGain(EQFrequency15Range freq, float gain)
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

void Equalizer15BandFX::resetGain()
{
    std::map<EQFrequency15Range, float> fg;
    fg = fxGain;

    for (auto const& x : fg)
    {
        setGain(x.first, 0);
    }

    fg.clear();
}
