#include "FX.h"


FX::FX(int priority)
{
    this->priority = priority;
    _on = false;
}

FX::~FX()
{

}

uint FX::uids()
{
    return static_cast<unsigned int>(this->type);
}


#ifndef __linux__

QList<float> FX::getVSTParams(DWORD vstHandle)
{
    QList<float> params;
    int count = BASS_VST_GetParamCount(vstHandle);
    for (int i=0; i<count; i++) {
        params.append(BASS_VST_GetParam(vstHandle, i));
    }
    return params;
}

void FX::setVSTParams(DWORD fxHandle, const QList<float> &params)
{
    for (int i=0; i<params.count(); i++) {
        BASS_VST_SetParam(fxHandle, i, params[i]);
    }
}

QList<HFX> FX::createEQ31Band(DWORD stream, int priority)
{
    QList<HFX> fxs;

    float eqfreq[31] = { 20, 25, 31.5, 40, 50, 63, 80, 100, 125, 160,
                         200, 250, 315, 400, 500, 630, 800, 1000, 1250, 1600,
                         2000, 2500, 3150, 4000, 5000, 6300, 8000, 10000, 12500, 16000, 20000 };

    BASS_BFX_PEAKEQ eq;
    eq.fQ = 0;
    eq.fBandwidth = 2.5f;
    eq.lChannel = BASS_BFX_CHANALL;

    for (int i=0; i<31; i++)
    {
        eq.lBand = i;
        eq.fCenter = eqfreq[i];
        eq.fGain = 0;

        HFX fx = BASS_ChannelSetFX(stream, BASS_FX_BFX_PEAKEQ, 1);
        BASS_FXSetParameters(fx, &eq);

        fxs.append(fx);
    }

    return fxs;

}

void FX::removeEQ31Band(DWORD stream, QList<HFX> fxs)
{
    for (HFX fx : fxs)
    {
        BASS_ChannelRemoveFX(stream, fx);
    }
}

QList<float> FX::getParamsEQ31Band(QList<HFX> fxs)
{
    QList<float> params = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                            0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

    if (fxs.count() != 31)
        return params;

    for (int i=0; i<31; i++)
    {
        BASS_BFX_PEAKEQ eq;
        eq.lBand = i;

        if (BASS_FXGetParameters(fxs[i], &eq))
            params[i] = eq.fGain;
    }

    return params;
}

#endif
