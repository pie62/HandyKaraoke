#include "FX.h"

FX::FX()
{

}

FX::~FX()
{

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

#endif
