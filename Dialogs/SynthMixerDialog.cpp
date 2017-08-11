#include "SynthMixerDialog.h"
#include "ui_SynthMixerDialog.h"

#include "MainWindow.h"
#include "Dialogs/SettingVuDialog.h"


SynthMixerDialog::SynthMixerDialog(QWidget *parent, MainWindow *mainWin) : //, MainWindow *mainWin) :
    QDialog(parent),
    ui(new Ui::SynthMixerDialog)
{
    ui->setupUi(this);

    this->mainWin = mainWin;
    this->player = mainWin->midiPlayer();
    this->synth = player->midiSynthesizer();

    mapChInstUI();
    setChInstDetails();

    setBtnEqIcon(synth->equalizer24BandFX()->isOn());
    setBtnReverbIcon(synth->reverbFX()->isOn());
    setBtnChorusIcon(synth->chorusFX()->isOn());

    ui->scrollArea->setWidgetResizable(false);
    this->adjustSize();
    this->setMinimumSize(970, height());
    this->setMaximumHeight(height() + 30);

    { // Settings
        QSettings st("HandyKaraoke", "SynthMixer");

        QSize size = st.value("Size", this->size()).toSize();
        resize(size);

        LEDVu *vu = chInstMap.first()->vuBar();
        QString bg = st.value("LedBgColor", vu->backgroundColor().name()).toString();
        QString o1 = st.value("LedColorOn1", vu->ledColorOn1().name()).toString();
        QString o2 = st.value("LedColorOn2", vu->ledColorOn2().name()).toString();
        QString o3 = st.value("LedColorOn3", vu->ledColorOn3().name()).toString();
        QString f1 = st.value("LedColorOff1", vu->ledColorOff1().name()).toString();
        QString f2 = st.value("LedColorOff2", vu->ledColorOff2().name()).toString();
        QString f3 = st.value("LedColorOff3", vu->ledColorOff3().name()).toString();
        bool sph = st.value("ShowPeakHold", vu->isShowPeakHold()).toBool();
        int phm = st.value("PeakHoldMs", vu->peakHoldMs()).toInt();

        st.beginReadArray("SynthMixer");
        for (InstrumentType t : chInstMap.keys())
        {
            st.setArrayIndex(static_cast<int>(t));
            int ml = st.value("MixLevel", 100).toInt();
            bool m = st.value("Mute", false).toBool();
            bool s = st.value("Solo", false).toBool();

            InstCh * ich = chInstMap[t];
            ich->setSliderLevel(ml);
            ich->setMuteButton(m);
            ich->setSoloButton(s);

            vu = ich->vuBar();
            vu->setBackGroundColor(QColor(bg));
            vu->setLedColorOn1(QColor(o1));
            vu->setLedColorOn2(QColor(o2));
            vu->setLedColorOn3(QColor(o3));
            vu->setLedColorOff1(QColor(f1));
            vu->setLedColorOff2(QColor(f2));
            vu->setLedColorOff3(QColor(f3));
            vu->setShowPeakHold(sph);
            vu->setPeakHoldMs(phm);

            synth->setMixLevel(t, ml);
            synth->setMute(t, m);
            synth->setSolo(t, s);
        }
        st.endArray();
    }

    connect(ui->btnEq, SIGNAL(clicked()),
            mainWin->equalizer24BandDialog(), SLOT(show()));
    connect(ui->btnReverb, SIGNAL(clicked()),
            mainWin->reverbDialog(), SLOT(show()));
    connect(ui->btnChorus, SIGNAL(clicked()),
            mainWin->chorusDialog(), SLOT(show()));

    connect(mainWin->equalizer24BandDialog(), SIGNAL(switchChanged(bool)),
            this, SLOT(setBtnEqIcon(bool)));
    connect(mainWin->reverbDialog(), SIGNAL(switchChanged(bool)),
            this, SLOT(setBtnReverbIcon(bool)));

    connect(mainWin->chorusDialog(), SIGNAL(switchChanged(bool)),
            this, SLOT(setBtnChorusIcon(bool)));
}

