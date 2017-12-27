#ifndef AUTOWAH_H
#define AUTOWAH_H

#include "BASSFX/FX.h"

class AutoWah : public FX
{
public:
    AutoWah(DWORD stream, int priority = 0);
    ~AutoWah();

    float dryMix()  { return fDryMix; }
    float wetMix()  { return fWetMix; }
    float feedback(){ return fFeedback; }
    float rate()    { return fRate; }
    float range()   { return fRange; }
    float freq()    { return fFreq; }

private:
    float fDryMix;
    float fWetMix;
    float fFeedback;
    float fRate;
    float fRange;
    float fFreq;
};

#endif // AUTOWAH_H
