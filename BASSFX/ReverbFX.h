#ifndef REVERBFX_H
#define REVERBFX_H

#include "FX.h"

class ReverbFX : public FX
{
private:
    float fInGain = 0.0f;
    float fReverbMix = 0.0f;
    float fReverbTime = 1000.0f;
    float fHighFreqRTRatio = 0.001f;

public:
    ReverbFX(DWORD stream = 0, int priority = 1);
    ~ReverbFX();

    void on();
    void off();

    float inGain() { return fInGain; }
    float reverbMix() { return fReverbMix; }
    float reverbTime() { return fReverbTime; }
    float highFreqRTRatio() { return fHighFreqRTRatio; }

    void setInGain(float ig);
    void setReverbMix(float rm);
    void setReverbTime(float rt);
    void setHighFreqRTRatio(float hf);

    QList<float> params();
    void setParams(const QList<float> &params);
    void setStreamHandle(DWORD stream);
    void setBypass(bool b);
    void reset();
};

#endif // REVERBFX_H
