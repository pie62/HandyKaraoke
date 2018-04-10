#include "CompressorFX.h"

CompressorFX::CompressorFX(DWORD stream, int priority) :FX(priority)
{
    this->stream = stream;
    this->type = FXType::Compressor;
}

CompressorFX::~CompressorFX()
{
    if (_on)
        this->setBypass(true);
}

void CompressorFX::setGain(float g)
{
    fGain = g;

    if (!_on && stream == 0)
        return;

    BASS_BFX_COMPRESSOR2 cp;
    if (BASS_FXGetParameters(fx, &cp))
    {
        cp.fGain = fGain;
        BASS_FXSetParameters(fx, &cp);
    }
}

void CompressorFX::setAttack(float a)
{
    fAttack = a;

    if (!_on && stream == 0)
        return;

    BASS_BFX_COMPRESSOR2 cp;
    if (BASS_FXGetParameters(fx, &cp))
    {
        cp.fAttack = fAttack;
        BASS_FXSetParameters(fx, &cp);
    }
}

void CompressorFX::setRelease(float r)
{
    fRelease = r;

    if (!_on && stream == 0)
        return;

    BASS_BFX_COMPRESSOR2 cp;
    if (BASS_FXGetParameters(fx, &cp))
    {
        cp.fRelease = fRelease;
        BASS_FXSetParameters(fx, &cp);
    }
}

void CompressorFX::setThreshold(float t)
{
    fThreshold = t;

    if (!_on && stream == 0)
        return;

    BASS_BFX_COMPRESSOR2 cp;
    if (BASS_FXGetParameters(fx, &cp))
    {
        cp.fThreshold = fThreshold;
        BASS_FXSetParameters(fx, &cp);
    }
}

void CompressorFX::setRatio(float r)
{
    fRatio = r;

    if (!_on && stream == 0)
        return;

    BASS_BFX_COMPRESSOR2 cp;
    if (BASS_FXGetParameters(fx, &cp))
    {
        cp.fRatio = fRatio;
        BASS_FXSetParameters(fx, &cp);
    }
}

QList<float> CompressorFX::params()
{
    QList<float> params;

    params.append(fGain);
    params.append(fAttack);
    params.append(fRelease);
    params.append(fThreshold);
    params.append(fRatio);

    return params;
}

void CompressorFX::setParams(const QList<float> &params)
{
    if (params.count() != 5)
        return;

    setGain(params[0]);
    setAttack(params[1]);
    setRelease(params[2]);
    setThreshold(params[3]);
    setRatio(params[4]);
}

void CompressorFX::setStreamHandle(DWORD stream)
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

void CompressorFX::setBypass(bool b)
{
    if (b == !_on)
        return;

    _on = !b;

    if (stream == 0)
        return;

    if (_on)
    {
        fx = BASS_ChannelSetFX(stream, BASS_FX_BFX_COMPRESSOR2, priority);

        BASS_BFX_COMPRESSOR2 cp;
        cp.fGain = fGain;
        cp.fAttack = fAttack;
        cp.fRelease = fRelease;
        cp.fThreshold = fThreshold;
        cp.fRatio = fRatio;
        cp.lChannel = BASS_BFX_CHANALL;

        BASS_FXSetParameters(fx, &cp);
    }
    else
    {
        BASS_ChannelRemoveFX(stream, fx);
    }
}

void CompressorFX::reset()
{
    setGain(0.0f);
    setAttack(10.0f);
    setRelease(200.0f);
    setThreshold(-15.0f);
    setRatio(3.0f);
}