SynthMixerDialog::~SynthMixerDialog()
{
    // settings
    {
        QSettings st("HandyKaraoke", "SynthMixer");
        st.setValue("Size", this->size());

        LEDVu *vu = chInstMap.first()->vuBar();
        st.setValue("LedColorOn1", vu->ledColorOn1());
        st.setValue("LedColorOn2", vu->ledColorOn2());
        st.setValue("LedColorOn3", vu->ledColorOn3());
        st.setValue("LedColorOff1", vu->ledColorOff1());
        st.setValue("LedColorOff2", vu->ledColorOff2());
        st.setValue("LedColorOff3", vu->ledColorOff3());
        st.setValue("ShowPeakHold", vu->isShowPeakHold());
        st.setValue("PeakHoldMs", vu->peakHoldMs());

        st.beginWriteArray("SynthMixer");

        for (InstrumentType t : chInstMap.keys())
        {
            st.setArrayIndex(static_cast<int>(t));
            st.setValue("MixLevel", synth->mixLevel(t));
            st.setValue("Mute", synth->isMute(t));
            st.setValue("Solo", synth->isSolo(t));
        }

        st.endArray();
    }
    // ------------------------
    chInstMap.clear();

    delete ui;
}

void SynthMixerDialog::setBtnEqIcon(bool s)
{
    if (s)
        ui->btnEq->setIcon(QIcon(":/Icons/circle_green"));
    else
        ui->btnEq->setIcon(QIcon(":/Icons/circle_red"));
}

void SynthMixerDialog::setBtnReverbIcon(bool s)
{
    if (s)
        ui->btnReverb->setIcon(QIcon(":/Icons/circle_green"));
    else
        ui->btnReverb->setIcon(QIcon(":/Icons/circle_red"));
}

void SynthMixerDialog::setBtnChorusIcon(bool s)
{
    if (s)
        ui->btnChorus->setIcon(QIcon(":/Icons/circle_green"));
    else
        ui->btnChorus->setIcon(QIcon(":/Icons/circle_red"));
}

void SynthMixerDialog::setMute(InstrumentType t, bool m)
{
    synth->setMute(t, m);
}

void SynthMixerDialog::setSolo(InstrumentType t, bool s)
{
    synth->setSolo(t, s);
}

void SynthMixerDialog::setMixLevel(InstrumentType t, int level)
{
    synth->setMixLevel(t, level);
}

void SynthMixerDialog::resetMixLevel(InstrumentType t)
{
    chInstMap[t]->setSliderLevel(100);
    synth->setMixLevel(t, 100);
}

void SynthMixerDialog::onPlayerPlayingEvents(MidiEvent *e)
{
    if (e->eventType() == MidiEventType::NoteOn)
    {
        InstrumentType t;

        if (e->channel() == 9) {
            t = MidiHelper::getInstrumentDrumType(e->data1());
        }
        else {
            t = player->midiChannel()[e->channel()].instrumentType();
        }

        chInstMap[t]->peak(e->data2());
    }
}

void SynthMixerDialog::showEvent(QShowEvent *)
{
    connect(player, SIGNAL(playingEvents(MidiEvent*)),
            this, SLOT(onPlayerPlayingEvents(MidiEvent*)));
}

void SynthMixerDialog::hideEvent(QHideEvent *event)
{
    disconnect(player, SIGNAL(playingEvents(MidiEvent*)),
            this, SLOT(onPlayerPlayingEvents(MidiEvent*)));
}

