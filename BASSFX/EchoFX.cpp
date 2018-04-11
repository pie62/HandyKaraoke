#include "EchoFX.h"

EchoFX::EchoFX(DWORD stream, int priority) : FX(priority)
{
    this->stream = stream;
    this->type = FXType::Echo;
}

EchoFX::~EchoFX()
{
    if (_on)
        this->setBypass(true);
}

void EchoFX::setWetDryMix(float w)
{
    fWetDryMix = w;

    if (!_on && stream == 0)
        return;

    BASS_DX8_ECHO ec;
    if (BASS_FXGetParameters(fx, &ec))
    {
        ec.fWetDryMix = fWetDryMix;
        BASS_FXSetParameters(fx, &ec);
    }
}

void EchoFX::setFeedback(float f)
{
    fFeedback = f;

    if (!_on && stream == 0)
        return;

    BASS_DX8_ECHO ec;
    if (BASS_FXGetParameters(fx, &ec))
    {
        ec.fFeedback = fFeedback;
        BASS_FXSetParameters(fx, &ec);
    }
}

void EchoFX::setLeftDelay(float l)
{
    fLeftDelay = l;

    if (!_on && stream == 0)
        return;

    BASS_DX8_ECHO ec;
    if (BASS_FXGetParameters(fx, &ec))
    {
        ec.fLeftDelay = fLeftDelay;
        BASS_FXSetParameters(fx, &ec);
    }
}

void EchoFX::setRightDelay(float r)
{
    fRightDelay = r;

    if (!_on && stream == 0)
        return;

    BASS_DX8_ECHO ec;
    if (BASS_FXGetParameters(fx, &ec))
    {
        ec.fRightDelay = fRightDelay;
        BASS_FXSetParameters(fx, &ec);
    }
}

void EchoFX::setPanDelay(bool pan)
{
    lPanDelay = pan;

    if (!_on && stream == 0)
        return;

    BASS_DX8_ECHO ec;
    if (BASS_FXGetParameters(fx, &ec))
    {
        ec.lPanDelay = lPanDelay;
        BASS_FXSetParameters(fx, &ec);
    }
}

QList<float> EchoFX::params()
{
    QList<float> params;

    params.append(fWetDryMix);
    params.append(fFeedback);
    params.append(fLeftDelay);
    params.append(fRightDelay);

    if (lPanDelay)
        params.append(1.0f);
    else
        params.append(0.0f);

    return params;
}

void EchoFX::setParams(const QList<float> &params)
{
    if (params.count() != 5)
        return;

    setWetDryMix(params[0]);
    setFeedback(params[1]);
    setLeftDelay(params[2]);
    setRightDelay(params[3]);

    if (params[4] == 1.0f)
        setPanDelay(true);
    else
        setPanDelay(false);
}

void EchoFX::setStreamHandle(DWORD stream)
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

void EchoFX::setBypass(bool b)
{
    if (b == !_on)
        return;

    _on = !b;

    if (stream == 0)
        return;

    if (_on)
    {
        fx = BASS_ChannelSetFX(stream, BASS_FX_DX8_ECHO, priority);

        BASS_DX8_ECHO ec;
        ec.fWetDryMix = fWetDryMix;
        ec.fFeedback = fFeedback;
        ec.fLeftDelay = fLeftDelay;
        ec.fRightDelay = fRightDelay;
        ec.lPanDelay = lPanDelay;

        BASS_FXSetParameters(fx, &ec);
    }
    else
    {
        BASS_ChannelRemoveFX(stream, fx);
    }
}

void EchoFX::reset()
{
    setWetDryMix(50.0f);
    setFeedback(50.0f);
    setLeftDelay(500.0f);
    setRightDelay(500.0f);
    setPanDelay(false);
}
