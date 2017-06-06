#include "MidiPlayer.h"

#include <QDebug>


MidiPlayer::MidiPlayer(QObject *parent) : QObject(parent)
{
    midi_out = new MidiOut();

    for (int i=0; i<16; i++) {
        Channel *c = new Channel(i, 0);
        channels.append(c);
    }

    synth = new MidiSynthesizer();
    midi = new MidiFile();
    thread = new QThread();
    eTimer = new QElapsedTimer();

    moveToThread(thread);

    connect(thread, SIGNAL(started()), this, SLOT(playEvents()));
}

MidiPlayer::~MidiPlayer()
{
    if (!stoped) stop();

    delete eTimer;
    delete thread;
    delete midi;
    delete synth;
    foreach (Channel *c, channels) {
        delete c;
    }
    delete midi_out;
}

QStringList MidiPlayer::midiDevices()
{
    QStringList outName;
    for (int i=0; i<midi_out->getPortCount(); i++) {
        QString n = QString::fromStdString(midi_out->getPortName(i));
        outName << n;
    }
    return outName;
}

int MidiPlayer::position()
{
    if (playing) {
        float time = (eTimer->elapsed() + startEventTime) * 1.0f;
        return tickFromTime(time);
    } else {
        return midi_position;
    }
}

qint64 MidiPlayer::positionMS()
{
    return playing ? startEventTime + eTimer->elapsed() : midi_position_ms;
}

bool MidiPlayer::setMidiOut(int portNumer)
{
    if (portNumer != -1 && portNumer >= midi_out->getPortCount())
        return false;

    if (!stoped)
        stop();

    bool result = false;
    if (midi_out->isPortOpen())
        midi_out->closePort();

    if (portNumer == -1) {
        defaultPort = -1;
        result = true;
    } else {
        midi_out->openPort(portNumer);
        midi_out->setVolume(midi_volume / 100.0f);
        result = midi_out->isPortOpen();
        defaultPort = result ? portNumer : defaultPort;
    }

    for (int i=0; i<16; i++) {
        channels[i]->setPort(defaultPort);
    }

    return result;
}

bool MidiPlayer::load(QString file)
{
    if (playing) stop();
    if (!midi->read(file.toStdString()))
        return false;

    MidiEvent *e = midi->events().back();
    midi_duration_ms = timeFromTick(e->tick());
    midi_duration = e->tick();
}

void MidiPlayer::play()
{
    if (playing) return;

    playing = true;
    stoped = false;
    playAfterSeek = true;
    thread->start();
}

void MidiPlayer::pause()
{
    if (!playing || stoped) return;

    playing = false;
    stoped = false;
    playAfterSeek = false;

    if (thread->isRunning())
        thread->quit();

    sendAllNotesOff();
    thread->msleep(50);
    sendAllNotesOff();
}

void MidiPlayer::resume()
{
    if (playing || stoped) return;
    playing = true;
    playAfterSeek = true;
    thread->start();
}

void MidiPlayer::stop()
{
    if (stoped) return;;

    playing = false;
    stoped = true;
    playAfterSeek = false;

    if (thread->isRunning())
        thread->quit();

    if (!thread->isRunning())
        qDebug() << "Thread is finished";

    sendAllNotesOff();
    thread->msleep(50);
    sendAllNotesOff();
    thread->msleep(120);
    startEventTime = 0;
    playedIndex = 0;
    midi_position = 0;
    midi_position_ms = 0;

    resetAllControllers();
    synth->reset();
}

void MidiPlayer::setVolume(int v)
{
    if (v < 0) midi_volume = 0;
    else if (v > 100) midi_volume = 100;
    else midi_volume = v;

    midi_out->setVolume(midi_volume / 100.0f);
    synth->setVolume(midi_volume / 100.0f);
}

