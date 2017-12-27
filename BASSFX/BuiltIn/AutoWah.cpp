#include "AutoWah.h"

AutoWah::AutoWah(DWORD stream, int priority)
{
    _stream = stream;
    _bypass = false;
    _type = FXType::AutoWah;
    _fx = BASS_ChannelSetFX(stream, BASS_FX_BFX_AUTOWAH, priority);

    if (_fx != 0) {
        BASS_BFX_AUTOWAH aw;
        BASS_FXGetParameters(_fx, &aw);
        fDryMix = aw.fDryMix;
        fFeedback = aw.fFeedback;
        fFreq = aw.fFreq;
        fRange = aw.fRange;
        fRate = aw.fRate;
        fWetMix = aw.fWetMix;
    }
}

AutoWah::~AutoWah()
{
    if (!_bypass) {
        BASS_ChannelRemoveFX(_stream, _fx);
    }
}