void SynthMixerDialog::mapChInstUI()
{
    chInstMap[InstrumentType::Piano]                = ui->ch;
    chInstMap[InstrumentType::Organ]                = ui->ch_2;
    chInstMap[InstrumentType::Accordion]            = ui->ch_3;
    chInstMap[InstrumentType::ChromaticPercussion]  = ui->ch_4;
    chInstMap[InstrumentType::Percussive]           = ui->ch_5;
    chInstMap[InstrumentType::Bass]                 = ui->ch_6;
    chInstMap[InstrumentType::AcousticGuitarNylon]  = ui->ch_7;
    chInstMap[InstrumentType::AcousticGuitarSteel]  = ui->ch_8;
    chInstMap[InstrumentType::ElectricGuitarJazz]   = ui->ch_9;
    chInstMap[InstrumentType::ElectricGuitarClean]  = ui->ch_10;
    chInstMap[InstrumentType::OverdrivenGuitar]     = ui->ch_11;
    chInstMap[InstrumentType::DistortionGuitar]     = ui->ch_12;
    chInstMap[InstrumentType::HarmonicsGuitar]      = ui->ch_13;
    chInstMap[InstrumentType::Trumpet]      = ui->ch_14;
    chInstMap[InstrumentType::Brass]        = ui->ch_15;
    chInstMap[InstrumentType::SynthBrass]   = ui->ch_16;
    chInstMap[InstrumentType::Saxophone]    = ui->ch_17;
    chInstMap[InstrumentType::Reed]         = ui->ch_18;
    chInstMap[InstrumentType::Pipe]         = ui->ch_19;
    chInstMap[InstrumentType::Strings]      = ui->ch_20;
    chInstMap[InstrumentType::Ensemble]     = ui->ch_21;
    chInstMap[InstrumentType::SynthLead]    = ui->ch_22;
    chInstMap[InstrumentType::SynthPad]     = ui->ch_23;
    chInstMap[InstrumentType::SynthEffects] = ui->ch_24;
    chInstMap[InstrumentType::Ethnic]       = ui->ch_25;
    chInstMap[InstrumentType::SoundEffects] = ui->ch_26;

    chInstMap[InstrumentType::BassDrum]     = ui->ch_27;
    chInstMap[InstrumentType::Snare]        = ui->ch_28;
    chInstMap[InstrumentType::SideStick]    = ui->ch_29;
    chInstMap[InstrumentType::LowTom]       = ui->ch_30;
    chInstMap[InstrumentType::MidTom]       = ui->ch_31;
    chInstMap[InstrumentType::HighTom]      = ui->ch_32;
    chInstMap[InstrumentType::Hihat]        = ui->ch_33;
    chInstMap[InstrumentType::Cowbell]      = ui->ch_34;
    chInstMap[InstrumentType::CrashCymbal]  = ui->ch_35;
    chInstMap[InstrumentType::RideCymbal]   = ui->ch_36;
    chInstMap[InstrumentType::Bongo]        = ui->ch_37;
    chInstMap[InstrumentType::Conga]        = ui->ch_38;
    chInstMap[InstrumentType::Timbale]      = ui->ch_39;
    chInstMap[InstrumentType::SmallCupShapedCymbals] = ui->ch_40;
    chInstMap[InstrumentType::ChineseCymbal] = ui->ch_41;
    chInstMap[InstrumentType::PercussionEtc] = ui->ch_42;
}

