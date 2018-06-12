#include "MixerManager.h"

#include <bassmix.h>

MixerManager::MixerManager(const QMap<int, QString> &devices)
{
    this->devices = devices;

    for (int dv : devices.keys())
    {
        MixerHandle mixer;
        mixer.handle = 0;

        mixer.eq = new Equalizer31BandFX();
        mixer.reverb = new ReverbFX();
        mixer.chorus = new ChorusFX();

        for (int b=0; b<16; b++)
            mixer.bus[b] = 0;

        mixers.append(mixer);
    }
}

MixerManager::~MixerManager()
{
    close();

    for (MixerHandle mixer : mixers)
    {
        delete mixer.eq;
        delete mixer.reverb;
        delete mixer.chorus;

        for (int b=0; b<16; b++)        // ลูปทั้ง 16 bus
            for (FX *fx : mixer.fxs[b]) // ลูป FX ใน bus
                delete fx;
    }

    mixers.clear();
}

void MixerManager::open(DWORD defaultDev, bool useSampleFloat)
{
    if (openned)
        return;

    DWORD f = useSampleFloat ? BASS_SAMPLE_FLOAT : 0;

    for (int i=0; i<mixers.count(); i++)
    {
        MixerHandle mixer;
        mixer.handle = BASS_Mixer_StreamCreate(44100, 2, f);

        mixer.eq->setStreamHandle(mixer.handle);
        mixer.reverb->setStreamHandle(mixer.handle);
        mixer.chorus->setStreamHandle(mixer.handle);

        for (int b=0; b<16; b++)
        {
            mixer.bus[b] = BASS_Mixer_StreamCreate(44100, 2, f|BASS_STREAM_DECODE);
            BASS_Mixer_StreamAddChannel(mixer.handle, mixer.bus[b], 0);

            for (FX *fx : mixer.fxs[b])
                fx->setStreamHandle(mixer.bus[b]);
        }

        DWORD device = (i == 0) ? defaultDev : devices.keys()[i];
        BASS_ChannelSetDevice(mixer.handle, device);
        BASS_ChannelPlay(mixer.handle, false);

        mixers[i] = mixer;
    }

    openned = true;
}

void MixerManager::close()
{
    if (!openned)
        return;

    for (MixerHandle mixer : mixers)
    {
        BASS_ChannelStop(mixer.handle);

        for (int b=0; b<16; b++)
        {
            // fx
            for (FX *fx : mixer.fxs[b])
                fx->setStreamHandle(0);

            // free bus
            BASS_Mixer_ChannelRemove(mixer.bus[b]);
            BASS_StreamFree(mixer.bus[b]);
        }

        mixer.eq->setStreamHandle(0);
        mixer.reverb->setStreamHandle(0);
        mixer.chorus->setStreamHandle(0);

        BASS_StreamFree(mixer.handle);
    }

    openned = false;
}

bool MixerManager::isOpenned()
{
    return openned;
}

int MixerManager::mixerCount()
{
    return mixers.count();
}

QList<DWORD> MixerManager::mixerHandles()
{
    QList<DWORD> mixs;
    for (MixerHandle mix : mixers)
        mixs.append(mix.handle);
    return mixs;
}

DWORD MixerManager::mixerHandle(int mix)
{
    if (openned && isInMixer(mix, 0))
        return mixers[mix].handle;
}

QList<DWORD> MixerManager::busHandles(int mix)
{
    QList<DWORD> buss;

    if (isInMixer(mix, 0))
    {
        for (int b=0; b<16; b++)
            buss.append(mixers[mix].bus[b]);
    }

    return buss;
}

DWORD MixerManager::busHandle(int mix, int bus)
{
    if (openned && isInMixer(mix, bus))
        return mixers[mix].bus[bus];
    else
        return 0;
}

QList<Equalizer31BandFX *> MixerManager::mixerEqualizers()
{
    QList<Equalizer31BandFX *> eqs;

    for (MixerHandle mixer : mixers)
        eqs.append(mixer.eq);

    return eqs;
}

Equalizer31BandFX *MixerManager::mixerEqualizer(int mix)
{
    if (isInMixer(mix, 0))
        return mixers[mix].eq;
    else
        return nullptr;
}

QList<ReverbFX *> MixerManager::mixerReverbs()
{
    QList<ReverbFX *> rvs;

    for (MixerHandle mixer : mixers)
        rvs.append(mixer.reverb);

    return rvs;
}

ReverbFX *MixerManager::mixerReverb(int mix)
{
    if (isInMixer(mix, 0))
        return mixers[mix].reverb;
    else
        return nullptr;
}

void MixerManager::moveChannel(DWORD stream, int mix, int bus, DWORD speakerFlag)
{
    if (!openned)
        return;

    BASS_Mixer_ChannelRemove(stream);

    if (bus == -1)
        BASS_Mixer_StreamAddChannel(mixers[mix].handle, stream, speakerFlag);
    else if (bus < 16)
        BASS_Mixer_StreamAddChannel(mixers[mix].bus[bus], stream, speakerFlag);
}

bool MixerManager::addFX(DWORD stream, int mix, int bus, FX *fx)
{
    if (isInMixer(mix, bus))
    {
        mixers[mix].fxs[bus].append(fx);
        return true;
    }
    else
        return false;
}

bool MixerManager::isInMixer(int mix, int bus)
{
    if (mix < 0 || mix >= mixers.count())
        return false;

    if (bus < 0 || bus >= 16)
        return false;

    return true;
}
