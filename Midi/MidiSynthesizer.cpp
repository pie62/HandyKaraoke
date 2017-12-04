#include "MidiSynthesizer.h"

#include "BASSFX/FX.h"

#include <thread>
#include <cstring>
#include <iostream>
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


    // Map inst
    for (int i=0; i<58; i++)
    {
        InstrumentType t = static_cast<InstrumentType>(i);
        Instrument im;
        im.type = t;
        im.mute = false;
        im.solo = false;
        im.enable = true;
        im.volume = 50;
        im.bus = -1;

        instMap[t] = im;
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

    auto concurentThreadsSupported = std::thread::hardware_concurrency();
    float nVoices = (concurentThreadsSupported > 1) ? 500 : 256;

    //BASS_Init(-1, 44100, 0, NULL, NULL);

    BASS_SetConfig(BASS_CONFIG_BUFFER, 100);
    BASS_SetConfig(BASS_CONFIG_UPDATEPERIOD, 5);
    BASS_SetConfig(BASS_CONFIG_MIDI_VOICES, nVoices);

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
    for (int i=0; i<42; i++) {

        HSTREAM h = BASS_MIDI_StreamCreate(16, flags, 0);

        BASS_Mixer_StreamAddChannel(mixHandle, h, 0);

        InstrumentType t = static_cast<InstrumentType>(i);
        handles[t] = h;
    }

    // Create bus stream
    for (int i=42; i<58; i++) {
        HSTREAM h = BASS_Mixer_StreamCreate(44100, 2, f);
        BASS_ChannelPlay(h, true);
        BASS_Mixer_StreamAddChannel(mixHandle, h, 0);

        InstrumentType t = static_cast<InstrumentType>(i);
        handles[t] = h;
    }

    // Check volume .. mute.. solo.. bus.. and VST
    for (int i=0; i<58; i++) {
        InstrumentType t = static_cast<InstrumentType>(i);
        setVolume(t, instMap[t].volume);
        setMute(t, instMap[t].mute);
        setSolo(t, instMap[t].solo);
        setBusGroup(t, instMap[t].bus);

        #ifndef __linux__
        for (int i=0; i<instMap[t].vstUids.count(); i++) {
            DWORD fx = addVST(t, instMap[t].vstUids[i]);
            FX::setVSTParams(fx, instMap[t].vstTempParams[i]);
        }
        instMap[t].vstTempParams.clear();
        #endif
    }

    BASS_ChannelPlay(mixHandle, true);

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

    return true;
}

void MidiSynthesizer::close()
{
    if (!openned)
        return;

    // Clear VST
    #ifndef __linux__
    for (InstrumentType t : instMap.keys()) {
        instMap[t].vstTempParams.clear();
        for (DWORD fx : instMap[t].vstHandles) {
            instMap[t].vstTempParams.append(FX::getVSTParams(fx));
            BASS_VST_ChannelRemoveDSP(handles[t], fx);
        }
        instMap[t].vstHandles.clear();
    }
    #endif


    eq->setStreamHandle(0);
    reverb->setStreamHandle(0);
    chorus->setStreamHandle(0);

    for (HSTREAM h : handles) {
        BASS_ChannelStop(h);
        BASS_StreamFree(h);
    }
    handles.clear();

    for (HSOUNDFONT f : synth_HSOUNDFONT) {
        BASS_MIDI_FontUnload(f, -1, -1);
        BASS_MIDI_FontFree(f);
    }
    synth_HSOUNDFONT.clear();

    BASS_StreamFree(mixHandle);
    //BASS_Free();

    openned = false;
}

int MidiSynthesizer::outPutDevice()
{
    return outDev;
}

bool MidiSynthesizer::setOutputDevice(int dv)
{
    outDev = dv;

    if (openned)
        return BASS_ChannelSetDevice(mixHandle, dv);
}

