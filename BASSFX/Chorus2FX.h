#ifndef CHORUSFX2_H
#define CHORUSFX2_H

#include "FX.h"

class Chorus2FX : public FX
{
private:
    float fDryMix   = 0.9f;
    float fWetMix   = -0.2f;
    float fFeedback = 0.5f;
    float fMinSweep = 1.0f;
    float fMaxSweep = 2.0f;
    float fRate     = 10.0f;

public:
    Chorus2FX(DWORD stream = 0, int priority = 1);
    ~Chorus2FX();

    void on();
    void off();

    float dryMix() { return fDryMix; }
    float wetMix() { return fWetMix; }
    float feedback() { return fFeedback; }
    float minSweep() { return fMinSweep; }
    float maxSweep() { return fMaxSweep; }
    float rate() { return fRate; }

    void setDryMix(float dm);
    void setWetMix(float wm);
    void setFeedback(float fb);
    void setMinSweep(float minSw);
    void setMaxSweep(float maxSw);
    void setRate(float rt);

    QList<float> params();
    void setParams(const QList<float> &params);
    void setStreamHandle(DWORD stream);
    void setBypass(bool b);
    void reset();
};

#endif // CHORUSFX2_H
