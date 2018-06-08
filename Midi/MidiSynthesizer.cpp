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
    for (int i=0; i<129; i++) {
        instmSf.append(0);
    }

    for (int i=0; i<16; i++) {
        drumSf.append(0);
    }

    eq = new Equalizer31BandFX(0);
    reverb = new ReverbFX(0);
    chorus = new ChorusFX(0);


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

    // Fx ------------
    delete eq;
    delete reverb;
    delete chorus;

    for (Instrument inst : instMap.values())
    {
        for (FX *fx : inst.FXs)
            delete fx;
    }

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

    // create mix stream
    DWORD f = useFloat ? BASS_SAMPLE_FLOAT : 0;
    mixHandle = BASS_Mixer_StreamCreate(44100, 2, f);
    BASS_ChannelSetDevice(mixHandle, outDev);

    DWORD flags = BASS_STREAM_DECODE|BASS_MIDI_SINCINTER;
    if (useFloat)
        flags = flags|BASS_SAMPLE_FLOAT;
    if (!useFX) {
        flags = flags|BASS_MIDI_NOFX;
    }

    // Create midi stream
    for (int i=0; i<HANDLE_MIDI_COUNT; i++)
    {
        InstrumentType t = static_cast<InstrumentType>(i);
        DWORD h = 0;

        if (i < HANDLE_MIDI_COUNT-4)
            h = BASS_MIDI_StreamCreate(16, flags, 0);
        else
        {
            #ifdef __linux__
            h = 0;
            #else
            int fIndex = i - (HANDLE_MIDI_COUNT-4);
            if (mVstiFiles[fIndex] == "")
                h = 0;
            else
            {
                #ifdef _WIN32
                h = BASS_VST_ChannelCreate(44100, 2, mVstiFiles[fIndex].toStdWString().c_str(),
                                           f|BASS_UNICODE|BASS_STREAM_DECODE);
                #else
                h = BASS_VST_ChannelCreate(44100, 2, mVstiFiles[fIndex].toStdString().c_str(),
                                           f|BASS_STREAM_DECODE);
                #endif
                if (f)
                {
                    BASS_VST_SetProgram(h, mVstiTempProgram[fIndex]);
                    FX::setVSTParams(h, mVstiTempParams[fIndex]);
                }
            }
            #endif
        }

        BASS_Mixer_StreamAddChannel(mixHandle, h, 0);

        handles[t] = h;
    }

    // Create bus stream
    for (int i=HANDLE_MIDI_COUNT; i<HANDLE_STREAM_COUNT; i++)
    {
        HSTREAM h = BASS_Mixer_StreamCreate(44100, 2, f|BASS_STREAM_DECODE);
        BASS_Mixer_StreamAddChannel(mixHandle, h, 0);

        InstrumentType t = static_cast<InstrumentType>(i);
        handles[t] = h;
    }


    // Check volume .. mute.. solo.. bus.. and VST
    for (int i=0; i<HANDLE_STREAM_COUNT; i++)
    {
        InstrumentType t = static_cast<InstrumentType>(i);
        setVolume(t, instMap[t].volume);
        setMute(t, instMap[t].mute);
        setSolo(t, instMap[t].solo);
        setBusGroup(t, instMap[t].bus);

        // Set stream handle to FX
        for (FX *fx : instMap[t].FXs)
        {
            fx->setStreamHandle(handles[t]);
        }
    }

    setSfToStream();

    bool checkDrumSfMap = true;
    for (int sfInex : instmSf) {
        if (sfInex > 0) {
            setMapSoundfontIndex(instmSf, drumSf);
            checkDrumSfMap = false;
            break;
        }
    }

    if (checkDrumSfMap) {
        for (int sfIndex : drumSf) {
            if (sfIndex > 0) {
                setMapSoundfontIndex(instmSf, drumSf);
                break;
            }
        }
    }

    // Set stream to Fx
    eq->setStreamHandle(mixHandle);
    reverb->setStreamHandle(mixHandle);
    chorus->setStreamHandle(mixHandle);

    BASS_ChannelPlay(mixHandle, false);

    return true;
}

