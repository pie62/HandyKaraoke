#include "SynthMixerDialog.h"
#include "ui_SynthMixerDialog.h"

#include "MainWindow.h"
#include "Config.h"
#include "BASSFX/FX.h"
#include "Dialogs/SettingVuDialog.h"
#include "Dialogs/VSTDialog.h"
#include "Dialogs/BusDialog.h"

#include "FXDialogs/AutoWahFXDialog.h"
#include "FXDialogs/ChorusFXDialog.h"
#include "FXDialogs/CompressorFXDialog.h"
#include "FXDialogs/DistortionFXDialog.h"
#include "FXDialogs/EchoFXDialog.h"
#include "FXDialogs/EQ15BandDialog.h"
#include "FXDialogs/EQ31BandDialog.h"
#include "FXDialogs/ReverbFXDialog.h"

#include <bass.h>

#ifndef __linux__
#include <bass_vst.h>
#include "Dialogs/VSTDirsDialog.h"
#endif

#include <QMenu>
#include <QScrollBar>


SynthMixerDialog::SynthMixerDialog(QWidget *parent, MainWindow *mainWin) : //, MainWindow *mainWin) :
    QDialog(parent),
    ui(new Ui::SynthMixerDialog),
    signalVstActionMapper(this),
    signalBusActionMapper(this)
{
    ui->setupUi(this);

    this->mainWin = mainWin;
    this->player = mainWin->midiPlayer();
    this->synth = player->midiSynthesizer();

    mapChInstUI();
    setChInstDetails();

    ui->scrollArea->setWidgetResizable(false);
    this->adjustSize();
    this->setMinimumSize(970, height());
    this->setMaximumHeight(height());

    setBtnEqIcon(synth->equalizer31BandFX()->isOn());
    setBtnReverbIcon(synth->reverbFX()->isOn());
    setBtnChorusIcon(synth->chorusFX()->isOn());


    { // Settings
        QSettings st(CONFIG_SYNTH_FILE_PATH, QSettings::IniFormat);


        QSize size = st.value("Size", this->size()).toSize();
        resize(size);

        QList<int> splits; splits.append(700); splits.append(200);
        QList<int> splitterSize = st.value("SplitterSize", QVariant::fromValue(splits)).value<QList<int>>();
        int scroll1 = st.value("ScrollInstrument", 0).toInt();
        int scroll2 = st.value("ScrollBusGroup", 0).toInt();

        ui->splitter->setSizes(splitterSize);
        ui->scrollArea->horizontalScrollBar()->setValue(scroll1);
        ui->scrollArea_2->horizontalScrollBar()->setValue(scroll2);

        // Bus names -----------------------------------
        QStringList n1 = st.value("BusNames", QStringList()).toStringList();
        QStringList n2 = st.value("BusFullNames", QStringList()).toStringList();
        if (n1.count() == 16 && n2.count() == 16) {
            int start = static_cast<int>(InstrumentType::BusGroup1);
            for (int i=0; i<16; i++) {
                InstrumentType type = static_cast<InstrumentType>(start + i);
                chInstMap[type]->setInstrumentNames(n1[i], n2[i]);
            }
        }
        // --------------------------------------------

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


        st.beginReadArray("VSTiGroup");
        for (int i=0; i<4; i++)
        {
            st.setArrayIndex(i);

            QString      filePath    = st.value("VstiFilePath", "").toString();
            int          program     = st.value("VstiPrograms", 0).toInt();
            QList<float> params      = st.value("VstiParams").value<QList<float>>();

            DWORD vsti = synth->setVSTiFile(i, filePath);

            if (vsti != 0)
            {
                BASS_VST_SetProgram(vsti, program);
                FX::setVSTParams(vsti, params);
            }
        }
        st.endArray();


        st.beginReadArray("SynthMixer");
        for (InstrumentType t : chInstMap.keys())
        {
            st.setArrayIndex(static_cast<int>(t));
            int  b = st.value("Bus", -1).toInt();
            int ml = st.value("Volume", 50).toInt();
            bool m = st.value("Mute", false).toBool();
            bool s = st.value("Solo", false).toBool();
            int  v = st.value("VSTi", -1).toInt();

            InstCh * ich = chInstMap[t];
            ich->setSliderLevel(ml);
            ich->setMuteButton(m);
            ich->setSoloButton(s);

            LEDVu *vBar = ich->vuBar();
            vBar->setBackGroundColor(QColor(bg));
            vBar->setLedColorOn1(QColor(o1));
            vBar->setLedColorOn2(QColor(o2));
            vBar->setLedColorOn3(QColor(o3));
            vBar->setLedColorOff1(QColor(f1));
            vBar->setLedColorOff2(QColor(f2));
            vBar->setLedColorOff3(QColor(f3));
            vBar->setShowPeakHold(sph);
            vBar->setPeakHoldMs(phm);

            synth->setBusGroup(t, b);
            synth->setVolume(t, ml);
            synth->setMute(t, m);
            synth->setSolo(t, s);
            synth->setUseVSTi(t, v);
        }
        st.endArray();
    }

    #ifdef __linux__
    ui->btnVSTDirs->hide();
    #endif


    connect(ui->btnEq, SIGNAL(clicked()),
            mainWin->equalizer31BandDialog(), SLOT(show()));
    connect(ui->btnReverb, SIGNAL(clicked()),
            mainWin->reverbDialog(), SLOT(show()));
    connect(ui->btnChorus, SIGNAL(clicked()),
            mainWin->chorusDialog(), SLOT(show()));

    connect(mainWin->equalizer31BandDialog(), SIGNAL(switchChanged(bool)),
            this, SLOT(setBtnEqIcon(bool)));
    connect(mainWin->reverbDialog(), SIGNAL(switchChanged(bool)),
            this, SLOT(setBtnReverbIcon(bool)));

    connect(mainWin->chorusDialog(), SIGNAL(switchChanged(bool)),
            this, SLOT(setBtnChorusIcon(bool)));

    connect(&signalVstActionMapper, SIGNAL(mapped(QString)),
            this, SLOT(addFX(QString)));

    connect(&signalBusActionMapper, SIGNAL(mapped(int)),
            this, SLOT(setBusGroup(int)));
}

