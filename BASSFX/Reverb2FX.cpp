#include "Reverb2FX.h"

Reverb2FX::Reverb2FX(DWORD stream, int priority) : FX(priority)
{
    this->stream = stream;
    this->type = FXType::Reverb2;
    _on = false;
}

Reverb2FX::~Reverb2FX()
{
    if (_on)
        off();
}

void Reverb2FX::on()
{
    if (_on)
        return;

    _on = true;

    if (stream == 0)
        return;

    // --------------------------------

    fx = BASS_ChannelSetFX(stream, BASS_FX_BFX_FREEVERB, priority);

    BASS_BFX_FREEVERB rv;
    rv.fDryMix = fDryMix;
    rv.fWetMix = fWetMix;
    rv.fRoomSize = fRoomSize;
    rv.fDamp = fDamp;
    rv.fWidth = fWidth;
    rv.lChannel = BASS_BFX_CHANALL;

    BASS_FXSetParameters(fx, &rv);
}

void Reverb2FX::off()
{
    if (!_on)
        return;

    _on = false;

    if (stream == 0)
        return;

    // ------------------------------------

    BASS_ChannelRemoveFX(stream, fx);
}

void Reverb2FX::setDryMix(float dm)
{
    if (dm < 0.0f)
        fDryMix = 0.0f;
    else if (dm > 1.0f)
        fDryMix = 1.0f;
    else
        fDryMix = dm;

    if (!_on || stream == 0)
        return;

    BASS_BFX_FREEVERB rv;
    if (BASS_FXGetParameters(fx, &rv))
    {
        rv.fDryMix = fDryMix;
        BASS_FXSetParameters(fx, &rv);
    }
}

void Reverb2FX::setWetMix(float wd)
{
    if (wd < 0.0f)
        fWetMix = 0.0f;
    else if (wd > 3.0f)
        fWetMix = 3.0f;
    else
        fWetMix = wd;

    if (!_on || stream == 0)
        return;

    BASS_BFX_FREEVERB rv;
    if (BASS_FXGetParameters(fx, &rv))
    {
        rv.fWetMix = fWetMix;
        BASS_FXSetParameters(fx, &rv);
    }
}

void Reverb2FX::setRoomSize(float rSize)
{
    if (rSize < 0.0f)
        fRoomSize = 0.0f;
    else if (rSize > 1.0f)
        fRoomSize = 1.0f;
    else
        fRoomSize = rSize;

    if (!_on || stream == 0)
        return;

    BASS_BFX_FREEVERB rv;
    if (BASS_FXGetParameters(fx, &rv))
    {
        rv.fRoomSize = fRoomSize;
        BASS_FXSetParameters(fx, &rv);
    }
}

void Reverb2FX::setDamp(float dp)
{
    if (dp < 0.0f)
        fDamp = 0.0f;
    else if (dp > 1.0f)
        fDamp = 1.0f;
    else
        fDamp = dp;

    if (!_on || stream == 0)
        return;

    BASS_BFX_FREEVERB rv;
    if (BASS_FXGetParameters(fx, &rv))
    {
        rv.fDamp = fDamp;
        BASS_FXSetParameters(fx, &rv);
    }
}

void Reverb2FX::setWidth(float wth)
{
    if (wth < 0.0f)
        fWidth = 0.0f;
    else if (wth > 1.0f)
        fWidth = 1.0f;
    else
        fWidth = wth;

    if (!_on || stream == 0)
        return;

    BASS_BFX_FREEVERB rv;
    if (BASS_FXGetParameters(fx, &rv))
    {
        rv.fWidth = fWidth;
        BASS_FXSetParameters(fx, &rv);
    }
}

QList<float> Reverb2FX::params()
{
    QList<float> params;
    params.append(fDryMix);
    params.append(fWetMix);
    params.append(fRoomSize);
    params.append(fDamp);
    params.append(fWidth);
    return params;
}

void Reverb2FX::setParams(const QList<float> &params)
{
    if (params.count() != 5)
        return;

    setDryMix(params[0]);
    setWetMix(params[1]);
    setRoomSize(params[2]);
    setDamp(params[3]);
    setWidth(params[4]);
}

void Reverb2FX::setStreamHandle(DWORD stream)
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

void Reverb2FX::setBypass(bool b)
{
    if (b)
        this->off();
    else
        this->on();
}

void Reverb2FX::reset()
{
    setDryMix(0.0f);
    setWetMix(1.0f);
    setRoomSize(0.5f);
    setDamp(0.5f);
    setWidth(1.0f);
}
