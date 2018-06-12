#ifndef MIXERMANAGER_H
#define MIXERMANAGER_H

#include <QMap>
#include "BASSFX/FX.h"
#include "BASSFX/Equalizer31BandFX.h"
#include "BASSFX/ReverbFX.h"
#include "BASSFX/ChorusFX.h"

typedef struct
{
    DWORD handle;

    Equalizer31BandFX *eq;
    ReverbFX *reverb;
    ChorusFX *chorus;

    DWORD bus[16];
    QList<FX*> fxs[16]; // bus fx
} MixerHandle;

class MixerManager
{
public:
    MixerManager(const QMap<int, QString> &devices);
    ~MixerManager();

    void open(DWORD defaultDev, bool useSampleFloat);
    void close();
    bool isOpenned();

    int mixerCount();

    QList<DWORD> mixerHandles();
    DWORD mixerHandle(int mix);

    QList<DWORD> busHandles(int mix);
    DWORD busHandle(int mix, int bus);

    QList<Equalizer31BandFX *> mixerEqualizers();
    Equalizer31BandFX *mixerEqualizer(int mix);

    QList<ReverbFX *> mixerReverbs();
    ReverbFX *mixerReverb(int mix);

    void moveChannel(DWORD stream, int mix, int bus, DWORD speakerFlag);
    bool addFX(DWORD stream, int mix, int bus, FX *fx);

private:
    bool isInMixer(int mix, int bus);

private:
    QMap<int, QString> devices;
    QList<MixerHandle> mixers;

    bool openned = false;
};

#endif // MIXERMANAGER_H