void MidiPlayer::setPosition(int tick)
{
    if (playing) {
        pause();
    }

    for (int i=0; i<midi->events().size(); i++) {
        if (midi->events()[i]->eventType() == MidiEventType::Controller
            || midi->events()[i]->eventType() == MidiEventType::ProgramChange) {
            sendEvent(midi->events()[i]);
        }
        playedIndex = i;
        midi_position = midi->events()[i]->tick();
        if (midi->events()[i]->tick() >= tick) break;
    }

    midi_position_ms = timeFromTick(midi_position);
    startEventTime = midi_position_ms;

    if (playAfterSeek) resume();
}

void MidiPlayer::playEvents()
{
    if (playedIndex != 0) {
        startEventTime = timeFromTick(midi->events()[playedIndex]->tick());
    }

    //QElapsedTimer t;
    //t.start();
    eTimer->restart();

    for (int i=playedIndex; i<midi->events().size(); i++) {

        if (!playing) break;

        MidiEvent *e = midi->events()[i];
        if (e->eventType() != MidiEventType::Meta) {

            long eventTime = timeFromTick(e->tick());
            long waitTime = eventTime - startEventTime - eTimer->elapsed();

            if (waitTime > 0) thread->msleep(waitTime);

            if (e->eventType() != MidiEventType::SysEx) {

                if (e->eventType() == MidiEventType::Controller
                    || e->eventType() == MidiEventType::ProgramChange) {
                    sendEvent(e);
                } else {
                    if (channels[e->channel()]->isMute() == false) {
                        if (useSolo) {
                            if (channels[e->channel()]->isSolo()) {
                                sendEvent(e);
                            }
                        } else {
                            sendEvent(e);
                        }
                    }
                }

            }

            midi_position_ms = eventTime;

        } else { // Meta event
            if (e->metaEventType() == MidiMetaType::SetTempo) {
                midi_bpm = e->tempoBPM();
            }
        }

        playedIndex = i;
        midi_position = e->tick();

        emit playingEvent(e);

    } // End for loop
    thread->quit();

    int count = midi->events().size();

    if (playedIndex == count - 1) {
        playing = false;
        stoped = true;
        playAfterSeek = false;
        thread->msleep(50);
        startEventTime = 0;
        playedIndex = 0;
        midi_position = 0;
        midi_position_ms = 0;
        emit finished();
    }
}

void MidiPlayer::sendEvent(MidiEvent *e)
{
    int ch = e->channel();

    switch (e->eventType()) {
    case MidiEventType::NoteOff: {
        if (defaultPort == -1) {
            synth->sendNoteOff(ch, e->data1(), e->data2());
        } else {
            midi_out->sendNoteOff(ch, e->data1(), e->data2());
        }
        break;
    }
    case MidiEventType::NoteOn: {
        if (defaultPort == -1) {
            synth->sendNoteOn(ch, e->data1(), e->data2());
        } else {
            midi_out->sendNoteOn(ch, e->data1(), e->data2());
        }
        break;
    }
    case MidiEventType::NoteAftertouch: {
        if (defaultPort == -1) {
            synth->sendNoteAftertouch(ch, e->data1(), e->data2());
        } else {
            midi_out->sendNoteAftertouch(ch, e->data1(), e->data2());
        }
        break;
    }
    case MidiEventType::Controller: {
        if (e->data1() == 7) channels[ch]->setVolume(e->data2());
        if (e->data1() == 10) channels[ch]->setPan(e->data2());

        if (defaultPort == -1) {
            synth->sendController(ch, e->data1(), e->data2());
        } else {
            midi_out->sendController(ch, e->data1(), e->data2());
        }
        break;
    }
    case MidiEventType::ProgramChange: {
        channels[ch]->setInstrument(e->data1());
        if (defaultPort == -1) {
            synth->sendProgramChange(ch, e->data1());
        } else {
            midi_out->sendProgramChange(ch, e->data1());
        }
        break;
    }
    case MidiEventType::ChannelAftertouch: {
        if (defaultPort == -1) {
            synth->sendChannelAftertouch(ch, e->data1());
        } else {
            midi_out->sendChannelAftertouch(ch, e->data1());
        }
        break;
    }
    case MidiEventType::PitchBend: {
        if (defaultPort == -1) {
            synth->sendPitchBend(ch, e->data1());
        } else {
            midi_out->sendPitchBend(ch, e->data1());
        }
        break;
    }
    }

}

