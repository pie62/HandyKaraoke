#ifndef COMPRESSORFX_H
#define COMPRESSORFX_H

#include "FX.h"

class CompressorFX : public FX
{
public:
    CompressorFX(DWORD stream = 0, int priority = 1);
    ~CompressorFX();

    float gain() { return fGain; }
    float attack() { return fAttack; }
    float release() { return fRelease; }
    float threshold() { return fThreshold; }
    float ratio() { return fRatio; }

    void setGain(float g);
    void setAttack(float a);
    void setRelease(float r);
    void setThreshold(float t);
    void setRatio(float r);

    QList<float> params();
    void setParams(const QList<float> &params);
    void setStreamHandle(DWORD stream);
    void setBypass(bool b);
    void reset();

private:
    float fGain = 0.0f;
    float fAttack = 10.0f;
    float fRelease = 200.0f;
    float fThreshold = -15.0f;
    float fRatio = 3.0f;
};

#endif // COMPRESSORFX_H
