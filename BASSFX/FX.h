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
    EQ15Band,
    EQ31Band,
    Rverb
};

class FX
{
public:
    FX();
    virtual ~FX();

    bool isBypass() { return _bypass; }
    FXType type() { return _type; }

    virtual void setBypass(bool bypass) = 0;

    virtual QList<float> params() = 0;
    virtual void setParams(const QList<float> &p) = 0;


    #ifndef __linux__
    static QList<float> getVSTParams(DWORD vstHandle);
    static void setVSTParams(DWORD fxHandle, const QList<float> &params);
    #endif

protected:
    FXType _type;
    DWORD _stream = 0;
    HFX _fx = 0;
    bool _bypass = false;
};

#endif // FX_H
