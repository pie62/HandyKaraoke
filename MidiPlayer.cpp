#include "MidiPlayer.h"


MidiPlayer::MidiPlayer(QObject *parent) : QObject(parent)
{
    timer = new QTimer();
    //midi_device = new QMidiOut();

    timer->setInterval(10);

    pMidiDevices = QMidiOut::devices();
    midi_out = pMidiDevices.keys()[0];
    midi_volume = 50;
    midi_duration = 0;
    midi_position = 0;

    connect(timer, SIGNAL(timeout()), this, SLOT(onTimerTimeOut()));
}

MidiPlayer::~MidiPlayer()
{
    stop();
    BASS_StreamFree(midi);
    foreach (HSOUNDFONT sf, fonts) {
        BASS_MIDI_FontFree(sf);
    }
    /*if (midi_device->isConnected()) {
        midi_device->disconnect();
    }
    delete midi_device;*/
    delete timer;
}

uint32_t MidiPlayer::resolution()
{
    float ppqn;
    BASS_ChannelGetAttribute(midi, BASS_ATTRIB_MIDI_PPQN, &ppqn);
    return ppqn;
}

void MidiPlayer::midiFilter(BASS_MIDI_EVENT *event)
{

    switch (event->event) {
    case MIDI_EVENT_NOTE:
        break;
    case MIDI_EVENT_KEYPRES:
        break;
    case MIDI_EVENT_PROGRAM:
        break;
    case MIDI_EVENT_CHANPRES:
        break;
    case MIDI_EVENT_PITCH:
        break;
    default:
        break;
    }
    //emit positionChanged(event->tick);
}

void MidiPlayer::setMidiOut(QString dv)
{
    if (!isStoped())
        stop();

    if (dv != "SoundFont") {
        if (pMidiDevices.keys().indexOf(dv) != -1) {
            midi_out = dv;
            /*if (midi_device->isConnected())
                midi_device->disconnect();
            midi_device->connect(dv);*/
        }
    } else {
        midi_out = dv;
        //if (midi_device->isConnected())
        //    midi_device->disconnect();
    }
}

bool MidiPlayer::load(QString file)
{
    stop();
    BASS_StreamFree(midi);
    midi = BASS_MIDI_StreamCreateFile(FALSE, file.toStdString().c_str(),
                               0, 0, BASS_SAMPLE_FLOAT|BASS_MIDI_NOFX, 1);
    if (!midi) {
        qDebug() << "MidiPlayer: Can't load";
        return false;
    }
    else {
        BASS_ChannelSetAttribute(midi, BASS_ATTRIB_VOL, midi_volume / 100.0f);
        BASS_ChannelSetSync(midi, BASS_SYNC_END, 0, endSync, this);
        BASS_MIDI_StreamSetFilter(midi, TRUE, midiFilterProc, this);

        midi_duration = BASS_ChannelGetLength(midi, BASS_POS_MIDI_TICK);
        QWORD l = BASS_ChannelGetLength(midi, BASS_POS_BYTE);
        qint64 ms = BASS_ChannelBytes2Seconds(midi, l) * 1000;
        emit durationChanged(midi_duration);
        emit durationMSChanged(ms);

        return true;
    }
}

void MidiPlayer::setSoundFonts(QStringList files)
{
    //stop();
    // Remove old sf
    unsetSoundFonts();

    // set new sf
    foreach (QString file, files) {
        HSOUNDFONT f = BASS_MIDI_FontInit(file.toStdString().c_str(), 0);
        if (f) {
            fonts.append(f);
        }
    }
    BASS_MIDI_FONTEX sf[fonts.count()];
    for (int i=0; i<fonts.count(); i++) {
        sf[i].font = fonts[0];
        sf[i].spreset = -1;
        sf[i].sbank = -1;
        sf[i].dpreset = -1;
        sf[i].dbank = 0;
        sf[i].dbanklsb = 0;
    }
    BASS_MIDI_StreamSetFonts(0, &sf[0], 1); // set default soundfont
    BASS_MIDI_StreamSetFonts(midi, sf, fonts.count()|BASS_MIDI_FONT_EX); // // set for current stream
}

void MidiPlayer::unsetSoundFonts()
{
    foreach (HSOUNDFONT sf, fonts) {
        BASS_MIDI_FontFree(sf);
    }
    fonts.clear();
}

void MidiPlayer::play()
{
    timer->start();
    BASS_ChannelPlay(midi, FALSE);
}

void MidiPlayer::pause()
{
    BASS_ChannelPause(midi);
    timer->stop();
}

void MidiPlayer::resume()
{
    BASS_ChannelPlay(midi, FALSE);
    timer->start();
}

void MidiPlayer::stop()
{
    BASS_ChannelStop(midi);
    BASS_ChannelSetPosition(midi, 0, BASS_POS_MIDI_TICK);
    timer->stop();

    midi_position = 0;
    emit positionMSChanged(0);
}

void MidiPlayer::setVolume(int v)
{
    if (v > 100) midi_volume = 100;
    else if (v < 0) midi_volume = 0;
    else midi_volume = v;
    BASS_ChannelSetAttribute(midi, BASS_ATTRIB_VOL, midi_volume / 100.0f);
}

void MidiPlayer::setPosition(int tick)
{
    midi_position = tick;
    BASS_ChannelSetPosition(midi, tick, BASS_POS_MIDI_TICK);
}

void MidiPlayer::endSync(HSYNC handle, DWORD channel, DWORD data, void *user)
{
    MidiPlayer *p = (MidiPlayer*)user;
    p->emitFinished();
}


BOOL MidiPlayer::midiFilterProc(HSTREAM handle, DWORD track, BASS_MIDI_EVENT *event, BOOL seeking, void *user)
{
    MidiPlayer *p = (MidiPlayer*)user;
    p->midiFilter(event);

    return TRUE; // process the event
}

void MidiPlayer::onTimerTimeOut()
{
    midi_position = BASS_ChannelGetPosition(midi, BASS_POS_MIDI_TICK);
    QWORD time = BASS_ChannelGetPosition(midi, BASS_POS_BYTE);
    qint64 ms = BASS_ChannelBytes2Seconds(midi, time) * 1000;

    emit positionChanged(midi_position);
    emit positionMSChanged(ms);
}
