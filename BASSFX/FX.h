#ifndef FX_H
#define FX_H

#include <bass.h>
#include <bass_fx.h>

#ifndef __linux__
#include <bass_vst.h>
#endif

#include <QObject>

enum class FXType {
    AutoWah,
    Chorus,
    Compressor,
    Distortion,
    Echo,
    EQ15Band,
    EQ31Band,
    Reverb,
    VSTEffects
};

class FX
{
public:
    FX(int priority = 1);
    virtual ~FX();

    bool isOn() { return _on; }
    bool isBypass() { return !_on; }
    FXType fxType() { return type; }
    DWORD streamHandle() { return stream; }

    virtual HFX fxHandle() { return fx; }

    virtual uint uids();

    virtual QList<float> params() = 0;
    virtual void setParams(const QList<float> &params) = 0;
    virtual void setStreamHandle(DWORD stream) = 0;
    virtual void setBypass(bool b) = 0;
    virtual void reset() = 0;

    // ================================================================

    #ifndef __linux__
    static QList<float> getVSTParams(DWORD vstHandle);
    static void setVSTParams(DWORD fxHandle, const QList<float> &params);
    #endif

    static QList<HFX>   createEQ31Band(DWORD stream, int priority = 1);
    static void         removeEQ31Band(DWORD stream, QList<HFX> fxs);
    static QList<float> getParamsEQ31Band(QList<HFX> fxs);

protected:
    DWORD stream;
    HFX fx;
    int priority = 1;
    bool _on;
    uint _uids = 0;
    FXType type = FXType::AutoWah;
};

#endif // FX_H