SynthMixerDialog::~SynthMixerDialog()
{
    // settings
    {
        QSettings st(CONFIG_SYNTH_FILE_PATH, QSettings::IniFormat);
        st.setValue("Size", this->size());
        st.setValue("SplitterSize", QVariant::fromValue(ui->splitter->sizes()));
        st.setValue("ScrollInstrument", ui->scrollArea->horizontalScrollBar()->value());
        st.setValue("ScrollBusGroup", ui->scrollArea_2->horizontalScrollBar()->value());

        LEDVu *vu = chInstMap.first()->vuBar();
        st.setValue("LedColorOn1", vu->ledColorOn1().name());
        st.setValue("LedColorOn2", vu->ledColorOn2().name());
        st.setValue("LedColorOn3", vu->ledColorOn3().name());
        st.setValue("LedColorOff1", vu->ledColorOff1().name());
        st.setValue("LedColorOff2", vu->ledColorOff2().name());
        st.setValue("LedColorOff3", vu->ledColorOff3().name());
        st.setValue("ShowPeakHold", vu->isShowPeakHold());
        st.setValue("PeakHoldMs", vu->peakHoldMs());


        st.beginWriteArray("VSTiGroup");
        for (int i=0; i<4; i++)
        {
            st.setArrayIndex(i);

            DWORD vsti = synth->vstiHandle(i);

            if (vsti == 0)
            {
                st.setValue("VstiFilePath", "");
                st.setValue("VstiPrograms", 0);
                st.setValue("VstiParams", QVariant::fromValue(QList<float>()));
            }
            else
            {
                st.setValue("VstiFilePath", synth->vstiFile(i));
                st.setValue("VstiPrograms", BASS_VST_GetProgram(vsti));
                st.setValue("VstiParams", QVariant::fromValue(FX::getVSTParams(vsti)));
            }
        }
        st.endArray();


        QStringList busNames;
        QStringList busFullNames;
        int start = static_cast<int>(InstrumentType::BusGroup1);
        for (int i=0; i<16; i++) {
            InstrumentType type = static_cast<InstrumentType>(start + i);
            busNames.append(chInstMap[type]->instrumentName());
            busFullNames.append(chInstMap[type]->fullInstrumentName());
        }
        st.setValue("BusNames", busNames);
        st.setValue("BusFullNames", busFullNames);


        st.beginWriteArray("SynthMixer");
        for (InstrumentType t : chInstMap.keys())
        {
            st.setArrayIndex(static_cast<int>(t));
            st.setValue("Volume", synth->volume(t));
            st.setValue("Mute", synth->isMute(t));
            st.setValue("Solo", synth->isSolo(t));
            st.setValue("Bus", synth->busGroup(t));
            st.setValue("VSTi", synth->useVSTi(t));

            QVariant v = QVariant::fromValue(synth->fxUids(t));
            st.setValue("VstUid", v);

            QVariant bypass = QVariant::fromValue(synth->fxBypass(t));
            st.setValue("VstBypass", bypass);

            QVariant vstPrograms = QVariant::fromValue(synth->fxProgram(t));
            st.setValue("VstPrograms", vstPrograms);

            QVariant vstParams = QVariant::fromValue(synth->fxParams(t));
            st.setValue("VstParams", vstParams);
        }
        st.endArray();
    }
    // ------------------------

    chInstMap.clear();

    for (QMenu *menu : vstVendorMenus) {
        menu->clear();
        delete menu;
    }
    vstVendorMenus.clear();

    if (signalBFXActionMapper != nullptr)
        delete signalBFXActionMapper;

    delete ui;
}

