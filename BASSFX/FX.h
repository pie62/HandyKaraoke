#ifndef FX_H
#define FX_H

#include <bass.h>
#include <bass_fx.h>

#ifndef __linux__
#include <bass_vst.h>
#endif

#include <QObject>

const int BUILTIN_FX_COUNT = 8;
const QString BUILTIN_FX_NAMES[] = {"Auto Wah", "Chorus", "Compressor", "Distortion",
                                    "Echo", "Equalizer 15 Band", "Equalizer 31 Band", "Reverb"};

enum class FXType {
    AutoWah,
    Chorus,
    Compressor,
    Distortion,
    Echo,
    EQ15Band,
    EQ31Band,
    Reverb,
    VSTEffects,
    Chorus2,
    Reverb2
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
    virtual int program() { return programIndex; }
    virtual void setProgram(int programIndex) { this->programIndex = programIndex; }

    virtual QByteArray chunk() { return QByteArray(); }
    virtual void setChunk(const QByteArray &cnk) { Q_UNUSED(cnk) }

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

protected:
    DWORD stream;
    HFX fx;
    int priority = 1;
    bool _on;
    uint _uids = 0;
    int programIndex = 0;
    FXType type = FXType::AutoWah;
};

#endif // FX_H
