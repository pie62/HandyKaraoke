#ifndef FX_H
#define FX_H

#include <bass.h>

#ifndef __linux__
#include <bass_vst.h>
#endif

#include <QObject>

//enum class FXType {
//    VSTFX
//};

class FX
{
public:
    FX();

    #ifndef __linux__
    static QList<float> getVSTParams(DWORD vstHandle);
    static void setVSTParams(DWORD fxHandle, const QList<float> &params);
    #endif
};

#endif // FX_H
