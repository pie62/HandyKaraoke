#ifndef DISTORTIONFX_H
#define DISTORTIONFX_H

#include "FX.h"

class DistortionFX : public FX
{
public:
    DistortionFX(DWORD stream = 0, int priority = 1);
    ~DistortionFX();

    float gain() { return fGain; }
    float edge() { return fEdge; }
    float postEQCenterFrequency() { return fPostEQCenterFrequency; }
    float postEQBandwidth() { return fPostEQBandwidth; }
    float preLowpassCutoff() { return fPreLowpassCutoff; }

    void setGain(float g);
    void setEdge(float e);
    void setPostEQCenterFrequency(float p);
    void setPostEQBandwidth(float p);
    void setPreLowpassCutoff(float p);

    QList<float> params();
    void setParams(const QList<float> &params);
    void setStreamHandle(DWORD stream);
    void setBypass(bool b);
    void reset();

private:
    float fGain = -18.0f;
    float fEdge = 15.0f;
    float fPostEQCenterFrequency = 2400.0f;
    float fPostEQBandwidth = 2400.0f;
    float fPreLowpassCutoff = 8000.0f;
};

#endif // DISTORTIONFX_H
