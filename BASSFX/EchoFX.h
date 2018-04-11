#ifndef ECHOFX_H
#define ECHOFX_H

#include "FX.h"

class EchoFX : public FX
{
public:
    EchoFX(DWORD stream = 0, int priority = 1);
    ~EchoFX();

    float wetDryMix() { return fWetDryMix; }
    float feedback() { return fFeedback; }
    float leftDelay() { return fLeftDelay; }
    float rightDelay() { return fRightDelay; }
    bool isPanDelay() { return lPanDelay; }

    void setWetDryMix(float w);
    void setFeedback(float f);
    void setLeftDelay(float l);
    void setRightDelay(float r);
    void setPanDelay(bool pan);

    QList<float> params();
    void setParams(const QList<float> &params);
    void setStreamHandle(DWORD stream);
    void setBypass(bool b);
    void reset();

private:
    float fWetDryMix = 50.0f;
    float fFeedback = 50.0f;
    float fLeftDelay = 500.0f;
    float fRightDelay = 500.0f;
    bool lPanDelay = false;
};

#endif // ECHOFX_H