#ifndef __linux__

void SynthMixerDialog::setVSTVendorMenu()
{
    for (QMenu *menu : vstVendorMenus) {
        menu->clear();
        delete menu;
    }
    vstVendorMenus.clear();

    QList<QString> vendors;

    for (const VSTNamePath &vst : synth->VSTList().values()) {
        int index = vendors.indexOf(vst.vstvendor);
        if (index == -1) {
            vendors.append(vst.vstvendor);
            QMenu *menu = new QMenu(vst.vstvendor);
            vstVendorMenus.append(menu);
            index = vendors.count() - 1;
        }
        QAction *action = vstVendorMenus[index]->addAction(vst.vstName);
        connect(action, SIGNAL(triggered()), &signalVstActionMapper, SLOT(map()));
        signalVstActionMapper.setMapping(action, QString::number(vst.uniqueID));
    }
}

#endif

void SynthMixerDialog::setFXToSynth()
{
    QSettings st(CONFIG_SYNTH_FILE_PATH, QSettings::IniFormat);
    st.beginReadArray("SynthMixer");
    for (InstrumentType t : chInstMap.keys())
    {
        st.setArrayIndex(static_cast<int>(t));

        QList<uint> vstUid = st.value("VstUid").value<QList<uint>>();
        QList<bool> vstBypass = st.value("VstBypass").value<QList<bool>>();
        QList<QList<float>> vstParams = st.value("VstParams").value<QList<QList<float>>>();
        QList<int> vstPrograms = st.value("VstPrograms").value<QList<int>>();

        this->currentType = t;

        for (int i=0; i<vstUid.count(); i++)
        {
            FX *fx = this->addFX(QString::number(vstUid[i]), vstBypass[i]);

            if (fx == nullptr)
                continue;

            fx->setProgram(vstPrograms[i]);
            fx->setParams(vstParams[i]);
        }

    }
    st.endArray();
}

InstCh *SynthMixerDialog::mixChannel(InstrumentType t)
{
    return chInstMap[t];
}

QMap<InstrumentType, InstCh *> SynthMixerDialog::mixChannelMap()
{
    return chInstMap;
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

    if (t < InstrumentType::BusGroup1)
        return;

    for (InstrumentType type : synth->instrumentMap().keys()) {
        if (synth->instrument(type).bus != (static_cast<int>(t) - synth->HANDLE_MIDI_COUNT))
            continue;
        synth->setMute(type, m);
        chInstMap[type]->setMuteButton(m);
    }
}

