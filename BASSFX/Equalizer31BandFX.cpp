#include "Equalizer31BandFX.h"

#include <bass_fx.h>

Equalizer31BandFX::Equalizer31BandFX(DWORD stream, int priority) :FX(priority)
{
    this->stream = stream;
    this->type = FXType::EQ31Band;

    _on = false;

    fxGain[EQFrequency31Range::Frequency20Hz]    = 0;
    fxGain[EQFrequency31Range::Frequency25Hz]    = 0;
    fxGain[EQFrequency31Range::Frequency31d5Hz]  = 0;
    fxGain[EQFrequency31Range::Frequency40Hz]    = 0;
    fxGain[EQFrequency31Range::Frequency50Hz]    = 0;
    fxGain[EQFrequency31Range::Frequency63Hz]    = 0;
    fxGain[EQFrequency31Range::Frequency80Hz]    = 0;
    fxGain[EQFrequency31Range::Frequency100Hz]   = 0;
    fxGain[EQFrequency31Range::Frequency125Hz]   = 0;
    fxGain[EQFrequency31Range::Frequency160Hz]   = 0;
    fxGain[EQFrequency31Range::Frequency200Hz]   = 0;
    fxGain[EQFrequency31Range::Frequency250Hz]   = 0;
    fxGain[EQFrequency31Range::Frequency315Hz]   = 0;
    fxGain[EQFrequency31Range::Frequency400Hz]   = 0;
    fxGain[EQFrequency31Range::Frequency500Hz]   = 0;
    fxGain[EQFrequency31Range::Frequency630Hz]   = 0;
    fxGain[EQFrequency31Range::Frequency800Hz]   = 0;
    fxGain[EQFrequency31Range::Frequency1000Hz]  = 0;
    fxGain[EQFrequency31Range::Frequency1250Hz]  = 0;
    fxGain[EQFrequency31Range::Frequency1600Hz]  = 0;
    fxGain[EQFrequency31Range::Frequency2000Hz]  = 0;
    fxGain[EQFrequency31Range::Frequency2500Hz]  = 0;
    fxGain[EQFrequency31Range::Frequency3150Hz]  = 0;
    fxGain[EQFrequency31Range::Frequency4000Hz]  = 0;
    fxGain[EQFrequency31Range::Frequency5000Hz]  = 0;
    fxGain[EQFrequency31Range::Frequency6300Hz]  = 0;
    fxGain[EQFrequency31Range::Frequency8000Hz]  = 0;
    fxGain[EQFrequency31Range::Frequency10000Hz] = 0;
    fxGain[EQFrequency31Range::Frequency12500Hz] = 0;
    fxGain[EQFrequency31Range::Frequency16000Hz] = 0;
    fxGain[EQFrequency31Range::Frequency20000Hz] = 0;
}

Equalizer31BandFX::~Equalizer31BandFX()
{
    if (_on)
        off();

    fxGain.clear();
    fxEQ.clear();
}

void Equalizer31BandFX::on()
{
    if (_on)
        return;

    _on = true;

    if (stream == 0)
        return;

    // -------------------------

    BASS_BFX_PEAKEQ eq;
    eq.fQ = 0;
    eq.fBandwidth = 2.5f;
    eq.lChannel = BASS_BFX_CHANALL;

    float eqfreq[31] = { 20, 25, 31.5, 40, 50, 63, 80, 100, 125, 160,
                         200, 250, 315, 400, 500, 630, 800, 1000, 1250, 1600,
                         2000, 2500, 3150, 4000, 5000, 6300, 8000, 10000, 12500, 16000, 20000 };
    fxEQ.clear();
    int i = 0;
    for (auto const& x : fxGain)
    {
        eq.lBand = i;
        eq.fCenter = eqfreq[i];
        eq.fGain = x.second;

        HFX fx = BASS_ChannelSetFX(stream, BASS_FX_BFX_PEAKEQ, priority);

        BASS_FXSetParameters(fx, &eq);

        fxEQ[x.first] = fx;

        i++;
    }
}

void Equalizer31BandFX::off()
{
    if (!_on)
        return;

    _on = false;

    if (stream == 0)
        return;

    //========================

    for (auto const& x : fxEQ)
    {
        BASS_ChannelRemoveFX(stream, x.second);
    }

    fxEQ.clear();
}

std::map<EQFrequency31Range, float> Equalizer31BandFX::gain()
{
    return fxGain;
}

float Equalizer31BandFX::gain(EQFrequency31Range freq)
{
    return fxGain[freq];
}

void Equalizer31BandFX::setGain(EQFrequency31Range freq, float gain)
{
    float g = 0;
    if (gain > 15)
        g = 15;
    else if (gain < -15)
        g = -15;
    else
        g = gain;

    fxGain[freq] = g;

    if (!_on || stream == 0)
        return;

    BASS_BFX_PEAKEQ eq;
    eq.lBand = static_cast<int>(freq);
    if (BASS_FXGetParameters(fxEQ[freq], &eq))
    {
        eq.fGain = g;
        BASS_FXSetParameters(fxEQ[freq], &eq);
    }
}

void Equalizer31BandFX::resetGain()
{
    std::map<EQFrequency31Range, float> fg;
    fg = fxGain;

    for (auto const& x : fg)
    {
        setGain(x.first, 0);
    }

    fg.clear();
}

QList<float> Equalizer31BandFX::params()
{
    QList<float> params;

    for (auto const& x : fxGain)
    {
        params.append(x.second);
    }

    return params;
}

void Equalizer31BandFX::setParams(const QList<float> &params)
{
    if (params.count() != 31)
        return;

    for (int i=0; i<31; i++)
    {
        EQFrequency31Range freq = static_cast<EQFrequency31Range>(i);
        this->setGain(freq, params[i]);
    }
}

void Equalizer31BandFX::setStreamHandle(DWORD stream)
{
    if (_on)
    {
        off();
        this->stream = stream;
        on();
    }
    else
    {
        this->stream = stream;
    }
}

void Equalizer31BandFX::setBypass(bool b)
{
    if (b)
        this->off();
    else
        this->on();
}

void Equalizer31BandFX::reset()
{
    this->resetGain();
}
