#include "ChorusFX.h"

DWORD ChorusFX::getBassWaveform(WaveformType wt)
{
    switch (wt) {
    case WaveformType::Triangle:
        return 0;
    case WaveformType::Sine:
        return 1;
    }
}

DWORD ChorusFX::getBassPhase(PhaseType pt)
{
    switch (pt) {
    case PhaseType::PhaseNeg180:
        return BASS_DX8_PHASE_NEG_180;
    case PhaseType::PhaseNeg90:
        return BASS_DX8_PHASE_NEG_90;
    case PhaseType::Phase0:
        return BASS_DX8_PHASE_ZERO;
    case PhaseType::Phase90:
        return BASS_DX8_PHASE_90;
    case PhaseType::Phase180:
        return BASS_DX8_PHASE_180;
    }
}

ChorusFX::ChorusFX(DWORD stream, int priority) :FX(priority)
{
    this->stream = stream;
    this->type = FXType::Chorus;

    _on = false;

    fWetDryMix = 50;
    fDepth = 10;
    fFeedback = 25;
    fFrequency = 1;
    fDelay = 16;
    fWaveform = WaveformType::Sine;
    fPhase = PhaseType::Phase90;
}

ChorusFX::~ChorusFX()
{
    if (_on)
        off();
}

void ChorusFX::on()
{
    if (_on)
        return;

    _on = true;

    if (stream == 0)
        return;

    // --------------------------------

    fx = BASS_ChannelSetFX(stream, BASS_FX_DX8_CHORUS, priority);

    BASS_DX8_CHORUS c;
    c.fWetDryMix = fWetDryMix;
    c.fDepth = fDepth;
    c.fFeedback = fFeedback;
    c.fFrequency = fFrequency;
    c.fDelay = fDelay;
    c.lWaveform = getBassWaveform(fWaveform);
    c.lPhase = getBassPhase(fPhase);

    BASS_FXSetParameters(fx, &c);
}

void ChorusFX::off()
{
    if (!_on)
        return;

    _on = false;

    if (stream == 0)
        return;

    // ------------------------------------

    BASS_ChannelRemoveFX(stream, fx);
}

void ChorusFX::setWetDryMix(float wetDryMix)
{
    if (wetDryMix > 100)
        fWetDryMix = 100;
    else if (wetDryMix < 0)
        fWetDryMix = 0;
    else
        fWetDryMix = wetDryMix;

    if (!_on || stream == 0)
        return;

    BASS_DX8_CHORUS c;
    if (BASS_FXGetParameters(fx, &c))
    {
        c.fWetDryMix = fWetDryMix;
        BASS_FXSetParameters(fx, &c);
    }
}

void ChorusFX::setDepth(float depth)
{
    if (depth > 100)
        fDepth = 100;
    else if (depth < 0)
        fDepth = 0;
    else
        fDepth = depth;

    if (!_on || stream == 0)
        return;

    BASS_DX8_CHORUS c;
    if (BASS_FXGetParameters(fx, &c))
    {
        c.fDepth = fDepth;
        BASS_FXSetParameters(fx, &c);
    }
}

void ChorusFX::setFeedback(float feedback)
{
    if (feedback > 99)
        fFeedback = 99;
    else if (feedback < -99)
        fFeedback = -99;
    else
        fFeedback = feedback;

    if (!_on || stream == 0)
        return;

    BASS_DX8_CHORUS c;
    if (BASS_FXGetParameters(fx, &c))
    {
        c.fFeedback = fFeedback;
        BASS_FXSetParameters(fx, &c);
    }
}

void ChorusFX::setFrequency(float frequency)
{
    if (frequency > 10)
        fFrequency = 10;
    else if (frequency < 0)
        fFrequency = 0;
    else
        fFrequency = frequency;

    if (!_on || stream == 0)
        return;

    BASS_DX8_CHORUS c;
    if (BASS_FXGetParameters(fx, &c))
    {
        c.fFrequency = fFrequency;
        BASS_FXSetParameters(fx, &c);
    }
}

void ChorusFX::setDelay(float delay)
{
    if (delay > 20)
        fDelay = 20;
    else if (delay < 0)
        fDelay = 0;
    else
        fDelay = delay;

    if (!_on || stream == 0)
        return;

    BASS_DX8_CHORUS c;
    if (BASS_FXGetParameters(fx, &c))
    {
        c.fDelay = fDelay;
        BASS_FXSetParameters(fx, &c);
    }
}

void ChorusFX::setWaveform(WaveformType waveform)
{
    fWaveform = waveform;

    if (!_on || stream == 0)
        return;

    BASS_DX8_CHORUS c;
    if (BASS_FXGetParameters(fx, &c))
    {
        c.lWaveform = getBassWaveform(fWaveform);
        BASS_FXSetParameters(fx, &c);
    }
}

void ChorusFX::setPhase(PhaseType phase)
{
    fPhase = phase;

    if (!_on || stream == 0)
        return;

    BASS_DX8_CHORUS c;
    if (BASS_FXGetParameters(fx, &c))
    {
        c.lPhase = getBassPhase(fPhase);
        BASS_FXSetParameters(fx, &c);
    }
}

QList<float> ChorusFX::params()
{
    QList<float> params;

    params.append(fWetDryMix);
    params.append(fDepth);
    params.append(fFeedback);
    params.append(fFrequency);
    params.append(fDelay);
    params.append(getBassWaveform(fWaveform));
    params.append(getBassPhase(fPhase));

    return params;
}

void ChorusFX::setParams(const QList<float> &params)
{
    if (params.count() != 7)
        return;

    setWetDryMix(params[0]);
    setDepth(params[1]);
    setFeedback(params[2]);
    setFrequency(params[3]);
    setDelay(params[4]);

    if (params[5] == 0)
        setWaveform(WaveformType::Triangle);
    else
        setWaveform(WaveformType::Sine);

    DWORD pt = (DWORD)params[6];
    setPhase(static_cast<PhaseType>(pt));
}

void ChorusFX::setStreamHandle(DWORD stream)
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

void ChorusFX::setBypass(bool b)
{
    if (b)
        this->off();
    else
        this->on();
}

void ChorusFX::reset()
{
    setWetDryMix(50);
    setDepth(10);
    setFeedback(25);
    setFrequency(1);
    setDelay(16);
    setWaveform(WaveformType::Sine);
    setPhase(PhaseType::Phase90);
}