void SynthMixerDialog::setSolo(InstrumentType t, bool s)
{
    synth->setSolo(t, s);

    if (t < InstrumentType::BusGroup1)
        return;

    for (InstrumentType type : synth->instrumentMap().keys()) {
        if (synth->instrument(type).bus != (static_cast<int>(t) - synth->HANDLE_MIDI_COUNT))
            continue;
        synth->setSolo(type, s);
        chInstMap[type]->setSoloButton(s);
    }
}

void SynthMixerDialog::setMixLevel(InstrumentType t, int level)
{
    synth->setVolume(t, level);
}

void SynthMixerDialog::resetMixLevel(InstrumentType t)
{
    chInstMap[t]->setSliderLevel(50);
    synth->setVolume(t, 50);
}

void SynthMixerDialog::showPeakVU(InstrumentType t, int bus,  int ch, int note, int velocity)
{
    chInstMap[t]->peak(velocity);
    if(bus > -1) {
        InstrumentType bType = static_cast<InstrumentType>(bus + synth->HANDLE_MIDI_COUNT);
        chInstMap[bType]->peak(velocity);
    }
}

void SynthMixerDialog::showEvent(QShowEvent *)
{
    connect(synth, SIGNAL(noteOnSended(InstrumentType,int,int,int,int)),
            this, SLOT(showPeakVU(InstrumentType,int,int,int,int)));
}

void SynthMixerDialog::hideEvent(QHideEvent *event)
{
    disconnect(synth, SIGNAL(noteOnSended(InstrumentType,int,int,int,int)),
            this, SLOT(showPeakVU(InstrumentType,int,int,int,int)));
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

    chInstMap[InstrumentType::HighTom]      = ui->ch_30;
    chInstMap[InstrumentType::MidTom]       = ui->ch_31;
    chInstMap[InstrumentType::LowTom]       = ui->ch_32;

    chInstMap[InstrumentType::Hihat]        = ui->ch_33;
    chInstMap[InstrumentType::Cowbell]      = ui->ch_34;
    chInstMap[InstrumentType::CrashCymbal]  = ui->ch_35;
    chInstMap[InstrumentType::RideCymbal]   = ui->ch_36;
    chInstMap[InstrumentType::Bongo]        = ui->ch_37;
    chInstMap[InstrumentType::Conga]        = ui->ch_38;
    chInstMap[InstrumentType::Timbale]      = ui->ch_39;
    chInstMap[InstrumentType::SmallCupShapedCymbals] = ui->ch_40;
    chInstMap[InstrumentType::ThaiChap]     = ui->ch_41;
    chInstMap[InstrumentType::PercussionEtc] = ui->ch_42;

    chInstMap[InstrumentType::VSTi1]        = ui->ch_43;
    chInstMap[InstrumentType::VSTi2]        = ui->ch_44;
    chInstMap[InstrumentType::VSTi3]        = ui->ch_45;
    chInstMap[InstrumentType::VSTi4]        = ui->ch_46;

    chInstMap[InstrumentType::BusGroup1]    = ui->ch_47;
    chInstMap[InstrumentType::BusGroup2]    = ui->ch_48;
    chInstMap[InstrumentType::BusGroup3]    = ui->ch_49;
    chInstMap[InstrumentType::BusGroup4]    = ui->ch_50;
    chInstMap[InstrumentType::BusGroup5]    = ui->ch_51;
    chInstMap[InstrumentType::BusGroup6]    = ui->ch_52;
    chInstMap[InstrumentType::BusGroup7]    = ui->ch_53;
    chInstMap[InstrumentType::BusGroup8]    = ui->ch_54;
    chInstMap[InstrumentType::BusGroup9]    = ui->ch_55;
    chInstMap[InstrumentType::BusGroup10]   = ui->ch_56;
    chInstMap[InstrumentType::BusGroup11]   = ui->ch_57;
    chInstMap[InstrumentType::BusGroup12]   = ui->ch_58;
    chInstMap[InstrumentType::BusGroup13]   = ui->ch_59;
    chInstMap[InstrumentType::BusGroup14]   = ui->ch_60;
    chInstMap[InstrumentType::BusGroup15]   = ui->ch_61;
    chInstMap[InstrumentType::BusGroup16]   = ui->ch_62;
}

