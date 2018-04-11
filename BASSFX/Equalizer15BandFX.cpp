#include "Equalizer15BandFX.h"

Equalizer15BandFX::Equalizer15BandFX(DWORD stream, int priority) :FX(priority)
{
    this->stream = stream;
    this->type = FXType::EQ15Band;

    _on = false;

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
    if (_on)
        off();

    fxGain.clear();
    fxEQ.clear();
}

void Equalizer15BandFX::on()
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

    float eqfreq[15] = { 25, 40, 63, 100, 160, 250, 400, 630, 1000,
                         1600, 2500, 4000, 6300, 10000, 16000};

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

void Equalizer15BandFX::off()
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

QList<float> Equalizer15BandFX::params()
{
    QList<float> params;

    for (auto const& x : fxGain)
    {
        params.append(x.second);
    }

    return params;
}

void Equalizer15BandFX::setParams(const QList<float> &params)
{
    if (params.count() != 15)
        return;

    for (int i=0; i<15; i++)
    {
        EQFrequency15Range freq = static_cast<EQFrequency15Range>(i);
        this->setGain(freq, params[i]);
    }
}

void Equalizer15BandFX::setStreamHandle(DWORD stream)
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

void Equalizer15BandFX::setBypass(bool b)
{
    if (b)
        this->off();
    else
        this->on();
}

void Equalizer15BandFX::reset()
{
    this->resetGain();
}