void MidiPlayer::sendAllNotesOff(int ch)
{
    if (defaultPort == -1) {
        synth->sendAllNotesOff(ch);
    } else {
        midi_out->sendAllNotesOff(ch);
    }
}

void MidiPlayer::sendAllNotesOff()
{
    for (int i=0; i<16; i++) {
        sendAllNotesOff(i);
    }
}

void MidiPlayer::resetAllControllers()
{
    for (int i=0; i<16; i++) {

        midi_out->sendResetAllControllers(i);
        midi_out->sendProgramChange(i, 0);

        synth->sendResetAllControllers(i);

        channels[i]->setVolume(127);
        channels[i]->setPan(64);
        channels[i]->setInstrument(0);
    }
}

float MidiPlayer::timeFromTick(uint32_t tick)
{
    switch (midi->divisionType()) {
        case MidiFile::PPQ: {
            float tempo_event_time = 0.0;
            qint32 tempo_event_tick = 0;
            float tempo = 120.0;

            for (MidiEvent* e : midi->tempoEvents()) {
                if (e->tick() >= tick) {
                    break;
                }
                tempo_event_time +=
                    (((float)(e->tick() - tempo_event_tick)) / midi->resorution() / (tempo / 60000));
                tempo_event_tick = e->tick();
                tempo = e->tempoBPM();
            }

            float time =
                tempo_event_time + (((float)(tick - tempo_event_tick)) / midi->resorution() / (tempo / 60000));
            return time;

            /*const float milliSecondsPerQuarterNote = microsecondsPerQuarterNote / 1000.0f;
            const float milliSecondsPerTick = milliSecondsPerQuarterNote / midi->getResorution();
            float deltaTimeMilliInSeconds = tick * milliSecondsPerTick;
            return deltaTimeMilliInSeconds;*/
        }
        case MidiFile::SMPTE24: {
            return 1000.0f * ((float)(tick) / (midi->resorution() * 24.0));
        }
        case MidiFile::SMPTE25: {
            return 1000.0f * ((float)(tick) / (midi->resorution() * 25.0));
        }
        case MidiFile::SMPTE30DROP: {
            return 1000.0f * ((float)(tick) / (midi->resorution() * 29.97));
        }
        case MidiFile::SMPTE30: {
            return 1000.0f * ((float)(tick) / (midi->resorution() * 30.0));
        }
        default: {
            return -1;
        }
    }

}

qint32 MidiPlayer::tickFromTime(float msTime)
{
    switch (midi->divisionType()) {
    case MidiFile::PPQ: {
        float tempo_event_time = 0.0;
        qint32 tempo_event_tick = 0;
        float tempo = 120.0;

        for (MidiEvent* e : midi->tempoEvents()) {
            float next_tempo_event_time =
                tempo_event_time +
                (((float)(e->tick() - tempo_event_tick)) / midi->resorution() / (tempo / 60000));
            if (next_tempo_event_time >= msTime) break;
            tempo_event_time = next_tempo_event_time;
            tempo_event_tick = e->tick();
            tempo = e->tempoBPM();
        }

        return tempo_event_tick + (qint32)((msTime - tempo_event_time) * (tempo / 60000) * midi->resorution());
    }
    case MidiFile::SMPTE24:
        return 1000.0f * ((qint32)(msTime * midi->resorution() * 24.0));
    case MidiFile::SMPTE25:
        return 1000.0f * ((qint32)(msTime * midi->resorution() * 25.0));
    case MidiFile::SMPTE30DROP:
        return 1000.0f * ((qint32)(msTime * midi->resorution() * 29.97));
    case MidiFile::SMPTE30:
        return 1000.0f * ((qint32)(msTime * midi->resorution() * 30.0));
    default:
        return 0.0f;
    }
}
