#include "ReverbFX.h"

ReverbFX::ReverbFX(HSTREAM streamHandle)
{
    stream = streamHandle;
    _on = false;
}

ReverbFX::~ReverbFX()
{
    if (_on)
        off();
}

void ReverbFX::setStreamHandle(HSTREAM streamHandle)
{
    if (_on)
    {
        off();
        stream = streamHandle;
        on();
    }
    else
    {
        stream = streamHandle;
    }
}

void ReverbFX::on()
{
    if (_on)
        return;

    _on = true;

    if (stream == 0)
        return;

    // --------------------------------

    fx = BASS_ChannelSetFX(stream, BASS_FX_DX8_REVERB, 1);

    BASS_DX8_REVERB rv;
    rv.fInGain = fInGain;
    rv.fReverbMix = fReverbMix;
    rv.fReverbTime = fReverbTime;
    rv.fHighFreqRTRatio = fHighFreqRTRatio;

    BASS_FXSetParameters(fx, &rv);
}

void ReverbFX::off()
{
    if (!_on)
        return;

    _on = false;

    if (stream == 0)
        return;

    // ------------------------------------

    BASS_ChannelRemoveFX(stream, fx);
}

void ReverbFX::setInGain(float ig)
{
    if (ig > 0.0f)
        fInGain = 0.0f;
    else if (ig < -96.0f)
        fInGain = -96.0f;
    else
        fInGain = ig;

    if (!_on || stream == 0)
        return;

    BASS_DX8_REVERB rv;
    if (BASS_FXGetParameters(fx, &rv))
    {
        rv.fInGain = fInGain;
        BASS_FXSetParameters(fx, &rv);
    }
}

void ReverbFX::setReverbMix(float rm)
{
    if (rm > 0.0f)
        fReverbMix = 0.0f;
    else if (rm < -96.0f)
        fReverbMix = -96.0f;
    else
        fReverbMix = rm;

    if (!_on || stream == 0)
        return;

    BASS_DX8_REVERB rv;
    if (BASS_FXGetParameters(fx, &rv))
    {
        rv.fReverbMix = fReverbMix;
        BASS_FXSetParameters(fx, &rv);
    }
}

void ReverbFX::setReverbTime(float rt)
{
    if (rt > 3000.0f)
        fReverbTime = 3000.0f;
    else if (rt < 0.001f)
        fReverbTime = 0.001f;
    else
        fReverbTime = rt;

    if (!_on || stream == 0)
        return;

    BASS_DX8_REVERB rv;
    if (BASS_FXGetParameters(fx, &rv))
    {
        rv.fReverbTime = fReverbTime;
        BASS_FXSetParameters(fx, &rv);
    }
}

void ReverbFX::setHighFreqRTRatio(float hf)
{
    if (hf > 0.999f)
        fHighFreqRTRatio = 0.999f;
    else if (hf < 0.001f)
        fHighFreqRTRatio = 0.001f;
    else
        fHighFreqRTRatio = hf;

    if (!_on || stream == 0)
        return;

    BASS_DX8_REVERB rv;
    if (BASS_FXGetParameters(fx, &rv))
    {
        rv.fHighFreqRTRatio = fHighFreqRTRatio;
        BASS_FXSetParameters(fx, &rv);
    }
}

void ReverbFX::reset()
{
    setInGain(0.0f);
    setReverbMix(0.0f);
    setReverbTime(1000.0f);
    setHighFreqRTRatio(0.001f);
}
