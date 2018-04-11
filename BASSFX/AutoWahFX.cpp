#include "AutoWahFX.h"

AutoWahFX::AutoWahFX(DWORD stream, int priority) :FX(priority)
{
    this->stream = stream;
    this->type = FXType::AutoWah;
}

AutoWahFX::~AutoWahFX()
{
    if (_on)
        this->setBypass(true);
}

void AutoWahFX::setDryMix(float dm)
{
    fDryMix = dm;

    if (!_on && stream == 0)
        return;

    BASS_BFX_AUTOWAH aw;
    if (BASS_FXGetParameters(fx, &aw))
    {
        aw.fDryMix = dm;
        BASS_FXSetParameters(fx, &aw);
    }
}

void AutoWahFX::setWetMix(float wm)
{
    fWetMix = wm;

    if (!_on && stream == 0)
        return;

    BASS_BFX_AUTOWAH aw;
    if (BASS_FXGetParameters(fx, &aw))
    {
        aw.fWetMix = wm;
        BASS_FXSetParameters(fx, &aw);
    }
}

void AutoWahFX::setFeedback(float fb)
{
    fFeedback = fb;

    if (!_on && stream == 0)
        return;

    BASS_BFX_AUTOWAH aw;
    if (BASS_FXGetParameters(fx, &aw))
    {
        aw.fFeedback = fb;
        BASS_FXSetParameters(fx, &aw);
    }
}

void AutoWahFX::setRate(float r)
{
    fRate = r;

    if (!_on && stream == 0)
        return;

    BASS_BFX_AUTOWAH aw;
    if (BASS_FXGetParameters(fx, &aw))
    {
        aw.fRate = r;
        BASS_FXSetParameters(fx, &aw);
    }
}

void AutoWahFX::setRange(float r)
{
    fRange = r;

    if (!_on && stream == 0)
        return;

    BASS_BFX_AUTOWAH aw;
    if (BASS_FXGetParameters(fx, &aw))
    {
        aw.fRange = r;
        BASS_FXSetParameters(fx, &aw);
    }
}

void AutoWahFX::setFreq(float f)
{
    fFreq = f;

    if (!_on || stream == 0)
        return;

    BASS_BFX_AUTOWAH aw;
    if (BASS_FXGetParameters(fx, &aw))
    {
        aw.fFreq = f;
        BASS_FXSetParameters(fx, &aw);
    }
}

QList<float> AutoWahFX::params()
{
    QList<float> params;

    params.append(fDryMix);
    params.append(fWetMix);
    params.append(fFeedback);
    params.append(fRate);
    params.append(fRange);
    params.append(fFreq);

    return params;
}

void AutoWahFX::setParams(const QList<float> &params)
{
    if (params.count() != 6)
        return;

    setDryMix(params[0]);
    setWetMix(params[1]);
    setFeedback(params[2]);
    setRate(params[3]);
    setRange(params[4]);
    setFreq(params[5]);
}

void AutoWahFX::setStreamHandle(DWORD stream)
{
    if (_on)
    {
        this->setBypass(true); //off();
        this->stream = stream;
        this->setBypass(false); //on();
    }
    else
    {
        this->stream = stream;
    }
}

void AutoWahFX::setBypass(bool b)
{
    if (b == !_on)
        return;

    _on = !b;

    if (stream == 0)
        return;

    if (_on)
    {
        fx = BASS_ChannelSetFX(stream, BASS_FX_BFX_AUTOWAH, priority);

        BASS_BFX_AUTOWAH aw;
        aw.fDryMix = fDryMix;
        aw.fWetMix = fWetMix;
        aw.fFeedback = fFeedback;
        aw.fRate = fRate;
        aw.fRange = fRange;
        aw.fFreq = fFreq;
        aw.lChannel = BASS_BFX_CHANALL;

        BASS_FXSetParameters(fx, &aw);
    }
    else
    {
        BASS_ChannelRemoveFX(stream, fx);
    }
}

void AutoWahFX::reset()
{
    setDryMix(0.500f);
    setWetMix(1.500f);
    setFeedback(0.5f);
    setRate(2.0f);
    setRange(4.3f);
    setFreq(50.0f);
}