void MidiSynthesizer::setSoundFonts(QStringList &soundfonsFiles)
{
    this->sfFiles.clear();
    this->sfFiles = soundfonsFiles;

    if (openned)
        setSfToStream();
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
    for (int i=0; i<42; i++) {
        HSTREAM h = handles[static_cast<InstrumentType>(i)];
        BASS_MIDI_StreamSetFonts(h, mFonts.data(), mFonts.size());
    }


    // check drum sf
    int li = 0;
    for (int i=26; i<42; i++) {

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
        BASS_MIDI_StreamEvent(getDrumHandleFromNote(note), 9, MIDI_EVENT_NOTE, MAKEWORD(note, 0));
    else
        BASS_MIDI_StreamEvent(handles[chInstType[ch]], ch, MIDI_EVENT_NOTE, MAKEWORD(note, 0));
}

void MidiSynthesizer::sendNoteOn(int ch, int note, int velocity)
{
    if (note < 0 || note > 127)
        return;

    if (ch == 9) {
        BASS_MIDI_StreamEvent(getDrumHandleFromNote(note), 9, MIDI_EVENT_NOTE, MAKEWORD(note, velocity));
        InstrumentType t = MidiHelper::getInstrumentDrumType(note);
        emit noteOnSended(t, instMap[t].bus, 9, note, velocity);
    }
    else {
        BASS_MIDI_StreamEvent(handles[chInstType[ch]], ch, MIDI_EVENT_NOTE, MAKEWORD(note, velocity));
        InstrumentType t = chInstType[ch];
        emit noteOnSended(t, instMap[t].bus, 9, note, velocity);
    }
}

void MidiSynthesizer::sendNoteAftertouch(int ch, int note, int value)
{
    if (note < 0 || note > 127)
        return;

    if (ch == 9)
        BASS_MIDI_StreamEvent(getDrumHandleFromNote(note), 9, MIDI_EVENT_KEYPRES, MAKEWORD(note, value));
    else
        BASS_MIDI_StreamEvent(handles[chInstType[ch]], ch, MIDI_EVENT_KEYPRES, MAKEWORD(note, value));
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
        for (int i=0; i<42; i++) {
            HSTREAM h = handles[static_cast<InstrumentType>(i)];
            BASS_MIDI_StreamEvents(h, BASS_MIDI_EVENTS_RAW, data, 3);
        }
        qDebug() << BASS_ErrorGetCode();
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
    sendToAllMidiStream(ch, MIDI_EVENT_PITCH, value);
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

void MidiSynthesizer::setBusGroup(InstrumentType t, int group)
{
    if (static_cast<int>(t) > 41)
        return;

    instMap[t].bus = group;

    if (!openned)
        return;

    BASS_Mixer_ChannelRemove(handles[t]);

    if (group == -1) {
        BASS_Mixer_StreamAddChannel(mixHandle, handles[t], 0);
    } else {
        InstrumentType toType = static_cast<InstrumentType>(group + 42);
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

    /*
    for (const auto &im : instMap) {

        Instrument i = im.second;
        //if (i.enable)
        //    continue;

        for (const int &ch : getChannelsFromType(i.type))
        {
            if (i.enable)
                BASS_MIDI_StreamEvent(stream, ch, MIDI_EVENT_MIXLEVEL, instMap[i.type].mixlevel);
            else
                BASS_MIDI_StreamEvent(stream, ch, MIDI_EVENT_MIXLEVEL, 0);
        }

    }
    */
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
        for (int i=0; i<42; i++) {
            HSTREAM h = handles[static_cast<InstrumentType>(i)];
            BASS_ChannelFlags(h, 0, BASS_MIDI_NOFX); // remove the flag
        }
    } else {
        for (int i=0; i<42; i++) {
            HSTREAM h = handles[static_cast<InstrumentType>(i)];
            BASS_ChannelFlags(h, BASS_MIDI_NOFX, BASS_MIDI_NOFX); // set flag
        }
    }
}

HSTREAM MidiSynthesizer::getChannelHandle(InstrumentType type)
{
    return handles[type];
}

#ifndef __linux__

DWORD MidiSynthesizer::addVST(InstrumentType type, DWORD uid)
{
    DWORD fx = 0;

    if (openned) {
        if (!_vstList.contains(uid))
            return 0;

        #ifdef _WIN32
        fx = BASS_VST_ChannelSetDSP(handles[type], _vstList[uid].vstPath.toStdWString().c_str(),
                                         BASS_VST_KEEP_CHANS|BASS_UNICODE, instMap[type].vstHandles.count());
        instMap[type].vstHandles.append(fx);
        #elif __APPLE__
        fx = BASS_VST_ChannelSetDSP(handles[type], _vstList[uid].vstPath.toStdString().c_str(),
                                         BASS_VST_KEEP_CHANS, instMap[type].fxHandles.count());
        instMap[type].fxHandles.append(fx);
        #endif
    }

    if (instMap[type].vstHandles.count() > instMap[type].vstUids.count()) {
        instMap[type].vstUids.append(uid);
        instMap[type].vstBypass.append(false);
    }

    return fx;
}

bool MidiSynthesizer::removeVST(InstrumentType type, int fxIndex)
{
    Instrument inst = instMap[type];

    if (fxIndex >= inst.vstUids.count())
        return false;

    if (openned) {
        #ifndef __linux__
        DWORD fx = inst.vstHandles.at(fxIndex);
        BASS_VST_ChannelRemoveDSP(handles[type], fx);
        instMap[type].vstHandles.removeAt(fxIndex);
        #endif
    }

    instMap[type].vstUids.removeAt(fxIndex);
    instMap[type].vstBypass.removeAt(fxIndex);
    return true;
}

void MidiSynthesizer::setVSTBypass(InstrumentType type, int fxIndex, bool state)
{
    instMap[type].vstBypass[fxIndex] = state;

    if (openned) {
        BASS_VST_SetBypass(instMap[type].vstHandles[fxIndex], state);
    }
}

bool MidiSynthesizer::isVSTFile(const QString &vstPath, BASS_VST_INFO *info)
{
    HSTREAM stream = BASS_StreamCreate(44100, 2, 0, NULL, NULL);

    #ifdef _WIN32
    DWORD h = BASS_VST_ChannelSetDSP(stream, vstPath.toStdWString().c_str(),
                                     BASS_VST_KEEP_CHANS|BASS_UNICODE, 0);
    #else
    DWORD h = BASS_VST_ChannelSetDSP(stream, vstPath.toStdString().c_str(),
                                     BASS_VST_KEEP_CHANS, 0);
    #endif

    bool result = false;

    if (BASS_VST_GetInfo(h, info) && !info->isInstrument)
        result = true;
    else
        result = false;

    BASS_VST_ChannelRemoveDSP(stream, h);
    BASS_StreamFree(stream);

    return result;
}

#endif

void MidiSynthesizer::sendToAllMidiStream(int ch, DWORD eventType, DWORD param)
{
    for (int i=0; i<42; i++) {
        HSTREAM stream = handles[static_cast<InstrumentType>(i)];
        BASS_MIDI_StreamEvent(stream, ch, eventType, param);
    }
}

void MidiSynthesizer::setSfToStream()
{
    for (HSOUNDFONT f : synth_HSOUNDFONT) {
        //BASS_MIDI_FontUnload(f, -1, -1);
        BASS_MIDI_FontFree(f);
    }
    synth_HSOUNDFONT.clear();

    for (const QString &sfile : sfFiles) {

        #ifdef _WIN32
        HSOUNDFONT f = BASS_MIDI_FontInit(sfile.toStdWString().c_str(),
                                          /*BASS_MIDI_FONT_MMAP|*/BASS_MIDI_FONT_NOFX);
        #else
        HSOUNDFONT f = BASS_MIDI_FontInit(sfile.toStdString().c_str(),
                                          BASS_MIDI_FONT_MMAP|BASS_MIDI_FONT_NOFX);
        #endif
        if (f) {
            //BASS_MIDI_FontLoad(f, -1, -1);
            BASS_MIDI_FontCompact(f);
            synth_HSOUNDFONT.push_back(f);
        }

    }

    //BASS_MIDI_FontCompact(0);

    if (synth_HSOUNDFONT.size() > 0) {
        BASS_MIDI_FONT font;
        font.font = synth_HSOUNDFONT.at(0);
        font.preset = -1;
        font.bank = 0;

        for (int i=0; i<42; i++) {
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
