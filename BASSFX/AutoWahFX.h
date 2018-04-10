#ifndef AUTOWAHFX_H
#define AUTOWAHFX_H

#include "FX.h"

class AutoWahFX : public FX
{
public:
    AutoWahFX(DWORD stream = 0, int priority = 1);
    ~AutoWahFX();

    float dryMix() { return fDryMix; }
    float wetMix() { return fWetMix; }
    float feedback() { return fFeedback; }
    float rate() { return fRate; }
    float range() { return fRange; }
    float freq() { return fFreq; }

    void setDryMix(float dm);
    void setWetMix(float wm);
    void setFeedback(float fb);
    void setRate(float r);
    void setRange(float r);
    void setFreq(float f);

    QList<float> params();
    void setParams(const QList<float> &params);
    void setStreamHandle(DWORD stream);
    void setBypass(bool b);
    void reset();

private:
    float fDryMix = 0.500f;
    float fWetMix = 1.500f;
    float fFeedback = 0.5f;
    float fRate = 2.0f;
    float fRange = 4.3f;
    float fFreq = 50.0f;
};

#endif // AUTOWAHFX_H
