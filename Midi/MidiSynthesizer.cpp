#include "MidiSynthesizer.h"

#include "BASSFX/AutoWahFX.h"
#include "BASSFX/CompressorFX.h"
#include "BASSFX/DistortionFX.h"
#include "BASSFX/EchoFX.h"
#include "BASSFX/Equalizer15BandFX.h"
#include "BASSFX/VSTFX.h"

#include <cstring>

#include <QDebug>


MidiSynthesizer::MidiSynthesizer(QObject *parent) : QObject(parent)
{   
    // create mixers
    for (int dv : outDevices.keys())
    {
        MixerHandle mixer;
        mixer.handle = 0;

        mixer.eq = new Equalizer31BandFX();
        mixer.reverb = new ReverbFX();
        mixer.chorus = new ChorusFX();

        mixers.append(mixer);
    }

    for (int i=0; i<128; i++) {
        instmSf.append(0);
    }

    for (int i=0; i<16; i++) {
        drumSf.append(0);
    }

    BASS_VST_INFO vstinfo;
    for (int i=0; i<4; i++)
    {
        mVstiFiles[i] = "";
        mVstiInfos[i] = vstinfo;
        mVstiTempProgram[i] = 0;
        mVstiTempParams[i] = QList<float>();
    }


    // Map inst
    for (int i=0; i<HANDLE_STREAM_COUNT; i++)
    {
        InstrumentType t = static_cast<InstrumentType>(i);
        Instrument im;
        im.type = t;
        im.mute = false;
        im.solo = false;
        im.enable = true;
        im.volume = 50;
        im.bus = -1;
        im.vsti = -1;
        im.device = 0;
        im.speaker = SpeakerType::Default;

        instMap[t] = im;

        handles[t] = 0;
    }

    for (int i=0; i<16; i++)
    {
        chInstType[i] = InstrumentType::Piano;
    }
    chInstType[9] = InstrumentType::PercussionEtc;
}

MidiSynthesizer::~MidiSynthesizer()
{
    if (openned)
        close();

    // Free soundfont
    for (HSOUNDFONT f : synth_HSOUNDFONT) {
        BASS_MIDI_FontUnload(f, -1, -1);
        BASS_MIDI_FontFree(f);
    }
    synth_HSOUNDFONT.clear();

    // free instruments fx
    for (Instrument inst : instMap.values())
    {
        for (FX *fx : inst.FXs)
            delete fx;
    }


    // free mixer fx
    for (MixerHandle mixer : mixers)
    {
        delete mixer.eq;
        delete mixer.reverb;
        delete mixer.chorus;
    }
    mixers.clear();

    // --------------

    instMap.clear();

    sfFiles.clear();
    instmSf.clear();
    drumSf.clear();
}

bool MidiSynthesizer::open()
{
    if (openned)
        return true;

    openned = true;

    DWORD f = useFloat ? BASS_SAMPLE_FLOAT : 0;

    // create mixer, bus
    for (int i=0; i<mixers.count(); i++)
    {
        MixerHandle mixer = mixers[i];
        mixer.handle = BASS_Mixer_StreamCreate(44100, 2, f);
        mixer.eq->setStreamHandle(mixer.handle);
        mixer.reverb->setStreamHandle(mixer.handle);
        mixer.chorus->setStreamHandle(mixer.handle);

        DWORD device = (i == 0) ? defaultDev : outDevices.keys()[i];
        BASS_ChannelSetDevice(mixer.handle, device);
        BASS_ChannelPlay(mixer.handle, false);

        mixers[i] = mixer;
    }

    // create midi and bus stream
    for (int i=0; i<HANDLE_STREAM_COUNT; i++)
    {
        InstrumentType t = static_cast<InstrumentType>(i);
        handles[t] = createStream(t);
    }

    // Check device.. volume .. mute.. solo.. bus.. and VST
    for (int i=0; i<HANDLE_STREAM_COUNT; i++)
    {
        InstrumentType t = static_cast<InstrumentType>(i);
        setDevice(t, instMap[t].device);
        setVolume(t, instMap[t].volume);
        setMute(t, instMap[t].mute);
        setSolo(t, instMap[t].solo);
        setBusGroup(t, instMap[t].bus);  // speaker will set here

        // Set fx to stream handle
        for (FX *fx : instMap[t].FXs)
            fx->setStreamHandle(handles[t]);
    }

    setSfToStream();
    setMapSoundfontIndex(instmSf, drumSf);
    setVolume(synth_volume);

    return true;
}

void MidiSynthesizer::close()
{
    if (!openned)
        return;

    // Clear FX
    for (InstrumentType t : instMap.keys())
    {
        if (t >= InstrumentType::BusGroup1)
            break;

        for (FX *fx : instMap[t].FXs) {
            fx->setStreamHandle(0);
        }
    }

    // clear handles
    for (InstrumentType t: handles.keys())
    {
        if (t >= InstrumentType::BusGroup1)
            break;

        HSTREAM h = handles[t];

        if (t==InstrumentType::VSTi1 || t==InstrumentType::VSTi2
         || t==InstrumentType::VSTi3 || t==InstrumentType::VSTi4)
        {
            int vIndex = static_cast<int>(t) - HANDLE_VSTI_START;
            mVstiTempProgram[vIndex] = BASS_VST_GetProgram(h);
            mVstiTempParams[vIndex] = FX::getVSTParams(h);
            BASS_VST_ChannelFree(h);
        }
        else
            BASS_StreamFree(h);

        handles[t] = 0;
    }

    // clear mixers fx
    for (int i=0; i<mixers.count(); i++)
    {
        MixerHandle mixer = mixers[i];
        mixer.eq->setStreamHandle(0);
        mixer.reverb->setStreamHandle(0);
        mixer.chorus->setStreamHandle(0);

        BASS_ChannelStop(mixer.handle);
        BASS_StreamFree(mixer.handle);

        mixer.handle = 0;
        mixers[i] = mixer;
    }

    // compact soundfont
    BASS_MIDI_FontCompact(0);

    openned = false;
}