void SynthMixerDialog::setChInstDetails()
{
    // 62
    QString names[62] = { "Piano", "Organ", "Accordn", "Chmt.P", "Percsve",
                        "Bass", "Nylon", "Steel", "Jazz", "Clean",
                        "Overdrv", "Distrn", "Harmncs", "Trumpet", "Brass",
                        "S.Brass", "Saxphn", "Reed", "Pipe", "Strings",
                        "Ensembl", "SynthL", "SynthP", "SynthE", "Ethnic", "SoundFX",
                        "Kick", "Snare", "S.Stick", "HighTom", "MidTom",
                        "LowTom", "Hihat", "Cowbell", "Crash", "Ride",
                        "Bongo", "Conga", "Timbale", "ฉิ่ง", "ฉาบ", "PercEtc",

                        "VSTi 1", "VSTi 2", "VSTi 3", "VSTi 4",

                        "Bus 1", "Bus 2", "Bus 3", "Bus 4",
                        "Bus 5", "Bus 6", "Bus 7", "Bus 8",
                        "Bus 9", "Bus 10", "Bus 11", "Bus 12",
                        "Bus 13", "Bus 14", "Bus 15", "Bus 16" };

    QString tooltips[62] = { "Piano", "Organ", "Accordion", "Chromatic Percussion",
                             "Percussive", "Bass", "Acoustic Guitar (nylon)",
                             "Acoustic Guitar (steel)", "Electric Guitar (jazz)",
                             "Electric Guitar (clean)", "Overdriven Guitar",
                             "Distortion Guitar", "Harmonics Guitar", "Trumpet",
                             "Brass", "Synth Brass", "Saxophone", "Reed", "Pipe",
                             "Strings", "Ensemble", "Synth Lead", "Synth Pad",
                             "Synth Effects", "Ethnic", "Sound effects",
                             "Kick (Bass Drum)", "Snare", "Side Stick/Rimshot",
                             "High Tom", "Mid Tom", "Low Tom", "Hi-hat", "Cowbell",
                             "Crash Cymbal", "Ride Cymbal", "Bongo", "Conga",
                             "Timbale", "ฉิ่ง / Triangle",
                             "ฉาบ", "Percussion Etc.",

                             "VST instruments 1", "VST instruments 2",
                             "VST instruments 3", "VST instruments 4",

                             "Bus Group 1", "Bus Group 2", "Bus Group 3", "Bus Group 4",
                             "Bus Group 5", "Bus Group 6", "Bus Group 7", "Bus Group 8",
                             "Bus Group 9", "Bus Group 10", "Bus Group 11", "Bus Group 12",
                             "Bus Group 13", "Bus Group 14", "Bus Group 15", "Bus Group 16"};

    QString rc = ":/Icons/Instruments/";
    QString imgs[62] = { "Piano.png", "Organ.png", "Accordion.png",
                         "Chromatic Percussion.png", "Percussive.png",
                         "Bass.png", "Nylon.png", "Steel.png", "Jazz.png",
                         "Clean.png", "Overdriven.png", "Distortion.png",
                         "Harmonics.png", "Trumpet.png", "Brass.png",
                         "Synth Brass.png", "Saxophone.png", "Reed.png",
                         "Pipe.png", "Strings.png", "Ensemble.png",
                         "Synth Lead.png", "Synth Pad.png", "Synth Effects.png",
                         "Ethnic.png", "FX.png",
                         "Kick.png", "Snare.png", "Side Stick.png",
                         "High Tom.png", "Mid Tom.png", "Low Tom.png",
                         "Hihat.png", "Cowbell.png", "Crash.png", "Ride.png",
                         "Bongo.png", "Conga.png", "Timbale.png", "Ching.png",
                         "Chab.png", "Percussion Etc.png",

                         "vst.png", "vst.png", "vst.png", "vst.png",

                         "bs1.png", "bs2.png", "bs3.png", "bs4.png",
                         "bs5.png", "bs6.png", "bs7.png", "bs8.png",
                         "bs9.png", "bs10.png", "bs11.png", "bs12.png",
                         "bs13.png", "bs14.png", "bs15.png", "bs16.png" };


    for (int i=0; i<synth->HANDLE_STREAM_COUNT; i++)
    {
        InstCh *ich = chInstMap.values()[i];

        ich->setInstrumentType(chInstMap.keys()[i]);
        ich->setInstrumentNames(names[i], tooltips[i]);

        ich->setInstrumentImage(QImage(rc + imgs[i]));

        connect(ich, SIGNAL(muteChanged(InstrumentType,bool)),
                this, SLOT(setMute(InstrumentType,bool)));

        connect(ich, SIGNAL(soloChanged(InstrumentType,bool)),
                this, SLOT(setSolo(InstrumentType,bool)));

        connect(ich, SIGNAL(sliderLevelChanged(InstrumentType,int)),
                this, SLOT(setMixLevel(InstrumentType,int)));

        connect(ich, SIGNAL(sliderDoubleClicked(InstrumentType)),
                this, SLOT(resetMixLevel(InstrumentType)));

        connect(ich, SIGNAL(menuRequested(InstrumentType,QPoint)),
                this, SLOT(showChannelMenu(InstrumentType,QPoint)));

        connect(ich, SIGNAL(fxByPassChanged(InstrumentType,int,bool)),
                this, SLOT(byPassFX(InstrumentType,int,bool)));

        connect(ich, SIGNAL(fxDoubleClicked(InstrumentType,int)),
                this, SLOT(showFxDialog(InstrumentType,int)));

        connect(ich, SIGNAL(fxRemoveMenuRequested(InstrumentType,int,QPoint)),
                this, SLOT(showFXRemoveMenu(InstrumentType,int,QPoint)));
    }
}

