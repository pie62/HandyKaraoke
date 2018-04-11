#include "DistortionFX.h"

DistortionFX::DistortionFX(DWORD stream, int priority) : FX(priority)
{
    this->stream = stream;
    this->type = FXType::Distortion;
}

DistortionFX::~DistortionFX()
{
    if (_on)
        this->setBypass(true);
}

void DistortionFX::setGain(float g)
{
    fGain = g;

    if (!_on && stream == 0)
        return;

    BASS_DX8_DISTORTION dis;
    if (BASS_FXGetParameters(fx, &dis))
    {
        dis.fGain = fGain;
        BASS_FXSetParameters(fx, &dis);
    }
}

void DistortionFX::setEdge(float e)
{
    fEdge = e;

    if (!_on && stream == 0)
        return;

    BASS_DX8_DISTORTION dis;
    if (BASS_FXGetParameters(fx, &dis))
    {
        dis.fEdge = fEdge;
        BASS_FXSetParameters(fx, &dis);
    }
}

void DistortionFX::setPostEQCenterFrequency(float p)
{
    fPostEQCenterFrequency = p;

    if (!_on && stream == 0)
        return;

    BASS_DX8_DISTORTION dis;
    if (BASS_FXGetParameters(fx, &dis))
    {
        dis.fPostEQCenterFrequency = fPostEQCenterFrequency;
        BASS_FXSetParameters(fx, &dis);
    }
}

void DistortionFX::setPostEQBandwidth(float p)
{
    fPostEQBandwidth = p;

    if (!_on && stream == 0)
        return;

    BASS_DX8_DISTORTION dis;
    if (BASS_FXGetParameters(fx, &dis))
    {
        dis.fPostEQBandwidth = fPostEQBandwidth;
        BASS_FXSetParameters(fx, &dis);
    }
}

void DistortionFX::setPreLowpassCutoff(float p)
{
    fPreLowpassCutoff = p;

    if (!_on && stream == 0)
        return;

    BASS_DX8_DISTORTION dis;
    if (BASS_FXGetParameters(fx, &dis))
    {
        dis.fPreLowpassCutoff = fPreLowpassCutoff;
        BASS_FXSetParameters(fx, &dis);
    }
}

QList<float> DistortionFX::params()
{
    QList<float> params;

    params.append(fGain);
    params.append(fEdge);
    params.append(fPostEQCenterFrequency);
    params.append(fPostEQBandwidth);
    params.append(fPreLowpassCutoff);

    return params;
}

void DistortionFX::setParams(const QList<float> &params)
{
    if (params.count() != 5)
        return;

    setGain(params[0]);
    setEdge(params[1]);
    setPostEQCenterFrequency(params[2]);
    setPostEQBandwidth(params[3]);
    setPreLowpassCutoff(params[4]);
}

void DistortionFX::setStreamHandle(DWORD stream)
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

void DistortionFX::setBypass(bool b)
{
    if (b == !_on)
        return;

    _on = !b;

    if (stream == 0)
        return;

    if (_on)
    {
        fx = BASS_ChannelSetFX(stream, BASS_FX_DX8_DISTORTION, priority);

        BASS_DX8_DISTORTION dis;
        dis.fGain = fGain;
        dis.fEdge = fEdge;
        dis.fPostEQCenterFrequency = fPostEQCenterFrequency;
        dis.fPostEQBandwidth = fPostEQBandwidth;
        dis.fPreLowpassCutoff = fPreLowpassCutoff;

        BASS_FXSetParameters(fx, &dis);
    }
    else
    {
        BASS_ChannelRemoveFX(stream, fx);
    }
}

void DistortionFX::reset()
{
    setGain(-18.0f);
    setEdge(15.0f);
    setPostEQCenterFrequency(2400.0f);
    setPostEQBandwidth(2400.0f);
    setPreLowpassCutoff(8000.0f);
}