int MidiSynthesizer::defaultDevice()
{
    return defaultDev;
}

bool MidiSynthesizer::setDefaultDevice(int dv)
{
    if (openned)
    {
        DWORD mix = mixers[0].handle;
        BASS_ChannelStop(mix);
        bool rs = BASS_ChannelSetDevice(mix, dv);
        BASS_ChannelPlay(mix, false);

        if (rs)
        {
            defaultDev = dv;
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        if (dv < audioDevices().count())
        {
            defaultDev = dv;
            return true;
        }
        else
            return false;
    }
}

void MidiSynthesizer::setVolume(float vol)
{
    synth_volume = vol;
    for (MixerHandle mix : mixers)
        BASS_ChannelSetAttribute(mix.handle, BASS_ATTRIB_VOL, vol);
}

bool MidiSynthesizer::addSoundfont(const QString &sfFile)
{
    #ifdef _WIN32
    HSOUNDFONT sf = BASS_MIDI_FontInit(sfFile.toStdWString().c_str(), BASS_MIDI_FONT_NOFX);
    #else
    HSOUNDFONT sf = BASS_MIDI_FontInit(sfFile.toStdString().c_str(), BASS_MIDI_FONT_NOFX);
    #endif

    if (!sf)
        return false;

    this->sfFiles.append(sfFile);

    if (sfLoadAll)
        BASS_MIDI_FontLoad(sf, -1, -1);
    BASS_MIDI_FontCompact(sf);

    synth_HSOUNDFONT.push_back(sf);

    if (!openned)
        return true;

    if (synth_HSOUNDFONT.size() == 1)
        setSfToStream();

    return true;
}

void MidiSynthesizer::removeSoundfont(int sfIndex)
{
    if (sfIndex < 0 || sfIndex >= synth_HSOUNDFONT.count())
        return;

    QList<int> instSfMap = instmSf;
    QList<int> drumSfMap = drumSf;

    for (int i=0; i<instSfMap.count(); i++)
    {
        if (instSfMap[i] == sfIndex)
            instSfMap[i] = 0;
        else if (instSfMap[i] > sfIndex)
            instSfMap[i] = instSfMap[i] - 1;
    }

    for (int i=0; i<drumSfMap.count(); i++)
    {
        if (drumSfMap[i] == sfIndex)
            drumSfMap[i] = 0;
        else if (drumSfMap[i] > sfIndex)
            drumSfMap[i] = drumSfMap[i] - 1;
    }

    HSOUNDFONT sf = synth_HSOUNDFONT.takeAt(sfIndex);
    BASS_MIDI_FontUnload(sf, -1, -1);
    BASS_MIDI_FontFree(sf);
    sfFiles.removeAt(sfIndex);

    setMapSoundfontIndex(instSfMap, drumSfMap);
}

void MidiSynthesizer::swapSoundfont(int sfIndex, int toIndex)
{
    if (sfIndex < 0 || sfIndex >= synth_HSOUNDFONT.count())
        return;

    if (toIndex < 0 || toIndex >= synth_HSOUNDFONT.count())
        return;

    QList<int> instSfMap = instmSf;
    QList<int> drumSfMap = drumSf;

    for (int i=0; i<instSfMap.count(); i++)
    {
        if (instSfMap[i] == 0)
            continue;
        if (instSfMap[i] == sfIndex)
            instSfMap[i] = toIndex;
    }

    for (int i=0; i<drumSfMap.count(); i++)
    {
        if (drumSfMap[i] == 0)
            continue;
        if (drumSfMap[i] == sfIndex)
            drumSfMap[i] = toIndex;
    }

    synth_HSOUNDFONT.swap(sfIndex, toIndex);
    sfFiles.swap(sfIndex, toIndex);

    setMapSoundfontIndex(instSfMap, drumSfMap);
}

float MidiSynthesizer::soundfontVolume(int sfIndex)
{
    if (sfIndex < 0 || sfIndex >= synth_HSOUNDFONT.count())
        return -1;

    return BASS_MIDI_FontGetVolume(synth_HSOUNDFONT[sfIndex]);
}

void MidiSynthesizer::setSoundfontVolume(int sfIndex, float sfvl)
{
    if (sfIndex < 0 || sfIndex >= synth_HSOUNDFONT.count())
        return;

    BASS_MIDI_FontSetVolume(synth_HSOUNDFONT[sfIndex], sfvl);
}

void MidiSynthesizer::compactSoundfont()
{
    BASS_MIDI_FontCompact(0);
}

void MidiSynthesizer::setLoadAllSoundfont(bool loadAll)
{
    if (loadAll == sfLoadAll)
        return;

    sfLoadAll = loadAll;

    for (HSOUNDFONT sf : synth_HSOUNDFONT)
    {
        if (sfLoadAll)
            BASS_MIDI_FontLoad(sf, -1, -1);
        BASS_MIDI_FontCompact(sf);
    }
}

bool MidiSynthesizer::setMapSoundfontIndex(QList<int> intrumentSfIndex, QList<int> drumSfIndex)
{
    instmSf.clear();
    drumSf.clear();
    instmSf = intrumentSfIndex;
    drumSf = drumSfIndex;

    if (intrumentSfIndex.count() < 128 || synth_HSOUNDFONT.count() == 0 || !openned)
        return false;


    std::vector<BASS_MIDI_FONT> mFonts;

    // check instrument use another sf
    for (int i=0; i<128; i++)
    {
        if (instmSf.at(i) <= 0)
            continue;

        if (instmSf.at(i) >= synth_HSOUNDFONT.count())
        {
            instmSf[i] = 0;
            continue;
        }

        BASS_MIDI_FONT font;
        font.font = synth_HSOUNDFONT.at(instmSf.at(i));
        font.preset = i;
        font.bank = 0;

        mFonts.push_back(font);
    }

    // defaut sf
    BASS_MIDI_FONT f;
    f.font = synth_HSOUNDFONT.at(0);
    f.preset = -1;
    f.bank = 0;

    mFonts.push_back(f);

    // set to stream
    for (int i=0; i<HANDLE_MIDI_COUNT-4; i++) {
        HSTREAM h = handles[static_cast<InstrumentType>(i)];
        BASS_MIDI_StreamSetFonts(h, mFonts.data(), mFonts.size());
    }


    // check drum sf
    int startDrum = static_cast<int>(InstrumentType::BassDrum);
    int li = 0;
    for (int i=startDrum; i<HANDLE_MIDI_COUNT-4; i++)
    {
        if (drumSf.at(li) < 0 || drumSf.at(li) >= synth_HSOUNDFONT.count())
            drumSf[li] = 0;

        BASS_MIDI_FONT font;
        font.font = synth_HSOUNDFONT.at(drumSf.at(li));
        font.preset = -1;
        font.bank = 0;

        InstrumentType type = static_cast<InstrumentType>(i);
        BASS_MIDI_StreamSetFonts(handles[type], &font, 1);

        li++;
    }

    return true;
}

void MidiSynthesizer::sendNoteOff(int ch, int note, int velocity)
{
    if (note < 0 || note > 127)
        return;

    if (ch == 9)
    {
        int vstiIndex = instMap[MidiHelper::getInstrumentDrumType(note)].vsti;
        if (vstiIndex == -1)
            BASS_MIDI_StreamEvent(getDrumHandleFromNote(note), 9, MIDI_EVENT_NOTE, MAKEWORD(note, 0));
        else
        {
            InstrumentType t = static_cast<InstrumentType>(vstiIndex + HANDLE_VSTI_START);
            BASS_VST_ProcessEvent(handles[t], 9, MIDI_EVENT_NOTE, MAKEWORD(note, 0));
        }
    }
    else
    {
        int vstiIndex = instMap[chInstType[ch]].vsti;
        if (vstiIndex == -1)
            BASS_MIDI_StreamEvent(handles[chInstType[ch]], ch, MIDI_EVENT_NOTE, MAKEWORD(note, 0));
        else
        {
            InstrumentType t = static_cast<InstrumentType>(vstiIndex + HANDLE_VSTI_START);
            BASS_VST_ProcessEvent(handles[t], ch, MIDI_EVENT_NOTE, MAKEWORD(note, 0));
        }
    }
}

void MidiSynthesizer::sendNoteOn(int ch, int note, int velocity)
{
    if (note < 0 || note > 127)
        return;

    if (ch == 9)
    {
        int vstiIndex = instMap[MidiHelper::getInstrumentDrumType(note)].vsti;
        if (vstiIndex == -1)
        {
            InstrumentType t = MidiHelper::getInstrumentDrumType(note);
            BASS_MIDI_StreamEvent(handles[t], 9, MIDI_EVENT_NOTE, MAKEWORD(note, velocity));
            emit noteOnSended(t, instMap[t].bus, 9, note, velocity);
        }
        else
        {
            InstrumentType t = static_cast<InstrumentType>(vstiIndex + HANDLE_VSTI_START);
            BASS_VST_ProcessEvent(handles[t], 9, MIDI_EVENT_NOTE, MAKEWORD(note, velocity));
            emit noteOnSended(t, instMap[t].bus, 9, note, velocity);
        }
    }
    else
    {
        int vstiIndex = instMap[chInstType[ch]].vsti;
        if (vstiIndex == -1)
        {
            InstrumentType t = chInstType[ch];
            BASS_MIDI_StreamEvent(handles[t], ch, MIDI_EVENT_NOTE, MAKEWORD(note, velocity));
            emit noteOnSended(t, instMap[t].bus, ch, note, velocity);
        }
        else
        {
            InstrumentType t = static_cast<InstrumentType>(vstiIndex + HANDLE_VSTI_START);
            BASS_VST_ProcessEvent(handles[t], ch, MIDI_EVENT_NOTE, MAKEWORD(note, velocity));
            emit noteOnSended(t, instMap[t].bus, ch, note, velocity);
        }
    }
}

void MidiSynthesizer::sendNoteAftertouch(int ch, int note, int value)
{
    if (note < 0 || note > 127)
        return;

    if (ch == 9)
    {
        int vstiIndex = instMap[MidiHelper::getInstrumentDrumType(note)].vsti;
        if (vstiIndex == -1)
            BASS_MIDI_StreamEvent(getDrumHandleFromNote(note), 9, MIDI_EVENT_KEYPRES, MAKEWORD(note, value));
        else
        {
            InstrumentType t = static_cast<InstrumentType>(vstiIndex + HANDLE_VSTI_START);
            BASS_VST_ProcessEvent(handles[t], 9, MIDI_EVENT_KEYPRES, MAKEWORD(note, value));
        }
    }
    else
    {
        int vstiIndex = instMap[chInstType[ch]].vsti;
        if (vstiIndex == -1)
            BASS_MIDI_StreamEvent(handles[chInstType[ch]], ch, MIDI_EVENT_KEYPRES, MAKEWORD(note, value));
        else
        {
            InstrumentType t = static_cast<InstrumentType>(vstiIndex + HANDLE_VSTI_START);
            BASS_VST_ProcessEvent(handles[t], ch, MIDI_EVENT_KEYPRES, MAKEWORD(note, value));
        }
    }
}

void MidiSynthesizer::sendController(int ch, int number, int value)
{
    DWORD et;

    switch (number) {
    case 0:
        et = MIDI_EVENT_BANK; break;
    case 1:
        et = MIDI_EVENT_MODULATION; break;
    case 5:
        et = MIDI_EVENT_PORTATIME; break;
        /*
    case 6:
    case 38:
        if (RPNType == 0)
            return;
        et = RPNType;
        break;
        */
    case 7:
        et = MIDI_EVENT_VOLUME; break;
    case 10:
        et = MIDI_EVENT_PAN; break;
    case 11:
        et = MIDI_EVENT_EXPRESSION; break;
    case 32:
        et = MIDI_EVENT_BANK_LSB; break;
    case 64:
        et = MIDI_EVENT_SUSTAIN; break;
    case 65:
        et = MIDI_EVENT_PORTAMENTO; break;
    case 66:
        et = MIDI_EVENT_SOSTENUTO; break;
    case 67:
        et = MIDI_EVENT_SOFT; break;
    case 71:
        et = MIDI_EVENT_RESONANCE; break;
    case 72:
        et = MIDI_EVENT_RELEASE; break;
    case 73:
        et = MIDI_EVENT_ATTACK; break;
    case 74:
        et = MIDI_EVENT_CUTOFF; break;
    case 75:
        et = MIDI_EVENT_DECAY; break;
    case 84:
        et = MIDI_EVENT_PORTANOTE; break;
    case 91:
        et = MIDI_EVENT_REVERB; break;
    case 93:
        et = MIDI_EVENT_CHORUS; break;
    case 94:
        et = MIDI_EVENT_USERFX; break;
        /*
    case 100:
    case 101:
        if (value == 127) {
            et = MIDI_EVENT_PITCHRANGE;
            break;
        }
        else {
            switch (value) {
            case 0: RPNType = MIDI_EVENT_PITCHRANGE; break;
            case 1: RPNType = MIDI_EVENT_FINETUNE; break;
            case 2: RPNType = MIDI_EVENT_COARSETUNE; break;
            default: RPNType = 0;  qDebug() << " Value " << value; break;
            }
            return;
        }
        */
    case 120:
        et = MIDI_EVENT_SOUNDOFF; break;
    case 121:
        et = MIDI_EVENT_RESET; break;
    case 123:
        et = MIDI_EVENT_NOTESOFF; break;
    case 126:
    case 127:
        et = MIDI_EVENT_MODE; break;
    default:
        if (ch < 0 || ch > 15)
            return;
        BYTE data[3] = { (0xB0 | ch), (number & 0x7F), (value & 0x7F) };
        //qDebug() << (data[0] & 0xF0) << "  " << (data[0] & 0x0F) << "  " << data[1] << "  " << data[2];
        for (int i=0; i<HANDLE_MIDI_COUNT; i++) {
            HSTREAM h = handles[static_cast<InstrumentType>(i)];
            if (i < HANDLE_VSTI_START)
                BASS_MIDI_StreamEvents(h, BASS_MIDI_EVENTS_RAW, data, 3);
            else
                BASS_VST_ProcessEventRaw(h, (void*)data, 3);
        }
        return;
    }

    sendToAllMidiStream(ch, et, value);
}

void MidiSynthesizer::sendProgramChange(int ch, int number)
{
    sendToAllMidiStream(ch, MIDI_EVENT_PROGRAM, number);

    if (ch != 9) {
        InstrumentType t = MidiHelper::getInstrumentType(number);
        chInstType[ch] = t;
    }
}

void MidiSynthesizer::sendChannelAftertouch(int ch, int value)
{
    sendToAllMidiStream(ch, MIDI_EVENT_CHANPRES, value);
}

void MidiSynthesizer::sendPitchBend(int ch, int value)
{
    if (ch == 9)
        sendToAllMidiStream(ch, MIDI_EVENT_PITCH, value);
    else
    {
        int vstiIndex = instMap[chInstType[ch]].vsti;
        if (vstiIndex == -1)
            BASS_MIDI_StreamEvent(handles[chInstType[ch]], ch, MIDI_EVENT_PITCH, value);
        else
        {
            InstrumentType t = static_cast<InstrumentType>(vstiIndex + HANDLE_VSTI_START);
            BASS_VST_ProcessEvent(handles[t], ch, MIDI_EVENT_PITCH, value);
        }
    }
}

void MidiSynthesizer::sendAllNotesOff(int ch)
{
    sendToAllMidiStream(ch, MIDI_EVENT_NOTESOFF, 0);
}

void MidiSynthesizer::sendAllNotesOff()
{
    for (int i=0; i<16; i++) {
        sendAllNotesOff(i);
    }
}

void MidiSynthesizer::sendResetAllControllers(int ch)
{
    sendToAllMidiStream(ch, MIDI_EVENT_RESET, 0);
    sendToAllMidiStream(ch, MIDI_EVENT_PITCHRANGE, 2);
}

void MidiSynthesizer::sendResetAllControllers()
{
    for (int i=0; i<16; i++) {
        sendResetAllControllers(i);
    }
}

int MidiSynthesizer::device(InstrumentType t)
{
    return instMap[t].device;
}

int MidiSynthesizer::busGroup(InstrumentType t)
{
    return instMap[t].bus;
}

int MidiSynthesizer::volume(InstrumentType t)
{
    return instMap[t].volume;
}

bool MidiSynthesizer::isMute(InstrumentType t)
{
    return instMap[t].mute;
}

bool MidiSynthesizer::isSolo(InstrumentType t)
{
    return instMap[t].solo;
}

int MidiSynthesizer::useVSTi(InstrumentType t)
{
    return instMap[t].vsti;
}

SpeakerType MidiSynthesizer::speaker(InstrumentType t)
{
    return instMap[t].speaker;
}

void MidiSynthesizer::setDevice(InstrumentType t, int device)
{
    if (device < 0 || device >= audioDevices().count())
        return;

    if (t < InstrumentType::BusGroup1 && instMap[t].bus != -1)
        return;

    instMap[t].device = device;

    if (!openned)
        return;

    if (t < InstrumentType::BusGroup1 && instMap[t].bus != -1)
        return;

    DWORD flag = MidiHelper::getSpeakerFlag(instMap[t].speaker);
    BASS_Mixer_ChannelRemove(handles[t]);
    BASS_Mixer_StreamAddChannel(mixers[device].handle, handles[t], flag);
}

void MidiSynthesizer::setBusGroup(InstrumentType t, int group)
{
    if (t >= InstrumentType::BusGroup1)
        return;

    instMap[t].bus = group;

    if (!openned)
        return;

    DWORD flag = MidiHelper::getSpeakerFlag(instMap[t].speaker);

    BASS_Mixer_ChannelRemove(handles[t]);

    if (group == -1)
    {
        DWORD mix = mixers[instMap[t].device].handle;
        BASS_Mixer_StreamAddChannel(mix, handles[t], flag);
    }
    else
    {
        InstrumentType busType = static_cast<InstrumentType>(group + HANDLE_BUS_START);
        BASS_Mixer_StreamAddChannel(handles[busType], handles[t], flag);
    }
}

void MidiSynthesizer::setVolume(InstrumentType t, int volume)
{
    if (volume > 100)
        instMap[t].volume = 100;
    else if (volume < 0)
        instMap[t].volume = 0;
    else
        instMap[t].volume = volume;

    if (!openned)
        return;

    if (!instMap[t].enable)
        return;

    BASS_ChannelSetAttribute(handles[t], BASS_ATTRIB_VOL, instMap[t].volume / 100.0f);
}

void MidiSynthesizer::setMute(InstrumentType t, bool m)
{
    instMap[t].mute = m;
    calculateEnable();

    if (!openned)
        return;

    if (m)
        BASS_ChannelSetAttribute(handles[t], BASS_ATTRIB_VOL, 0.0f);
    else
        BASS_ChannelSetAttribute(handles[t], BASS_ATTRIB_VOL, instMap[t].volume / 100.0f);
}

void MidiSynthesizer::setSolo(InstrumentType t, bool s)
{
    instMap[t].solo = s;

    bool us = false;

    for (const Instrument &im : instMap.values()) {
        if (im.solo) {
            us = true;
            break;
        }
    }
    useSolo = us;

    calculateEnable();

    if (!openned)
        return;

    for (const Instrument itr : instMap.values()) {

        if (itr.enable)
            BASS_ChannelSetAttribute(handles[itr.type], BASS_ATTRIB_VOL, itr.volume / 100.0f);
        else
            BASS_ChannelSetAttribute(handles[itr.type], BASS_ATTRIB_VOL, 0.0f);
    }
}

void MidiSynthesizer::setUseVSTi(InstrumentType t, int vstiIndex)
{
    if (t >= InstrumentType::BusGroup1)
        return;

    if (vstiIndex < -1 || vstiIndex > 4)
        return;

    int oldVstiIndex = instMap[t].vsti;
    instMap[t].vsti = vstiIndex;

    if (oldVstiIndex != -1 && vstiHandle(oldVstiIndex) != 0)
    {
        InstrumentType type = static_cast<InstrumentType>(vstiIndex + HANDLE_VSTI_START);
        for (int ch=0; ch<16; ch++)
            BASS_VST_ProcessEvent(handles[type], ch, MIDI_EVENT_NOTESOFF, 0);
    }
}

void MidiSynthesizer::setSpeaker(InstrumentType t, SpeakerType speaker)
{
    if (t >= InstrumentType::BusGroup1)
        return;

    instMap[t].speaker = speaker;

    if (!openned)
        return;

    BASS_Mixer_ChannelRemove(handles[t]);
    BASS_VST_ChannelFree(handles[t]);
    BASS_StreamFree(handles[t]);

    handles[t] = createStream(t);

    // Check device.. volume .. mute.. solo.. bus.. and VST
    setDevice(t, instMap[t].device);
    setVolume(t, instMap[t].volume);
    setMute(t, instMap[t].mute);
    setSolo(t, instMap[t].solo);
    setBusGroup(t, instMap[t].bus); // speaker will set in here

    // Set fx to stream handle
    for (FX *fx : instMap[t].FXs)
        fx->setStreamHandle(handles[t]);

    setMapSoundfontIndex(instmSf, drumSf);
}

QStringList MidiSynthesizer::audioDevices()
{
    return QStringList(outDevices.values());
}

void MidiSynthesizer::audioDevices(const QMap<int, QString> &devices)
{
    outDevices = devices;
}

bool MidiSynthesizer::isSoundFontFile(const QString &sfile)
{
    bool result = true;

    #ifdef _WIN32
    HSOUNDFONT f = BASS_MIDI_FontInit(sfile.toStdWString().c_str(), 0);
    #else
    HSOUNDFONT f = BASS_MIDI_FontInit(sfile.toStdString().c_str(), 0);
    #endif

    if (!f)
        result = false;

    BASS_MIDI_FontFree(f);

    return result;
}

QList<Equalizer31BandFX *> MidiSynthesizer::equalizer31BandFXs()
{
    QList<Equalizer31BandFX *> eqs;

    for (MixerHandle mix : mixers)
        eqs.append(mix.eq);

    return eqs;
}

QList<ReverbFX *> MidiSynthesizer::reverbFXs()
{
    QList<ReverbFX *> rvs;

    for (MixerHandle mix : mixers)
        rvs.append(mix.reverb);

    return rvs;
}

QList<ChorusFX *> MidiSynthesizer::chorusFXs()
{
    QList<ChorusFX *> chs;

    for (MixerHandle mix : mixers)
        chs.append(mix.chorus);

    return chs;
}

void MidiSynthesizer::setUsetFloattingPoint(bool use)
{
    if (use == useFloat)
        return;

    useFloat = use;

    QList<int> isf = instmSf;
    QList<int> dsf = drumSf;

    close();
    open();

    setMapSoundfontIndex(isf, dsf);
}

void MidiSynthesizer::setUseFXRC(bool use)
{
    if (use == useFX)
        return;

    useFX = use;

    if (useFX) {
        for (int i=0; i<HANDLE_MIDI_COUNT; i++) {
            HSTREAM h = handles[static_cast<InstrumentType>(i)];
            BASS_ChannelFlags(h, 0, BASS_MIDI_NOFX); // remove the flag
        }
    } else {
        for (int i=0; i<HANDLE_MIDI_COUNT; i++) {
            HSTREAM h = handles[static_cast<InstrumentType>(i)];
            BASS_ChannelFlags(h, BASS_MIDI_NOFX, BASS_MIDI_NOFX); // set flag
        }
    }
}

HSTREAM MidiSynthesizer::getChannelHandle(InstrumentType type)
{
    return handles[type];
}

FX *MidiSynthesizer::addFX(InstrumentType type, DWORD uid)
{
    FX *fx = nullptr;

    if (uid < BUILTIN_FX_COUNT)
    {
        FXType fxType = static_cast<FXType>(uid);
        switch (fxType) {
        case FXType::AutoWah:
            fx = new AutoWahFX(handles[type], instMap[type].FXs.count());
            break;
        case FXType::Chorus:
            fx = new ChorusFX(handles[type], instMap[type].FXs.count());
            break;
        case FXType::Compressor:
            fx = new CompressorFX(handles[type], instMap[type].FXs.count());
            break;
        case FXType::Distortion:
            fx = new DistortionFX(handles[type], instMap[type].FXs.count());
            break;
        case FXType::Echo:
            fx = new EchoFX(handles[type], instMap[type].FXs.count());
            break;
        case FXType::EQ15Band:
            fx = new Equalizer15BandFX(handles[type], instMap[type].FXs.count());
            break;
        case FXType::EQ31Band:
            fx = new Equalizer31BandFX(handles[type], instMap[type].FXs.count());
            break;
        case FXType::Reverb:
            fx = new ReverbFX(handles[type], instMap[type].FXs.count());
            break;
        }
    }
    else
    {
        #ifndef __linux__
        if (_vstList.contains(uid))
            fx = new VSTFX(_vstList[uid].vstPath, handles[type], instMap[type].FXs.count());
        else
            fx = nullptr;
        #endif
    }

    if (fx != nullptr)
    {
        fx->setBypass(false);
        instMap[type].FXs.append(fx);
    }

    return fx;
}

bool MidiSynthesizer::removeFX(InstrumentType type, int fxIndex)
{
    if (fxIndex >= instMap[type].FXs.count())
        return false;

    delete instMap[type].FXs.takeAt(fxIndex);

    return true;
}

void MidiSynthesizer::setFXBypass(InstrumentType type, int fxIndex, bool state)
{

    if (fxIndex >= instMap[type].FXs.count())
        return;

    instMap[type].FXs[fxIndex]->setBypass(state);
}

QList<uint> MidiSynthesizer::fxUids(InstrumentType type)
{
    QList<uint> uids;

    for (FX *fx : instMap[type].FXs)
        uids.append(fx->uids());

    return uids;
}

QList<bool> MidiSynthesizer::fxBypass(InstrumentType type)
{
    QList<bool> bypass;

    for (FX *fx : instMap[type].FXs)
        bypass.append(fx->isBypass());

    return bypass;
}

QList<int> MidiSynthesizer::fxProgram(InstrumentType type)
{
    QList<int> programs;

    for (FX *fx : instMap[type].FXs)
        programs.append(fx->program());

    return programs;
}

QList<QList<float> > MidiSynthesizer::fxParams(InstrumentType type)
{
    QList<QList<float>> params;

    for (FX *fx : instMap[type].FXs)
        params.append(fx->params());

    return params;
}

#ifndef __linux__

BASS_VST_INFO MidiSynthesizer::vstiInfo(int vstiIndex)
{
    if (vstiIndex < 0 || vstiIndex > 3)
        return BASS_VST_INFO();
    else
        return mVstiInfos[vstiIndex];
}

QStringList MidiSynthesizer::vstiFiles()
{
    return QStringList() << mVstiFiles[0] << mVstiFiles[1]
            << mVstiFiles[2] << mVstiFiles[3];
}

QString MidiSynthesizer::vstiFile(int vstiIndex)
{
    if (vstiIndex < 0 || vstiIndex > 3)
        return "";
    else
        return mVstiFiles[vstiIndex];
}

DWORD MidiSynthesizer::vstiHandle(int vstiIndex)
{
    InstrumentType t = static_cast<InstrumentType>(HANDLE_VSTI_START+vstiIndex);
    return handles[t];
}

int MidiSynthesizer::vstiProgram(int vstiIndex)
{
    DWORD h = vstiHandle(vstiIndex);
    if (isOpened() && h != 0)
        return BASS_VST_GetProgram(h);
    else
        return mVstiTempProgram[vstiIndex];
}

QList<float> MidiSynthesizer::vstiParams(int vstiIndex)
{
    DWORD h = vstiHandle(vstiIndex);
    if (isOpened() && h != 0)
        FX::getVSTParams(h);
    else
        return mVstiTempParams[vstiIndex];
}

DWORD MidiSynthesizer::setVSTiFile(int vstiIndex, const QString &file)
{
    if (vstiIndex < 0 || vstiIndex > 3)
        return 0;

    InstrumentType t = static_cast<InstrumentType>(HANDLE_VSTI_START+vstiIndex);
    DWORD vsti = handles[t];

    if (file == mVstiFiles[vstiIndex])
        return vsti;

    mVstiFiles[vstiIndex] = file;

    if (!openned)
        return 0;

    BASS_Mixer_ChannelRemove(vsti);
    BASS_VST_ChannelFree(vsti);

    vsti = createStream(t);

    if (vsti)
    {
        BASS_VST_INFO vstinfo;
        BASS_VST_GetInfo(vsti, &vstinfo);
        mVstiInfos[vstiIndex] = vstinfo;
        mVstiTempProgram[vstiIndex] = 0;
        mVstiTempParams[vstiIndex].clear();

        handles[t] = vsti;

        setDevice(t, instMap[t].device);
        setVolume(t, instMap[t].volume);
        setMute(t, instMap[t].mute);
        setSolo(t, instMap[t].solo);
        setBusGroup(t, instMap[t].bus); // speaker will set here

        // Set stream handle to FX
        for (FX *fx : instMap[t].FXs)
            fx->setStreamHandle(vsti);

        return vsti;
    }
    else
    {
        handles[t] = 0;
        return 0;
    }
}

void MidiSynthesizer::removeVSTiFile(int vstiIndex)
{
    if (vstiIndex < 0 || vstiIndex > 3)
        return ;

    InstrumentType t = static_cast<InstrumentType>(HANDLE_VSTI_START+vstiIndex);
    DWORD vsti = handles[t];

    BASS_Mixer_ChannelRemove(vsti);
    BASS_VST_ChannelFree(vsti);

    handles[t] = 0;
    mVstiFiles[vstiIndex] = "";
    mVstiInfos[vstiIndex] = BASS_VST_INFO();
    mVstiTempProgram[vstiIndex] = 0;
    mVstiTempParams[vstiIndex].clear();
}

DWORD MidiSynthesizer::createStream(InstrumentType t)
{
    int index = static_cast<int>(t);

    DWORD f = useFloat ? BASS_SAMPLE_FLOAT : 0;

    if (index < HANDLE_MIDI_COUNT)  // Midi & VSTi stream
    {
        if (index < HANDLE_MIDI_COUNT-4) // Midi
        {
             DWORD flags = f|BASS_STREAM_DECODE|BASS_MIDI_SINCINTER;
             if (!useFX) flags = flags|BASS_MIDI_NOFX;
             if (!MidiHelper::isStereoSpeaker(instMap[t].speaker))
                 flags = flags|BASS_SAMPLE_MONO;

            return BASS_MIDI_StreamCreate(16, flags, 44100);
        }
        else // VSTi
        {
            #ifdef __linux__
            return 0;
            #else
            int vIndex = index - (HANDLE_MIDI_COUNT-4);
            int chan = MidiHelper::isStereoSpeaker(instMap[t].speaker) ? 2 : 1;
            if (mVstiFiles[vIndex] == "")
                return 0;
            #ifdef _WIN32
            DWORD h = BASS_VST_ChannelCreate(44100, chan, mVstiFiles[vIndex].toStdWString().c_str(),
                                       f|BASS_UNICODE|BASS_STREAM_DECODE);
            #else
            DWORD h = BASS_VST_ChannelCreate(44100, chan, mVstiFiles[vIndex].toStdString().c_str(),
                                       f|BASS_STREAM_DECODE);
            #endif
            if (h)
            {
                BASS_VST_INFO info;
                BASS_VST_GetInfo(h, &info);
                mVstiInfos[vIndex] = info;
                BASS_VST_SetProgram(h, mVstiTempProgram[vIndex]);
                FX::setVSTParams(h, mVstiTempParams[vIndex]);
            }
            return h; // vsti handle
            #endif
        }
    }
    else // bus stream
    {
        int chan = MidiHelper::isStereoSpeaker(instMap[t].speaker) ? 2 : 1;
        return BASS_Mixer_StreamCreate(44100, chan, f|BASS_STREAM_DECODE);
    }
}

#endif

void MidiSynthesizer::sendToAllMidiStream(int ch, DWORD eventType, DWORD param)
{
    for (int i=0; i<HANDLE_MIDI_COUNT; i++) {
        HSTREAM stream = handles[static_cast<InstrumentType>(i)];
        if (i < HANDLE_VSTI_START)
            BASS_MIDI_StreamEvent(stream, ch, eventType, param);
        else
            BASS_VST_ProcessEvent(stream, ch, eventType, param);
    }
}

void MidiSynthesizer::setSfToStream()
{
    if (synth_HSOUNDFONT.size() > 0)
    {
        BASS_MIDI_FONT font;
        font.font = synth_HSOUNDFONT.at(0);
        font.preset = -1;
        font.bank = 0;

        for (int i=0; i<HANDLE_MIDI_COUNT; i++) {
            HSTREAM h = handles[static_cast<InstrumentType>(i)];
            BASS_MIDI_StreamSetFonts(h, &font, 1); // set sf stream
        }
    }

    // Reset map intrument sf
    /*
    instmSf.clear();
    drumSf.clear();
    for (int i=0; i<128; i++) {
        instmSf.append(0);
    }
    for (int i=0; i<16; i++) {
        drumSf.append(0);
    }
    */
}

void MidiSynthesizer::calculateEnable()
{
    for (Instrument i : instMap.values()) {
        //Instrument i = im.second;
        if (useSolo) {
            if (i.mute)
                instMap[i.type].enable = false;
            else if (i.solo)
                instMap[i.type].enable = true;
            else
                instMap[i.type].enable = false;
        }
        else {
            if (i.mute)
                instMap[i.type].enable = false;
            else
                instMap[i.type].enable = true;
        }
    }
}

HSTREAM MidiSynthesizer::getDrumHandleFromNote(int drumNote)
{
    switch (drumNote) {

    // Bass drum
    case 35:
    case 36:
        return handles[InstrumentType::BassDrum];

    // snare
    case 38:
    case 40:
        return handles[InstrumentType::Snare];

    // Side Stick/Rimshot
    case 37:
        return handles[InstrumentType::SideStick];

    // Low Tom
    case 41:
    case 43:
        return handles[InstrumentType::LowTom];

    // Mid Tom
    case 45:
    case 47:
        return handles[InstrumentType::MidTom];

    // High Tom
    case 48:
    case 50:
        return handles[InstrumentType::HighTom];

    // Hi-hat
    case 42:
    case 44:
    case 46:
        return handles[InstrumentType::Hihat];

    // Cowbell
    case 56:
        return handles[InstrumentType::Cowbell];

    // Crash Cymbal ( 55 = Splash Cymbal )
    case 49:
    case 52:
    case 55:
    case 57:
        return handles[InstrumentType::CrashCymbal];

    // Ride Cymbal
    case 51:
    case 59:
        return handles[InstrumentType::RideCymbal];

    // Bongo
    case 60:
    case 61:
        return handles[InstrumentType::Bongo];

    // Conga
    case 62:
    case 63:
    case 64:
        return handles[InstrumentType::Conga];

    // Timbale
    case 65:
    case 66:
        return handles[InstrumentType::Timbale];

    // ฉิ่ง
    case 80:
    case 81:
        return handles[InstrumentType::SmallCupShapedCymbals];

    // ฉาบ
    case 82:
    case 83:
        return handles[InstrumentType::ThaiChap];

    default:
        return handles[InstrumentType::PercussionEtc];
    }
}

QMap<int, QString> MidiSynthesizer::outDevices;