void MidiSynthesizer::close(bool freeSF)
{
    if (!openned)
        return;

    BASS_ChannelStop(mixHandle);

    // Clear FX
    for (InstrumentType t : instMap.keys())
    {
        for (FX *fx : instMap[t].FXs) {
            fx->setStreamHandle(0);
        }
    }

    eq->setStreamHandle(0);
    reverb->setStreamHandle(0);
    chorus->setStreamHandle(0);

    for (InstrumentType t: handles.keys())
    {
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

    // Clear soundfont
    if (freeSF)
    {
        for (HSOUNDFONT f : synth_HSOUNDFONT)
        {
            BASS_MIDI_FontUnload(f, -1, -1);
            BASS_MIDI_FontFree(f);
        }
        synth_HSOUNDFONT.clear();
        sfLoaded = false;
    }

    BASS_StreamFree(mixHandle);

    openned = false;
}

int MidiSynthesizer::outPutDevice()
{
    return outDev;
}

bool MidiSynthesizer::setOutputDevice(int dv)
{
    if (openned)
    {
        if (BASS_ChannelSetDevice(mixHandle, dv))
        {
            outDev = dv;
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        if (dv < audioDevices().size())
        {
            outDev = dv;
            return true;
        }
        else
            return false;
    }
}

void MidiSynthesizer::setSoundFonts(QStringList &soundfonsFiles)
{
    this->sfFiles.clear();
    this->sfFiles = soundfonsFiles;

    if (openned)
    {
        sfLoaded = false;
        setSfToStream();
    }
}

void MidiSynthesizer::setVolume(float vol)
{
    if (BASS_ChannelSetAttribute(mixHandle, BASS_ATTRIB_VOL, vol)) {
        synth_volume = vol;
    }
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
    for (int i=0; i<128; i++) {

        if (instmSf.at(i) <= 0)
            continue;

        if (instmSf.at(i) >= synth_HSOUNDFONT.count())
            continue;

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
    int li = 0;
    for (int i=26; i<HANDLE_MIDI_COUNT-4; i++) {

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

void MidiSynthesizer::setBusGroup(InstrumentType t, int group)
{
    if (static_cast<int>(t) >= HANDLE_MIDI_COUNT)
        return;

    instMap[t].bus = group;

    if (!openned)
        return;

    BASS_Mixer_ChannelRemove(handles[t]);

    if (group == -1) {
        BASS_Mixer_StreamAddChannel(mixHandle, handles[t], 0);
    } else {
        InstrumentType toType = static_cast<InstrumentType>(group + HANDLE_MIDI_COUNT);
        BASS_Mixer_StreamAddChannel(handles[toType], handles[t], 0);
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
    if (vstiIndex < -1 || vstiIndex > 4)
        return;

    instMap[t].vsti = vstiIndex;
}

std::vector<std::string> MidiSynthesizer::audioDevices()
{
    std::vector<std::string> dvs;

    int a, count=0;
    BASS_DEVICEINFO info;
    for (a=0; BASS_GetDeviceInfo(a, &info); a++)
        if (info.flags&BASS_DEVICE_ENABLED) { // device is enabled
            dvs.push_back(info.name);
            count++; // count it
        }

    return dvs;
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

void MidiSynthesizer::setUsetFloattingPoint(bool use)
{
    if (use == useFloat)
        return;

    useFloat = use;

    QList<int> isf = instmSf;
    QList<int> dsf = drumSf;

    close(false);
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

DWORD MidiSynthesizer::getSpeakerFlag(SpeakerType Speaker)
{
    switch (Speaker)
    {
    case SpeakerType::SpeakerDefault:
        return 0;

    case SpeakerType::SpeakerFrontStereo:
        return BASS_SPEAKER_FRONT;
    case SpeakerType::SpeakerFrontLeft:
        return BASS_SPEAKER_FRONTLEFT;
    case SpeakerType::SpeakerFrontRight:
        return BASS_SPEAKER_FRONTRIGHT;

    case SpeakerType::SpeakerRearStereo:
        return BASS_SPEAKER_REAR;
    case SpeakerType::SpeakerRearLeft:
        return BASS_SPEAKER_REARLEFT;
    case SpeakerType::SpeakerRearRight:
        return BASS_SPEAKER_REARRIGHT;

    case SpeakerType::SpeakerCenterStereo:
        return BASS_SPEAKER_CENLFE;
    case SpeakerType::SpeakerCenterMono:
        return BASS_SPEAKER_CENTER;
    case SpeakerType::SpeakerSubwooferMono:
        return BASS_SPEAKER_LFE;

    case SpeakerType::SpeakerSideStereo:
        return BASS_SPEAKER_REAR2;
    case SpeakerType::SpeakerSideLeft:
        return BASS_SPEAKER_REAR2LEFT;
    case SpeakerType::SpeakerSideRight:
        return BASS_SPEAKER_REAR2RIGHT;
    }
}

void MidiSynthesizer::setSpeaker(InstrumentType type, int device, SpeakerType Speaker)
{

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

    DWORD f = useFloat ? BASS_SAMPLE_FLOAT : 0;
    #ifdef _WIN32
    vsti = BASS_VST_ChannelCreate(44100, 2,file.toStdWString().c_str(),
                               f|BASS_UNICODE|BASS_STREAM_DECODE);
    #else
    h = BASS_VST_ChannelCreate(44100, 2, file.toStdString().c_str(),
                               f|BASS_STREAM_DECODE);
    #endif

    if (vsti)
    {
        BASS_VST_INFO vstinfo;
        BASS_VST_GetInfo(vsti, &vstinfo);
        mVstiInfos[vstiIndex] = vstinfo;
        mVstiTempProgram[vstiIndex] = 0;
        mVstiTempParams[vstiIndex].clear();

        BASS_Mixer_StreamAddChannel(mixHandle, vsti, 0);
        handles[t] = vsti;

        setVolume(t, instMap[t].volume);
        setMute(t, instMap[t].mute);
        setSolo(t, instMap[t].solo);
        setBusGroup(t, instMap[t].bus);

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
    if (!sfLoaded)
    {
        for (HSOUNDFONT f : synth_HSOUNDFONT) {
            //BASS_MIDI_FontUnload(f, -1, -1);
            BASS_MIDI_FontFree(f);
        }
        synth_HSOUNDFONT.clear();

        for (const QString &sfile : sfFiles) {

            #ifdef _WIN32
            HSOUNDFONT f = BASS_MIDI_FontInit(sfile.toStdWString().c_str(),
                                              BASS_MIDI_FONT_NOFX);
            #else
            HSOUNDFONT f = BASS_MIDI_FontInit(sfile.toStdString().c_str(),
                                              BASS_MIDI_FONT_NOFX);
            #endif
            if (!f)
                continue;

            //BASS_MIDI_FontLoad(f, -1, -1);
            BASS_MIDI_FontCompact(f);
            synth_HSOUNDFONT.push_back(f);
        }

        sfLoaded = true;
    }


    if (synth_HSOUNDFONT.size() > 0) {
        BASS_MIDI_FONT font;
        font.font = synth_HSOUNDFONT.at(0);
        font.preset = -1;
        font.bank = 0;

        for (int i=0; i<HANDLE_MIDI_COUNT; i++) {
            HSTREAM h = handles[static_cast<InstrumentType>(i)];
            BASS_MIDI_StreamSetFonts(h, &font, 1); // set to stream to
        }
    }

    // Reset map intrument sf
    instmSf.clear();
    drumSf.clear();
    for (int i=0; i<128; i++) {
        instmSf.append(0);
    }
    for (int i=0; i<16; i++) {
        drumSf.append(0);
    }

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

std::vector<int> MidiSynthesizer::getChannelsFromType(InstrumentType t)
{
    std::vector<int> r;

    switch (t) {
    case InstrumentType::BassDrum:
        r.push_back(16);
        break;
    case InstrumentType::Snare:
        r.push_back(17);
        break;
    case InstrumentType::SideStick:
        r.push_back(18);
        break;
    case InstrumentType::LowTom:
        r.push_back(19);
        break;
    case InstrumentType::MidTom:
        r.push_back(20);
        break;
    case InstrumentType::HighTom:
        r.push_back(21);
        break;
    case InstrumentType::Hihat:
        r.push_back(22);
        break;
    case InstrumentType::Cowbell:
        r.push_back(23);
        break;
    case InstrumentType::CrashCymbal:
        r.push_back(24);
        break;
    case InstrumentType::RideCymbal:
        r.push_back(25);
        break;
    case InstrumentType::Bongo:
        r.push_back(26);
        break;
    case InstrumentType::Conga:
        r.push_back(27);
        break;
    case InstrumentType::Timbale:
        r.push_back(28);
        break;
    case InstrumentType::SmallCupShapedCymbals:
        r.push_back(29);
        break;
    case InstrumentType::ThaiChap:
        r.push_back(30);
        break;
    case InstrumentType::PercussionEtc:
        r.push_back(31);
        break;
    default: {
        for (int i=0; i<16; i++) {
            if (chInstType[i] != t)
                continue;
            r.push_back(i);
        }
    }
    }

    return r;
}
