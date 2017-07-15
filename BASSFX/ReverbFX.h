#ifndef REVERBFX_H
#define REVERBFX_H

#include <bass.h>

class ReverbFX
{
private:
    HSTREAM stream;
    HFX fx;
    bool _on;

    float fInGain = 0.0f;
    float fReverbMix = 0.0f;
    float fReverbTime = 1000.0f;
    float fHighFreqRTRatio = 0.001f;

public:
    ReverbFX(HSTREAM streamHandle = 0);
    ~ReverbFX();

    void setStreamHandle(HSTREAM streamHandle);

    bool isOn() { return _on; }

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
    void reset();
};

#endif // REVERBFX_H
