#ifndef CHORUSFX_H
#define CHORUSFX_H

#include <bass.h>

enum class WaveformType
{
    Triangle = 0,
    Sine     = 1
};

enum class PhaseType
{
    PhaseNeg180,
    PhaseNeg90,
    Phase0,
    Phase90,
    Phase180
};

class ChorusFX
{
private:
    HSTREAM stream;
    HFX fx;
    bool _on;

    float fWetDryMix;
    float fDepth;
    float fFeedback;
    float fFrequency;
    float fDelay;
    WaveformType fWaveform;
    PhaseType fPhase;

    DWORD getBassWaveform(WaveformType wt);
    DWORD getBassPhase(PhaseType pt);

public:
    ChorusFX(HSTREAM streamHandle = 0);
    ~ChorusFX();

    void setStreamHandle(HSTREAM streamHandle);

    bool isOn() { return _on; }

    void on();
    void off();

    float wetDryMix() { return fWetDryMix; }
    float depth() { return fDepth; }
    float feedback() { return fFeedback; }
    float frequency() { return fFrequency; }
    float delay() { return fDelay; }
    WaveformType waveform() { return fWaveform; }
    PhaseType phase() { return fPhase; }

    void setWetDryMix(float wetDryMix);
    void setDepth(float depth);
    void setFeedback(float feedback);
    void setFrequency(float frequency);
    void setDelay(float delay);
    void setWaveform(WaveformType waveform);
    void setPhase(PhaseType phase);

    void reset();
};

#endif // CHORUSFX_H