void SynthMixerDialog::setChInstDetails()
{
    QString names[42] = { "Piano", "Organ", "Accordn", "Chmt.P", "Percsve",
                        "Bass", "Nylon", "Steel", "Jazz", "Clean",
                        "Overdrv", "Distrn", "Harmncs", "Trumpet", "Brass",
                        "S.Brass", "Saxphn", "Reed", "Pipe", "Strings",
                        "Ensembl", "SynthL", "SynthP", "SynthE", "Ethnic", "SoundFX",
                        "Kick", "Snare", "S.Stick", "LowTom", "MidTom",
                        "HighTom", "Hihat", "Cowbell", "Crash", "Ride",
                        "Bongo", "Conga", "Timbale", "ฉิ่ง", "ฉาบ", "PercEtc" };

    QString tooltips[42] = { "Piano", "Organ", "Accordion", "Chromatic Percussion",
                             "Percussive", "Bass", "Acoustic Guitar (nylon)",
                             "Acoustic Guitar (steel)", "Electric Guitar (jazz)",
                             "Electric Guitar (clean)", "Overdriven Guitar",
                             "Distortion Guitar", "Harmonics Guitar", "Trumpet",
                             "Brass", "Synth Brass", "Saxophone", "Reed", "Pipe",
                             "Strings", "Ensemble", "Synth Lead", "Synth Pad",
                             "Synth Effects", "Ethnic", "Sound effects",
                             "Kick (Bass Drum)", "Snare", "Side Stick/Rimshot",
                             "Low Tom", "Mid Tom", "High Tom", "Hi-hat", "Cowbell",
                             "Crash Cymbal", "Ride Cymbal", "Bongo", "Conga",
                             "Timbale", "ฉิ่ง / Triangle",
                             "ฉาบ / Chinese Cymbal", "Percussion Etc." };

    QString rc = ":/Icons/Instruments/";
    QString imgs[42] = { "Piano.png", "Organ.png", "Accordion.png",
                         "Chromatic Percussion.png", "Percussive.png",
                         "Bass.png", "Nylon.png", "Steel.png", "Jazz.png",
                         "Clean.png", "Overdriven.png", "Distortion.png",
                         "Harmonics.png", "Trumpet.png", "Brass.png",
                         "Synth Brass.png", "Saxophone.png", "Reed.png",
                         "Pipe.png", "Strings.png", "Ensemble.png",
                         "Synth Lead.png", "Synth Pad.png", "Synth Effects.png",
                         "Ethnic.png", "FX.png",
                         "Kick.png", "Snare.png", "Side Stick.png",
                         "Low Tom.png", "Mid Tom.png", "High Tom.png",
                         "Hihat.png", "Cowbell.png", "Crash.png", "Ride.png",
                         "Bongo.png", "Conga.png", "Timbale.png", "Ching.png",
                         "Chab.png", "Percussion Etc.png" };


    for (int i=0; i<42; i++)
    {
        InstCh *ich = chInstMap.values()[i];

        ich->setInstrumentType(chInstMap.keys()[i]);
        ich->setInstrumentName(names[i], tooltips[i]);

        ich->setInstrumentImage(QImage(rc + imgs[i]));

        connect(ich, SIGNAL(muteChanged(InstrumentType,bool)),
                this, SLOT(setMute(InstrumentType,bool)));
        connect(ich, SIGNAL(soloChanged(InstrumentType,bool)),
                this, SLOT(setSolo(InstrumentType,bool)));
        connect(ich, SIGNAL(sliderLevelChanged(InstrumentType,int)),
                this, SLOT(setMixLevel(InstrumentType,int)));
        connect(ich, SIGNAL(sliderDoubleClicked(InstrumentType)),
                this, SLOT(resetMixLevel(InstrumentType)));
    }
}

void SynthMixerDialog::on_btnSettingVu_clicked()
{
    QList<LEDVu*> vus;
    for (InstCh *ch : chInstMap.values())
    {
        vus.append(ch->vuBar());
    }

    SettingVuDialog vdlg(this, vus);
    vdlg.setModal(true);
    vdlg.adjustSize();
    vdlg.setFixedSize(vdlg.size());
    vdlg.setWindowTitle(vdlg.windowTitle() + " (Handy Synth Mixer)");
    vdlg.exec();
}

void SynthMixerDialog::on_btnReset_clicked()
{
    for (InstrumentType t : chInstMap.keys())
    {
        resetMixLevel(t);
        chInstMap[t]->setMuteButton(false);
        chInstMap[t]->setSoloButton(false);
        synth->setMute(t, false);
        synth->setSolo(t, false);
    }
}
