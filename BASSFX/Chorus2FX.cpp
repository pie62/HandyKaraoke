#include "Chorus2FX.h"

Chorus2FX::Chorus2FX(DWORD stream, int priority) : FX(priority)
{
    this->stream = stream;
    this->type = FXType::Chorus2;
    _on = false;
}

Chorus2FX::~Chorus2FX()
{
    if (_on)
        off();
}

void Chorus2FX::on()
{
    if (_on)
        return;

    _on = true;

    if (stream == 0)
        return;

    // --------------------------------

    fx = BASS_ChannelSetFX(stream, BASS_FX_BFX_CHORUS, priority);

    BASS_BFX_CHORUS cr;
    cr.fDryMix = fDryMix;
    cr.fWetMix = fWetMix;
    cr.fFeedback = fFeedback;
    cr.fMinSweep = fMinSweep;
    cr.fMaxSweep = fMaxSweep;
    cr.fRate = fRate;
    cr.lChannel = BASS_BFX_CHANALL;

    BASS_FXSetParameters(fx, &cr);
}

void Chorus2FX::off()
{
    if (!_on)
        return;

    _on = false;

    if (stream == 0)
        return;

    // ------------------------------------

    BASS_ChannelRemoveFX(stream, fx);
}

void Chorus2FX::setDryMix(float dm)
{
    if (dm < -2.0f)
        fDryMix = -2.0f;
    else if (dm > 2.0f)
        fDryMix = 2.0f;
    else
        fDryMix = dm;

    if (!_on || stream == 0)
        return;

    BASS_BFX_CHORUS cr;
    if (BASS_FXGetParameters(fx, &cr))
    {
        cr.fDryMix = fDryMix;
        BASS_FXSetParameters(fx, &cr);
    }
}

void Chorus2FX::setWetMix(float wm)
{
    if (wm < -2.0f)
        fWetMix = -2.0f;
    else if (wm > 2.0f)
        fWetMix = 2.0f;
    else
        fWetMix = wm;

    if (!_on || stream == 0)
        return;

    BASS_BFX_CHORUS cr;
    if (BASS_FXGetParameters(fx, &cr))
    {
        cr.fWetMix = fWetMix;
        BASS_FXSetParameters(fx, &cr);
    }
}

void Chorus2FX::setFeedback(float fb)
{
    if (fb < -1.0f)
        fFeedback = -1.0f;
    else if (fb > 1.0f)
        fFeedback = 1.0f;
    else
        fFeedback = fb;

    if (!_on || stream == 0)
        return;

    BASS_BFX_CHORUS cr;
    if (BASS_FXGetParameters(fx, &cr))
    {
        cr.fFeedback = fFeedback;
        BASS_FXSetParameters(fx, &cr);
    }
}

void Chorus2FX::setMinSweep(float minSw)
{
    if (minSw < 0.0f)
        fMinSweep = 0.0f;
    else if (minSw > 6000.0f)
        fMinSweep = 6000.0f;
    else
        fMinSweep = minSw;

    if (!_on || stream == 0)
        return;

    BASS_BFX_CHORUS cr;
    if (BASS_FXGetParameters(fx, &cr))
    {
        cr.fMinSweep = fMinSweep;
        BASS_FXSetParameters(fx, &cr);
    }
}

void Chorus2FX::setMaxSweep(float maxSw)
{
    if (maxSw < 0.0f)
        fMaxSweep = 0.0f;
    else if (maxSw > 6000.0f)
        fMaxSweep = 6000.0f;
    else
        fMaxSweep = maxSw;

    if (!_on || stream == 0)
        return;

    BASS_BFX_CHORUS cr;
    if (BASS_FXGetParameters(fx, &cr))
    {
        cr.fMaxSweep = fMaxSweep;
        BASS_FXSetParameters(fx, &cr);
    }
}

void Chorus2FX::setRate(float rt)
{
    if (rt < 0.0f)
        fRate = 0.0f;
    else if (rt > 1000.0f)
        fRate = 1000.0f;
    else
        fRate = rt;

    if (!_on || stream == 0)
        return;

    BASS_BFX_CHORUS cr;
    if (BASS_FXGetParameters(fx, &cr))
    {
        cr.fRate = fRate;
        BASS_FXSetParameters(fx, &cr);
    }
}

QList<float> Chorus2FX::params()
{
    QList<float> params;

    params.append(fDryMix);
    params.append(fWetMix);
    params.append(fFeedback);
    params.append(fMinSweep);
    params.append(fMaxSweep);
    params.append(fRate);

    return params;
}

void Chorus2FX::setParams(const QList<float> &params)
{
    if (params.count() != 6)
        return;

    setDryMix(params[0]);
    setWetMix(params[1]);
    setFeedback(params[2]);
    setMinSweep(params[3]);
    setMaxSweep(params[4]);
    setRate(params[5]);
}

void Chorus2FX::setStreamHandle(DWORD stream)
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

void Chorus2FX::setBypass(bool b)
{
    if (b)
        this->off();
    else
        this->on();
}

void Chorus2FX::reset()
{
    setDryMix(0.9f);
    setWetMix(-0.2f);
    setFeedback(0.5f);
    setMinSweep(1.0f);
    setMaxSweep(2.0f);
    setRate(10.0f);
}
