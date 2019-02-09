#ifndef REVERBFX2_H
#define REVERBFX2_H

#include "FX.h"

class Reverb2FX : public FX
{
private:
    float fDryMix   = 0.0f;
    float fWetMix   = 1.0f;
    float fRoomSize = 0.5f;
    float fDamp     = 0.5f;
    float fWidth    = 1.0f;

public:
    Reverb2FX(DWORD stream = 0, int priority = 1);
    ~Reverb2FX();

    void on();
    void off();

    float dryMix() { return fDryMix; }
    float wetMix() { return fWetMix; }
    float roomSize() { return fRoomSize; }
    float damp() { return fDamp; }
    float width() { return fWidth; }

    void setDryMix(float dm);
    void setWetMix(float wd);
    void setRoomSize(float rSize);
    void setDamp(float dp);
    void setWidth(float wth);

    QList<float> params();
    void setParams(const QList<float> &params);
    void setStreamHandle(DWORD stream);
    void setBypass(bool b);
    void reset();
};

#endif // REVERBFX2_H