void SynthMixerDialog::createBusActions(InstrumentType t, QMenu *busMenu)
{
    QList<QString> names;
    names.append("Master (Default)");
    int start = static_cast<int>(InstrumentType::BusGroup1);
    for (int i=0; i<16; i++) {
        InstrumentType type = static_cast<InstrumentType>(start + i);
        names.append(chInstMap[type]->fullInstrumentName());
    }

    for (int i=0; i<17; i++) {
        QAction *act = busMenu->addAction(names[i]);
        connect(act, SIGNAL(triggered()), &signalBusActionMapper, SLOT(map()));
        signalBusActionMapper.setMapping(act, i-1);
        if (synth->instrument(t).bus == (i-1)) {
            act->setCheckable(true);
            act->setChecked(true);
        }
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

void SynthMixerDialog::showChannelMenu(InstrumentType type, const QPoint &pos)
{
    currentType = type;

    QMenu menu(this);

    // VST
    #ifndef __linux__
    QMenu *vstMenu = menu.addMenu("VST Effects");

    for (QMenu *m : vstVendorMenus) {
        vstMenu->addMenu(m);
    }
    #endif

    // Built-in FX
    {
        if (signalBFXActionMapper != nullptr)
            delete signalBFXActionMapper;

        signalBFXActionMapper = new QSignalMapper(this);

        menu.addSeparator();

        QMenu *fxMenu = menu.addMenu("Built-in Effects");

        for (int i=0; i<BUILTIN_FX_COUNT; i++)
        {
            QAction *act1 = fxMenu->addAction(BUILTIN_FX_NAMES[i]);
            connect(act1, SIGNAL(triggered()), signalBFXActionMapper, SLOT(map()));
            signalBFXActionMapper->setMapping(act1, QString::number(i));
        }
    }

    // Bus
    if (static_cast<int>(type) < synth->HANDLE_MIDI_COUNT)
    {
        menu.addSeparator();
        QMenu *busMenu = menu.addMenu("Bus Group");
        createBusActions(type, busMenu);
    }

    connect(signalBFXActionMapper, SIGNAL(mapped(QString)), this, SLOT(addFX(QString)));

    menu.exec(chInstMap[type]->mapToGlobal(pos));
}

void SynthMixerDialog::setBusGroup(int group)
{
    synth->setBusGroup(currentType, group);
}

FX* SynthMixerDialog::addFX(const QString &uidStr, bool bypass)
{
    uint uid = uidStr.toUInt();
    VSTNamePath vst = synth->VSTList()[uid];
    Instrument inst = synth->instrument(currentType);

    FX *fx = synth->addFX(currentType, uid);

    if (fx == nullptr)
        return fx;

    int i = inst.FXs.count();
    synth->setFXBypass(currentType, i, bypass);

    QString name;
    if (uid < BUILTIN_FX_COUNT)
        name = BUILTIN_FX_NAMES[uid];
    else
        name = vst.vstName;

    chInstMap[currentType]->addFXLabel(name, inst.FXs.count(), bypass);

    return fx;
}

void SynthMixerDialog::byPassFX(InstrumentType type, int fxIndex, bool bypass)
{
    synth->setFXBypass(type, fxIndex, bypass);
}

void SynthMixerDialog::showFxDialog(InstrumentType type, int fxIndex)
{
    FX *fx = synth->instrument(type).FXs[fxIndex];

    if (fx->fxType() == FXType::VSTEffects)
    {
        if (!synth->isOpened())
            return;

        #ifndef __linux__
        if (fx->fxHandle() != 0)
        {
            VSTDialog *dlg = new VSTDialog(this, fx->fxHandle(), chInstMap[type]->fullInstrumentName());

            if (!dlg->isCanOpen())
            {
                delete dlg;
                return;
            }

            dlg->setAttribute(Qt::WA_DeleteOnClose);
            dlg->show();
        }
        #endif
    }
    else
    {   
        QDialog *d = nullptr;

        switch (fx->fxType()) {
        case FXType::AutoWah:
            d = new AutoWahFXDialog(this, dynamic_cast<AutoWahFX*>(fx));
            break;
        case FXType::Chorus:
            d = new ChorusFXDialog(this, dynamic_cast<ChorusFX*>(fx));
            break;
        case FXType::Compressor:
            d = new CompressorFXDialog(this, dynamic_cast<CompressorFX*>(fx));
            break;
        case FXType::Distortion:
            d = new DistortionFXDialog(this, dynamic_cast<DistortionFX*>(fx));
            break;
        case FXType::Echo:
            d = new EchoFXDialog(this, dynamic_cast<EchoFX*>(fx));
            break;
        case FXType::EQ15Band:
            d = new EQ15BandDialog(this, dynamic_cast<Equalizer15BandFX*>(fx));
            break;
        case FXType::EQ31Band:
            d = new EQ31BandDialog(this, dynamic_cast<Equalizer31BandFX*>(fx));
            break;
        case FXType::Reverb:
            d = new ReverbFXDialog(this, dynamic_cast<ReverbFX*>(fx));
            break;
        }

        if (d != nullptr)
        {
            int i = static_cast<int>(fx->fxType());
            d->setWindowTitle(BUILTIN_FX_NAMES[i] + "  [" + chInstMap[type]->fullInstrumentName() + "]");
            d->adjustSize();
            d->setFixedSize(d->size());
            d->setAttribute(Qt::WA_DeleteOnClose);
            d->show();
        }
    }
}

void SynthMixerDialog::showFXRemoveMenu(InstrumentType type, int fxIndex, const QPoint &pos)
{
    currentType = type;
    currentFxIndexToRemove = fxIndex;

    QMenu menu(this);

    menu.addAction("Remove", this, SLOT(removeFX()));

    menu.exec(chInstMap[type]->mapToGlobal(pos));
}

void SynthMixerDialog::removeFX()
{
    if (synth->removeFX(currentType, currentFxIndexToRemove))
    {
        chInstMap[currentType]->removeVSTLabel(currentFxIndexToRemove);
    }
}

void SynthMixerDialog::on_btnBus_clicked()
{
    BusDialog dlg(this, &chInstMap, synth);
    dlg.setModal(true);
    dlg.adjustSize();
    dlg.exec();
}

void SynthMixerDialog::on_btnVSTDirs_clicked()
{
    #ifndef __linux__
    VSTDirsDialog dlg(this, mainWin);
    dlg.setModal(true);
    dlg.adjustSize();
    dlg.setMinimumSize(dlg.size());
    dlg.exec();
    #endif
}
