#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "Config.h"
#include "Utils.h"
#include "DrumPadsKey.h"
#include "SettingsDialog.h"
#include "Midi/MidiFile.h"
#include "Midi/HNKFile.h"
#include "Dialogs/AboutDialog.h"
#include "Dialogs/MapSoundfontDialog.h"
#include "Dialogs/MapChannelDialog.h"

#include <QTime>
#include <QMenu>
#include <QCloseEvent>
#include <QMessageBox>
#include <QDir>
#include <QDirIterator>
#include <QFileDialog>
#include <QWindow>

#include <QDebug>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    // List audio device and Init BASS
    {
        QMap<int, QString> dvs;
        int a, count=0;
        BASS_DEVICEINFO info;
        for (a=0; BASS_GetDeviceInfo(a, &info); a++) {
            if (info.flags&BASS_DEVICE_ENABLED) { // device is enabled
                BASS_Init(a, 44100, BASS_DEVICE_SPEAKERS, NULL, NULL);
                dvs[a] =  QString(info.name);
                count++; // count it
            }
        }
        MidiSynthesizer::audioDevices(dvs);
    }

    BASS_SetDevice(1);
    BASS_FX_GetVersion();

    auto concurentThreadsSupported = Utils::concurentThreadsSupported();
    float nVoices = (concurentThreadsSupported > 1) ? 500 : 256;

    BASS_SetConfig(BASS_CONFIG_BUFFER, 100);
    BASS_SetConfig(BASS_CONFIG_UPDATEPERIOD, 5);
    BASS_SetConfig(BASS_CONFIG_MIDI_VOICES, nVoices);
    BASS_SetConfig(BASS_CONFIG_MIDI_COMPACT, true);
    // End Init BASS


    lyrWidget = new LyricsWidget(this);
    updateDetail = new Detail(this);

    ui->setupUi(this);

    #ifdef _WIN32
    taskbarButton = new QWinTaskbarButton();
    #endif

    settings = new QSettings(CONFIG_APP_FILE_PATH, QSettings::IniFormat);

    QString ncn = settings->value("NCNPath", QDir::currentPath() + "/Songs/NCN").toString();
    QString hnk = settings->value("HNKPath", QDir::currentPath() + "/Songs/HNK").toString();

    db = new SongDatabase();
    db->setNcnPath(ncn);
    db->setHNKPath(hnk);


    timer1 = new QTimer();
    timer2 = new QTimer();
    timer2->setSingleShot(true);

    positionTimer = new QTimer(this);
    positionTimer->setInterval(30);

    lyricsTimer = new QTimer(0);
    lyricsTimer->setInterval(30);

    detailTimer = new QTimer();
    detailTimer->setSingleShot(true);

    player = new MidiPlayer();

    locale = QLocale(QLocale::English, QLocale::UnitedStates);

    { // Channel Mixer
        bool lck = settings->value("ChMixLock", false).toBool();

        ui->chMix->setLock(lck);
        ui->chMix->setPlayer(player);

        if (lck) {
            ui->chMix->setLock(true);
            ui->chMix->show();
            ui->expandChMix->show();
        }
        else {
            ui->chMix->setLock(false);
            ui->chMix->hide();
            ui->expandChMix->hide();
        }

        connect(ui->chMix, SIGNAL(lockChanged(bool)), this, SLOT(onChMixLockChanged(bool)));
        connect(ui->chMix, SIGNAL(mouseLeaved()), this, SLOT(onChMixMouseLeaved()));
    }


    { // UI window size
        int sTime = settings->value("SearchTimeout", 5).toInt();
        int pTime = settings->value("PlaylistTimeout", 5).toInt();
        setSearchTimeout(sTime);
        setPlaylistTimeout(pTime);

        bool removeList  = settings->value("RemoveFromPlaylist", true).toBool();
        bool aPlayNext   = settings->value("AutoPlayNext", true).toBool();
        remove_playlist = removeList;
        auto_playnext = aPlayNext;

        int bg = settings->value("BackgroundType", 0).toInt();
        if (bg == 0) {
            QString color = settings->value("BackgroundColor", "#525252").toString();
            setBackgroundColor(color);
        } else {
            QString img = settings->value("BackgroundImage", "").toString();
            if (img != "" && QFile::exists(img)) {
                setBackgroundImage(img);
            } else {
                QString color = settings->value("BackgroundColor", "#525252").toString();
                setBackgroundColor(color);
            }
        }

        int w       = settings->value("WindowWidth", this->minimumWidth()).toInt();
        int h       = settings->value("WindowHeight", this->minimumHeight()).toInt();
        bool maximum= settings->value("WindowMaximized", false).toBool();
        bool full   = settings->value("WindowFullScreen", false).toBool();

        this->resize(w, h);
        if (maximum) {
            this->showMaximized();
        } else {
            if (full) {
                this->showFullScreen();
            }
        }
    }


    { // Player
        int oPort   = settings->value("MidiOut", 0).toInt();
        int iPort   = settings->value("MidiIn", -1).toInt();
        int vl      = settings->value("MidiVolume", 50).toInt();
        bool lDrum  = settings->value("MidiLockDrum", false).toBool();
        bool lSnare = settings->value("MidiLockSnare", false).toBool();
        bool lBass  = settings->value("MidiLockBass", false).toBool();
        int synBuf  = settings->value("SynthBuffer", 100).toInt();

        BASS_SetConfig(BASS_CONFIG_BUFFER, synBuf);
        player->setMidiOut(oPort);
        player->setMidiIn(iPort);
        player->setVolume(vl);

        if (lDrum) {
            int ldNum = settings->value("MidiLockDrumNumber", 0).toInt();
            player->setLockDrum(true, ldNum);
        }
        if (lSnare) {
            int lsNum = settings->value("MidiLockSnareNumber", 38).toInt();
            player->setLockSnare(true, lsNum);
        }
        if (lBass) {
            int lbNum = settings->value("MidiLockBassNumber", 32).toInt();
            player->setLockBass(true, lbNum);
        }

        // Midi Channel Mapper
        QList<int> ports = settings->value("MidiChannelMapper").value<QList<int>>();
        if (ports.count() == 16) {
            for (int i=0; i<16; i++) {
                player->setMapChannelOutput(i, ports[i]);
            }
        }

        connect(player, SIGNAL(finished()), this, SLOT(onPlayerThreadFinished()));
        connect(player, SIGNAL(bpmChanged(int)), ui->rhmWidget, SLOT(setBpm(int)));
    }


    { // Synth
        MidiSynthesizer *synth = player->midiSynthesizer();

        // Audio out
        int aout = settings->value("SynthDefaultDevice", 1).toInt();
        synth->setDefaultDevice(aout);

        // floating and fx
        bool useFloat = settings->value("SynthFloatPoint", true).toBool();
        bool useFX = settings->value("SynthUseFXRC", false).toBool();
        synth->setUsetFloattingPoint(useFloat);
        synth->setUseFXRC(useFX);

        // Soundfonts and Soundfonts map
        // move to setup in main function (main.cpp)

        // Synth EQ
        auto eqs = synth->equalizer31BandFXs();
        std::map<EQFrequency31Range, float> eqgain = eqs[0]->gain();

        bool eqon = settings->value("SynthFXEQOn", false).toBool();
        if (eqon)
            for (auto eq : eqs)
                eq->on();

        int gi =0;
        settings->beginReadArray("SynthFXEQGain");
        for (const auto& g : eqgain) {
            settings->setArrayIndex(gi);
            float gain = settings->value("gain", 0.0f).toFloat();
            for (auto eq : eqs)
                eq->setGain(g.first, gain);
            gi++;
        }
        settings->endArray();


        // Synth reverb
        bool rvOn   = settings->value("SynthFXReverbOn", false).toBool();
        int rvGain  = settings->value("SynthFXReverbInGain", 0).toInt();
        int rvMix   = settings->value("SynthFXReverbMix", 0).toInt();
        int rvTime  = settings->value("SynthFXReverbTime", 1000).toInt();
        float rvHF  = settings->value("SynthFXReverbHF", 0.001).toFloat();

        if (rvOn)
            for (auto reverb : synth->reverbFXs())
                reverb->on();

        for (auto reverb : synth->reverbFXs())
        {
            reverb->setInGain((float)rvGain);
            reverb->setReverbMix((float)rvMix);
            reverb->setReverbTime((float)rvTime);
            reverb->setHighFreqRTRatio(rvHF);
        }


        // Synth chorus
        bool cOn = settings->value("SynthFXChorusOn", false).toBool();

        int cWf  = settings->value("SynthFXChorusWaveform", 1).toInt();
        int cPh  = settings->value("SynthFXChorusPhase", 3).toInt();

        int cWet = settings->value("SynthFXChorusWetDryMix", 50).toInt();
        int cDep = settings->value("SynthFXChorusDepth", 10).toInt();
        int cFb  = settings->value("SynthFXChorusFeedback", 25).toInt();
        int cFq  = settings->value("SynthFXChorusFrequency", 1).toInt();
        int cDl  = settings->value("SynthFXChorusDelay", 16).toInt();

        WaveformType lWaveform = static_cast<WaveformType>(cWf);
        PhaseType lPhase = static_cast<PhaseType>(cPh);

        if (cOn)
            for (auto chorus : synth->chorusFXs())
                chorus->on();

        for (auto chorus : synth->chorusFXs())
        {
            chorus->setWaveform(lWaveform);
            chorus->setPhase(lPhase);
            chorus->setWetDryMix((float)cWet);
            chorus->setDepth((float)cDep);
            chorus->setFeedback((float)cFb);
            chorus->setFrequency((float)cFq);
            chorus->setDelay((float)cDl);
        }

        // Create synth mixer
        synthMix = new SynthMixerDialog(this, this);
    }


    { // Lyrics
        QString family  = settings->value("LyricsFamily", font().family()).toString();
        int size        = settings->value("LyricsSize", 40).toInt();
        int weight      = settings->value("LyricsWeight", 75).toInt();
        bool italic     = settings->value("LyricsItalic", false).toBool();
        bool strikeOut  = settings->value("LyricsStrikeOut", false).toBool();
        bool underline  = settings->value("LyricsUnderline", false).toBool();

        QString tColor  = settings->value("LyricsTextColor", "#f3f378").toString();
        QString tbColor = settings->value("LyricsTextBorderColor", "#000000").toString();
        QString toColor = settings->value("LyricsTextBorderOutColor", "#000000").toString();
        int     tbWidth = settings->value("LyricsTextBorderWidth", 3).toInt();
        int     toWidth = settings->value("LyricsTextBorderOutWidth", 2).toInt();

        QString cColor  = settings->value("LyricsCurColor", "#0000ff").toString();
        QString cbColor = settings->value("LyricsCurBorderColor", "#ffffff").toString();
        QString coColor = settings->value("LyricsCurBorderOutColor", "#000000").toString();
        int     cbWidth = settings->value("LyricsCurBorderWidth", 4).toInt();
        int     coWidth = settings->value("LyricsCurBorderOutWidth", 2).toInt();

        int     line1X  = settings->value("LyricsLine1X", 0).toInt();
        int     line2X  = settings->value("LyricsLine2X", 0).toInt();
        int     line1Y  = settings->value("LyricsLine1Y", 320).toInt();
        int     line2Y  = settings->value("LyricsLine2Y", 170).toInt();
        int     aTime   = settings->value("LyricsAnimationTime", 250).toInt();
        bool   autosize = settings->value("LyricsAutoFontSize", true).toBool();

        QFont f;
        f.setFamily(family);
        f.setPointSize(size);
        f.setWeight(weight);
        f.setItalic(italic);
        f.setStrikeOut(strikeOut);
        f.setUnderline(underline);

        lyrWidget->setTextFont(f);
        lyrWidget->setTextColor(QColor(tColor));
        lyrWidget->setTextBorderColor(QColor(tbColor));
        lyrWidget->setTextBorderOutColor(QColor(toColor));
        lyrWidget->setTextBorderWidth(tbWidth);
        lyrWidget->setTextBorderOutWidth(toWidth);

        lyrWidget->setCurColor(QColor(cColor));
        lyrWidget->setCurBorderColor(QColor(cbColor));
        lyrWidget->setCurBorderOutColor(QColor(coColor));
        lyrWidget->setCurBorderWidth(cbWidth);
        lyrWidget->setCurBorderOutWidth(coWidth);

        lyrWidget->setLine1Position(static_cast<LinePosition>(line1X));
        lyrWidget->setLine2Position(static_cast<LinePosition>(line2X));
        lyrWidget->setLine1Y(line1Y);
        lyrWidget->setLine2Y(line2Y);
        lyrWidget->setAnimationTime(aTime);
        lyrWidget->setAutoFontSize(autosize);
    }


    { // Init UI
        this->setWindowTitle(QApplication::applicationName() + "  " + QApplication::applicationVersion());

        Utils::LAST_OPEN_DIR = settings->value("LastOpenDir", QDir::homePath()).toString();

        updateDetail->hide();
        updateDetail->resize(250, 60);
        updateDetail->setText("กำลังปรับปรุงฐานข้อมูล");

        ui->detail->hide();
        ui->frameSearch->hide();
        ui->playlistWidget->hide();
        ui->songDetail->hide();
        ui->sliderVolume->setValue(player->volume());

        // 249
        ui->playlistWidget->setMinimumHeight(305);
        ui->playlistWidget->setMaximumHeight(305);

        { // Playback control
            ui->btnPlay->setIconFiles(":/Icons/play-white", ":/Icons/play-blue");
            ui->btnPause->setIconFiles(":/Icons/pause-white", ":/Icons/pause-blue");
            ui->btnStop->setIconFiles(":/Icons/stop-white", ":/Icons/stop-blue");
            ui->btnPrevious->setIconFiles(":/Icons/previous-white", ":/Icons/previous-blue");
            ui->btnNext->setIconFiles(":/Icons/next-white", ":/Icons/next-blue");

            ui->btnVolumeMute->setIconFiles(":/Icons/volume-white", ":/Icons/volume-blue");
        }

        #ifdef __linux__
        { // UI Font in linux
            QString fontName = "TH Sarabun New Bold";
            QFont f = ui->lbSearch->font();
            f.setBold(true);
            f.setFamily(fontName);
            f.setPointSize(f.pointSize() - 10);

            ui->lbSearch->setFont(f);
            ui->lbArtist->setFont(f);
            ui->lbBtw->setFont(f);
            ui->lbId->setFont(f);
            ui->lbName->setFont(f);
            ui->lbTempoKey->setFont(f);
            ui->lbType->setFont(f);

            ui->lbPlaylist->setFont(f);

            f.setPointSize(f.pointSize() - 2);
            ui->playlist->setFont(f);

            f.setPointSize(f.pointSize() - 4);
            ui->lbLyrics->setFont(f);
        }
        #endif

        connect(db, SIGNAL(started()), updateDetail, SLOT(show()));
        connect(db, SIGNAL(finished()), updateDetail, SLOT(hide()));
        connect(db, SIGNAL(updatePositionChanged(int)), this, SLOT(onDbUpdateChanged(int)));

        connect(timer1, SIGNAL(timeout()), this, SLOT(showCurrentTime()));
        connect(timer2, SIGNAL(timeout()), this, SLOT(hideUIFrame()));

        connect(positionTimer, SIGNAL(timeout()), this, SLOT(onPositiomTimerTimeOut()));
        connect(lyricsTimer, SIGNAL(timeout()), this, SLOT(onLyricsTimerTimeOut()));

        connect(detailTimer, SIGNAL(timeout()), this, SLOT(onDetailTimerTimeout()));

        connect(ui->btnPause, SIGNAL(clicked()), this, SLOT(pause()));
        connect(ui->btnStop, SIGNAL(clicked()), this, SLOT(stop()));
        connect(ui->btnNext, SIGNAL(clicked()), this, SLOT(playNext()));
        connect(ui->btnPrevious, SIGNAL(clicked()), this, SLOT(playPrevious()));
        connect(ui->sliderPosition, SIGNAL(sliderPressed()), this, SLOT(onSliderPositionPressed()));
        connect(ui->sliderPosition, SIGNAL(sliderReleased()), this, SLOT(onSliderPositionReleased()));
        connect(ui->sliderVolume, SIGNAL(valueChanged(int)), this, SLOT(onSliderVolumeValueChanged(int)));

        timer1->start(1000);
    }


    // Menu
    connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showContextMenu(const QPoint&)));
}

MainWindow::~MainWindow()
{
    stop();

    { // Write synth FX settings
        // Synth EQ
        Equalizer31BandFX *eq = player->midiSynthesizer()->equalizer31BandFXs()[0];
        std::map<EQFrequency31Range, float> eqgain = eq->gain();

        settings->setValue("SynthFXEQOn", eq->isOn());

        int gi =0;
        settings->beginWriteArray("SynthFXEQGain");
        for (const auto& g : eqgain) {
            settings->setArrayIndex(gi);
            settings->setValue("gain", g.second);
            gi++;
        }
        settings->endArray();

        // Synth reverb
        ReverbFX *reverb = player->midiSynthesizer()->reverbFXs()[0];
        settings->setValue("SynthFXReverbOn", reverb->isOn());
        settings->setValue("SynthFXReverbInGain", (int)reverb->inGain());
        settings->setValue("SynthFXReverbMix", (int)reverb->reverbMix());
        settings->setValue("SynthFXReverbTime", (int)reverb->reverbTime());
        settings->setValue("SynthFXReverbHF", reverb->highFreqRTRatio());


        // Synth chorus
        ChorusFX *chorus = player->midiSynthesizer()->chorusFXs()[0];
        settings->setValue("SynthFXChorusOn", chorus->isOn());

        int cWf  = static_cast<int>(chorus->waveform());
        int cPh  = static_cast<int>(chorus->phase());
        settings->setValue("SynthFXChorusWaveform", cWf);
        settings->setValue("SynthFXChorusPhase", cPh);

        settings->setValue("SynthFXChorusWetDryMix", (int)chorus->wetDryMix());
        settings->setValue("SynthFXChorusDepth", (int)chorus->depth());
        settings->setValue("SynthFXChorusFeedback", (int)chorus->feedback());
        settings->setValue("SynthFXChorusFrequency", (int)chorus->frequency());
        settings->setValue("SynthFXChorusDelay", (int)chorus->delay());
    }

    delete synthMix;

    settings->setValue("LastOpenDir", Utils::LAST_OPEN_DIR);
    settings->setValue("MidiVolume", ui->sliderVolume->value());
    if (this->isFullScreen()) {
        settings->setValue("WindowFullScreen", true);
        settings->setValue("WindowMaximized", false);
    } else if (this->isMaximized()) {
        settings->setValue("WindowFullScreen", false);
        settings->setValue("WindowMaximized", true);
    } else {
        settings->setValue("WindowFullScreen", false);
        settings->setValue("WindowMaximized", false);
        settings->setValue("WindowWidth", this->width());
        settings->setValue("WindowHeight", this->height());
    }

    foreach (Song *s, playlist) {
        delete s;
    }

    delete player;

    delete detailTimer;

    delete lyricsTimer;
    delete positionTimer;
    delete timer2;
    delete timer1;

    delete db;
    delete settings;

    #ifdef _WIN32
    delete taskbarButton;
    #endif

    delete ui;

    delete updateDetail;
    delete lyrWidget;

    BASS_Free();
}

void MainWindow::setBackgroundColor(const QString &colorName)
{
    bgType = 0;
    bgColor = colorName;
    this->setStyleSheet("#MainWindow {background-color: " + colorName + ";}");
}

void MainWindow::setBackgroundImage(const QString &img)
{
    if (QFile::exists(img)) {
        this->setStyleSheet("");
        bgType = 1;
        bgImg = img;
        QPixmap bg(img);
        bg = bg.scaled(this->size(), Qt::IgnoreAspectRatio);
        QPalette palette;
        palette.setBrush(QPalette::Background, bg); //set the pic to the background
        this->setPalette(palette); //show the background pic
    }
}

void MainWindow::play(int index, int position)
{
    stop();
    if (index == -1 && playingSong.id() != "")
    {
        lyrWidget->reset();
        lyrWidget->show();
        if (secondLyr != nullptr)
        {
            secondLyr->reset();
            secondLyr->show();
        }
        if (position > 0)
        {
            lyrWidget->setSeekPositionCursor(position);
            if (secondLyr != nullptr)
                secondLyr->setSeekPositionCursor(position);
        }

        #ifdef _WIN32
        taskbarButton->progress()->resume();
        taskbarButton->progress()->show();
        #endif

        if (position > 0)
            player->setPositionTick(position);

        player->play();
        positionTimer->start();
        lyricsTimer->start();
        return;
    }

    Song *s = playlist[index];
    playingSong = *s;
    playingIndex = index;

    if (remove_playlist) {
        delete s; // delete playlist in "index"
        playlist.removeAt(index);
        ui->playlistWidget->removeRow(index);
        playingIndex = -1;
    }


    // NCN File
    if (playingSong.songType() == "NCN") {

        QString p = db->ncnPath() + playingSong.path();
        if (!player->load(p, true)) {
            QMessageBox::warning(this, "ไม่สามารถเล่นเพลงได้",
                                 "ไม่มีไฟล์ " + p +
                                 "\nหรือไฟล์อาจเสียหายไม่สามารถอ่านได้", QMessageBox::Ok);
            return;
        }

        QString curPath = db->getCurFilePath(p);
        if (curPath == "" || !QFile::exists(curPath)) {
            QMessageBox::warning(this, "ไม่สามารถเล่นเพลงได้",
                                 "ไม่มีไฟล์ Cursor หรัส " + playingSong.id() +
                                 "\nหรือไฟล์อาจเสียหายไม่สามารถอ่านได้", QMessageBox::Ok);
            return;
        }

        QString lyrPath = db->getLyrFilePath(p);
        if (lyrPath == "" || !QFile::exists(lyrPath)) {
            QMessageBox::warning(this, "ไม่สามารถเล่นเพลงได้",
                                 "ไม่มีไฟล์ Lyrics รหัส " + playingSong.id() +
                                 "\nหรือไฟล์อาจเสียหายไม่สามารถอ่านได้", QMessageBox::Ok);
            return;
        }

        lyrWidget->setLyrics(Utils::readLyrics(lyrPath),
            Utils::readCurFile(curPath, player->midiFile()->resorution()));

    } else if (playingSong.songType() == "HNK") {
        // HNK File
        QString p = db->hnkPath() + playingSong.path();
        if (!QFile::exists(p)) {
            QMessageBox::warning(this, "ไม่สามารถเล่นเพลงได้",
                                 "ไม่มีไฟล์ " + p, QMessageBox::Ok);
            return;
        }

        QFile mid("temp.mid");
        if (mid.exists())
            mid.remove();

        mid.open(QFile::ReadWrite);
        mid.write(HNKFile::midData(p));
        mid.close();

        if (!player->load("temp.mid", true)) {
            QMessageBox::warning(this, "ไม่สามารถเล่นเพลงได้",
                                 "ไฟล์อาจเสียหายไม่สามารถอ่านได้", QMessageBox::Ok);
            mid.remove();
            return;
        }

        mid.remove();

        lyrWidget->setLyrics(Utils::readLyrics(HNKFile::lyrData(p)),
            Utils::readCurFile(HNKFile::curData(p), player->midiFile()->resorution()));

    }

    if (secondLyr != nullptr)
        secondLyr->setLyrics(lyrWidget->lyrData(), lyrWidget->curData());

    onPlayerDurationTickChanged(player->durationTick());
    onPlayerDurationMSChanged(player->durationMs());

    #ifdef _WIN32
    taskbarButton->progress()->setValue(0);
    taskbarButton->progress()->setMaximum(player->durationTick());
    taskbarButton->progress()->resume();
    taskbarButton->progress()->show();
    #endif

    // RHM
    ui->rhmWidget->setBeat(MidiPlayer::CalculateBeats(player->midiFile()), player->beatCount());

    ui->frameSearch->hide();
    ui->playlistWidget->hide();
    ui->chMix->hide();
    ui->expandChMix->hide();

    // SongDetail
    ui->songDetail->setDetail(&playingSong);
    ui->songDetail->adjustSize();
    ui->songDetail->show();
    timer2->start(songDetail_timeout);

    player->setBpmSpeed(playingSong.bpmSpeed());
    player->setTranspose(playingSong.transpose());
    player->play();

    lyrWidget->show();

    if (secondLyr != nullptr)
        secondLyr->show();
    positionTimer->start();
    lyricsTimer->start();
}

void MainWindow::pause()
{
    positionTimer->stop();
    lyricsTimer->stop();
    lyrWidget->stopAnimation();
    if (secondLyr != nullptr)
        secondLyr->stopAnimation();
    player->stop();

    #ifdef _WIN32
    taskbarButton->progress()->pause();
    #endif
}

void MainWindow::resume()
{
    player->play();
    positionTimer->start();
    lyricsTimer->start();

    #ifdef _WIN32
    taskbarButton->progress()->resume();
    #endif
}

void MainWindow::stop()
{
    positionTimer->stop();
    lyricsTimer->stop();
    player->stop(true);

    ui->sliderPosition->setValue(0);
    lyrWidget->hide();
    lyrWidget->reset();
    if (secondLyr != nullptr) {
        secondLyr->hide();
        secondLyr->reset();
    }
    ui->sliderPosition->setValue(0);
    onPlayerPositionMSChanged(0);

    ui->rhmWidget->reset();

    #ifdef _WIN32
    taskbarButton->progress()->hide();
    taskbarButton->progress()->setValue(0);
    #endif
}

void MainWindow::playNext()
{
    if (playlist.count() > 0 && playingIndex < playlist.count()-1) {
        play(playingIndex+1);
    } else {
        stop();
    }
}

void MainWindow::playPrevious()
{
    if (playingIndex > 0 && playlist.count() > 0) {
        play(playingIndex - 1);
    }
}

void MainWindow::showEvent(QShowEvent *event)
{
    #ifdef _WIN32
    taskbarButton->setWindow(this->windowHandle());
    #endif
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (ui->chMix->isLock())
        return;

    if (event->pos().y() < 70 && event->pos().x() < 700) {
        showChMix();
    }
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    if (bgType == 1) {
        setBackgroundImage(bgImg);
        QMainWindow::resizeEvent(event);
    }
    lyrWidget->resize(ui->centralWidget->size());
    updateDetail->move(width() - 260, 70);
    emit resized(event->size());
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QMessageBox::StandardButton resBtn = QMessageBox::question(
                                            this, "ออกจากโปรแกรม",
                                            "ท่านต้องการออกจากโปรแกรม?",
                                            QMessageBox::Yes|QMessageBox::No);
    if (resBtn != QMessageBox::Yes) {
        event->ignore();
    } else {
        if (secondLyr != nullptr) {
            secondMonitor->close();
            delete secondMonitor;
            secondLyr = nullptr;
        }
        event->accept();
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->modifiers() == Qt::ControlModifier) {
        switch (event->key()) {
            case Qt::Key_Up:
                if (ui->frameSearch->isVisible())
                {
                    preSetTranspose(db->currentSong()->transpose() + 1);
                }
                else if (ui->playlistWidget->isVisible())
                {
                    int i = ui->playlistWidget->currentRow();
                    if (i < 0)
                        break;
                    Song *s = playlist[i];
                    s->setTranspose(s->transpose() + 1);
                    ui->playlistWidget->updateDetail(i, s);
                    timer2->start(playlist_timeout);
                }
                else
                {
                    ui->sliderVolume->setValue(ui->sliderVolume->value() + 5);
                }
                break;
            case Qt::Key_Down:
                if (ui->frameSearch->isVisible())
                {
                    preSetTranspose(db->currentSong()->transpose() - 1);
                }
                else if (ui->playlistWidget->isVisible())
                {
                    int i = ui->playlistWidget->currentRow();
                    if (i < 0)
                        break;
                    Song *s = playlist[i];
                    s->setTranspose(s->transpose() - 1);
                    ui->playlistWidget->updateDetail(i, s);
                    timer2->start(playlist_timeout);
                }
                else
                {
                    ui->sliderVolume->setValue(ui->sliderVolume->value() - 5);
                }
                break;
            case Qt::Key_Right:
                if (ui->frameSearch->isVisible())
                {
                    preSetBpmSpeed(db->currentSong()->bpmSpeed() + 1);
                }
                else if (ui->playlistWidget->isVisible())
                {
                    int i = ui->playlistWidget->currentRow();
                    if (i < 0)
                        break;
                    Song *s = playlist[i];
                    s->setBpmSpeed(s->bpmSpeed() + 1);
                    ui->playlistWidget->updateDetail(i, s);
                    timer2->start(playlist_timeout);
                }
                break;
            case Qt::Key_Left:
                if (ui->frameSearch->isVisible())
                {
                    preSetBpmSpeed(db->currentSong()->bpmSpeed() - 1);
                }
                else if (ui->playlistWidget->isVisible())
                {
                    int i = ui->playlistWidget->currentRow();
                    if (i < 0)
                        break;
                    Song *s = playlist[i];
                    s->setBpmSpeed(s->bpmSpeed() - 1);
                    ui->playlistWidget->updateDetail(i, s);
                    timer2->start(playlist_timeout);
                }
                break;
            case Qt::Key_X:
                if (ui->frameSearch->isVisible()) {
                    setFrameSearch( db->search("") );
                    ui->lbSearch->setText("_");
                    ui->frameSearch->show();
                    timer2->start(search_timeout);
                }
                break;
            case Qt::Key_Equal:
                if (ui->playlistWidget->isVisible()) {
                    int i = ui->playlistWidget->currentRow();
                    if (i >= ui->playlistWidget->rowCount() -1)
                        break;

                    playlist.swap(i, i+1);
                    ui->playlistWidget->swapRow(i, i+1);
                    ui->playlistWidget->setCurrentRow(i+1);
                    timer2->start(playlist_timeout);
                }
                break;
            case Qt::Key_Minus:
                if (ui->playlistWidget->isVisible()) {
                    int i = ui->playlistWidget->currentRow();
                    if (i == 0)
                        break;

                    playlist.swap(i, i-1);
                    ui->playlistWidget->swapRow(i, i-1);
                    ui->playlistWidget->setCurrentRow(i-1);
                    timer2->start(playlist_timeout);
                }
                break;
            case Qt::Key_S:
                if (ui->playlistWidget->isVisible())
                {
                    timer2->stop();
                    savePlaylist();
                    timer2->start(playlist_timeout);
                }
                break;
            case Qt::Key_O:
                if (ui->playlistWidget->isVisible())
                {
                    timer2->stop();
                    loadPlaylist();
                    timer2->start(playlist_timeout);
                }
                else
                    this->sendDrumPads(event, true);
                break;
            default:
                this->sendDrumPads(event, true);
                break;
        }
        return;
    }


    switch (event->key()) {
    case Qt::Key_Escape: {
        hideUIFrame();
        break;
    }
    case Qt::Key_F5: {
        if (player->isPlayerPaused())
            resume();
        else
            play(-1);
        break;
    }
    case Qt::Key_F6: {
        pause();
        break;
    }
    case Qt::Key_F7: {
        stop();
        break;
    }
    case Qt::Key_F8: {
        playPrevious();
        break;
    }
    case Qt::Key_F9: {
        playNext();
        break;
    }
    case Qt::Key_Insert: {
        if (ui->frameSearch->isVisible()) {
            preSetTranspose(db->currentSong()->transpose() + 1);
            break;
        }
        playingSong.setTranspose(player->transpose() + 1);
        player->setTranspose(player->transpose() + 1);
        int trp = player->transpose();
        QString t;
        if (trp > 0) t = "+" + QString::number(trp);
        else t = QString::number(trp);
        ui->songDetail->setDetail(&playingSong);
        ui->songDetail->adjustSize();
        ui->detail->setDetail("คีย์เพลง ", t);
        ui->detail->show();
        detailTimer->start(3000);
        if (this->width() < 1160 && ui->chMix->isVisible()) {
            ui->lcdTime->hide();
        }
        break;
    }
    case Qt::Key_Delete: {
        if (ui->frameSearch->isVisible()) {
            preSetTranspose(db->currentSong()->transpose() - 1);
            break;
        }
        if (ui->playlistWidget->isVisible()) {
            int i = ui->playlistWidget->currentRow();
            if (i<0)
                break;
            delete playlist.at(i);
            playlist.removeAt(i);
            ui->playlistWidget->removeRow(i);
            showPlaylist();
            timer2->start(playlist_timeout);
            break;
        }
        playingSong.setTranspose(player->transpose() - 1);
        player->setTranspose(player->transpose() - 1);
        int trp = player->transpose();
        QString t;
        if (trp > 0) t = "+" + QString::number(trp);
        else t = QString::number(trp);
        ui->songDetail->setDetail(&playingSong);
        ui->songDetail->adjustSize();
        ui->detail->setDetail("คีย์เพลง ", t);
        ui->detail->show();
        detailTimer->start(3000);
        if (this->width() < 1160 && ui->chMix->isVisible()) {
            ui->lcdTime->hide();
        }
        break;
    }
    case Qt::Key_PageUp:
        if (ui->frameSearch->isVisible()) {
            preSetBpmSpeed(db->currentSong()->bpmSpeed() + 1);
        } else if (ui->playlistWidget->isVisible()) {
            int i = ui->playlistWidget->currentRow();
            if (i < 0)
                break;
            Song *s = playlist[i];
            s->setBpmSpeed(s->bpmSpeed() + 1);
            ui->playlistWidget->updateDetail(i, s);
            timer2->start(playlist_timeout);
        } else {
            addBpmSpeed(1);
        }
        break;
    case Qt::Key_PageDown:
        if (ui->frameSearch->isVisible()) {
            preSetBpmSpeed(db->currentSong()->bpmSpeed() - 1);
        } else if (ui->playlistWidget->isVisible()) {
            int i = ui->playlistWidget->currentRow();
            if (i < 0)
                break;
            Song *s = playlist[i];
            s->setBpmSpeed(s->bpmSpeed() - 1);
            ui->playlistWidget->updateDetail(i, s);
            timer2->start(playlist_timeout);
        } else {
            addBpmSpeed(-1);
        }
        break;
    case Qt::Key_Home:
        if (playingSong.id() != "") {
            play(-1);
        }
        break;
    case Qt::Key_End:
        if (auto_playnext) {
            playNext();
        } else {
            stop();
        }
        break;
    case Qt::Key_Up:
        if (ui->playlistWidget->isVisible()) {
            if (ui->playlistWidget->currentRow() > 0 )
                ui->playlistWidget->setCurrentRow( ui->playlistWidget->currentRow() - 1 );
            timer2->start(playlist_timeout);
        } else {
            ui->frameSearch->hide();
            if (ui->playlistWidget->currentRow() == -1)
                ui->playlistWidget->setCurrentRow(0);
            showPlaylist();
            ui->chMix->hide();
            ui->expandChMix->hide();
            timer2->start(playlist_timeout);
        }
        break;
    case Qt::Key_Down:
        if (ui->playlistWidget->isVisible()) {
            if (ui->playlistWidget->currentRow() < ui->playlistWidget->rowCount() - 1 )
                ui->playlistWidget->setCurrentRow( ui->playlistWidget->currentRow() + 1 );
            timer2->start(playlist_timeout);
        } else {
            ui->frameSearch->hide();
            if (ui->playlistWidget->currentRow() == -1)
                ui->playlistWidget->setCurrentRow(0);
            showPlaylist();
            ui->chMix->hide();
            ui->expandChMix->hide();
            timer2->start(playlist_timeout);
        }
        break;
    case Qt::Key_Right:
        if (ui->frameSearch->isVisible()) {
            if (searchBoxChangeBpm) {
                searchBoxChangeBpm = false;
                ui->lbSearch->setText(db->searchText() + "_");
            }
            setFrameSearch( db->searchNext() );
            timer2->start(search_timeout);
        } else {
            searchBoxChangeBpm = false;
            db->currentSong()->setBpmSpeed(0);
            db->currentSong()->setTranspose(0);
            ui->playlistWidget->hide();
            if (ui->lbId->text() == "")
                setFrameSearch( db->search("") );
            ui->lbSearch->setText("_");
            showFrameSearch();
            ui->chMix->hide();
            ui->expandChMix->hide();
            timer2->start(search_timeout);
        }
        break;
    case Qt::Key_Left:
        if (ui->frameSearch->isVisible()) {
            if (searchBoxChangeBpm) {
                searchBoxChangeBpm = false;
                ui->lbSearch->setText(db->searchText() + "_");
            }
            setFrameSearch( db->searchPrevious() );
            timer2->start(search_timeout);
        } else {
            searchBoxChangeBpm = false;
            db->currentSong()->setBpmSpeed(0);
            db->currentSong()->setTranspose(0);
            ui->playlistWidget->hide();
            if (ui->lbId->text() == "")
                setFrameSearch( db->search("") );
            ui->lbSearch->setText("_");
            showFrameSearch();
            ui->chMix->hide();
            ui->expandChMix->hide();
            timer2->start(search_timeout);
        }
        break;
    case Qt::Key_Tab:
        if (ui->frameSearch->isVisible()) {
            QString s = ui->lbSearch->text();
            s = s.replace(s.length() - 1, 1, "");
            setFrameSearch( db->nextType(s) );
            timer2->start(search_timeout);
        }
        break;
    case Qt::Key_Backspace:
        if (ui->frameSearch->isVisible()) {
            if (ui->lbSearch->text() == "_") {
                timer2->start(search_timeout);
                break;
            }
            QString s = ui->lbSearch->text();
            s = s.replace(s.length() - 2, 2, "");
            setFrameSearch( db->search(s) );
            ui->lbSearch->setText(s + "_");
            timer2->start(search_timeout);
        }
        break;
    case Qt::Key_Enter:
    case Qt::Key_Return:
        if (ui->frameSearch->isVisible()) {
            Song *s = db->currentSong();
            ui->playlistWidget->addSong(s);

            Song *songToAdd = new Song();
            *songToAdd = *s;
            playlist.append(songToAdd);

            if (auto_playnext && playlist.count() == 1 && player->isPlayerStopped()) {
                play(0);
            } else {
                hideUIFrame();
            }
        }
        if (ui->playlistWidget->isVisible() && ui->playlistWidget->rowCount() > 0) {
            ui->playlistWidget->hide();
            play(ui->playlistWidget->currentRow());
        }
        break;
    case Qt::Key_Space:
        if (ui->frameSearch->isHidden()) {
            showSongDetail();
            timer2->start(songDetail_timeout);
            break;
        }
    case Qt::Key_Plus:
        if (ui->playlistWidget->isVisible()) {
            int i = ui->playlistWidget->currentRow();
            if (i >= ui->playlistWidget->rowCount() -1)
                break;

            playlist.swap(i, i+1);
            ui->playlistWidget->swapRow(i, i+1);
            ui->playlistWidget->setCurrentRow(i+1);
            timer2->start(playlist_timeout);
            break;
        }
    case Qt::Key_Minus:
        if (ui->playlistWidget->isVisible()) {
            int i = ui->playlistWidget->currentRow();
            if (i == 0)
                break;

            playlist.swap(i, i-1);
            ui->playlistWidget->swapRow(i, i-1);
            ui->playlistWidget->setCurrentRow(i-1);
            timer2->start(playlist_timeout);
            break;
        }
    default:
        if (searchBoxChangeBpm) {
            ui->lbSearch->setText("_");
            searchBoxChangeBpm = false;
        }
        if (ui->frameSearch->isVisible()) {
            QString s = ui->lbSearch->text();
            s = s.replace(s.length() - 1, 1, "");
            ui->lbSearch->setText(s + event->text() + "_");
            setFrameSearch( db->search(s + event->text()) );
            timer2->start(search_timeout);
        } else {
            db->setSearchType(SearchType::ByAll);
            ui->lbSearch->setText(event->text() + "_");
            setFrameSearch( db->search(event->text() ));
            showFrameSearch();
            ui->chMix->hide();
            ui->expandChMix->hide();
            timer2->start(search_timeout);
        }
        break;
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    if (event->modifiers() == Qt::ControlModifier) {
        this->sendDrumPads(event, false);
    }
}

void MainWindow::showFrameSearch()
{
    ui->chMix->hide();
    ui->expandChMix->hide();
    ui->songDetail->hide();
    ui->playlistWidget->hide();

    ui->frameSearch->show();
}

void MainWindow::showPlaylist()
{
    ui->chMix->hide();
    ui->expandChMix->hide();
    ui->songDetail->hide();
    ui->frameSearch->hide();

    ui->playlistWidget->show();
}

void MainWindow::showSongDetail()
{
    ui->chMix->hide();
    ui->expandChMix->hide();
    ui->frameSearch->hide();
    ui->playlistWidget->hide();

    ui->songDetail->show();
}


void MainWindow::showChMix()
{
    if (ui->playlistWidget->isVisible()
            || ui->frameSearch->isVisible()
            || ui->songDetail->isVisible())
    {
        return;
    }

    ui->chMix->show();
    ui->expandChMix->show();

    if (ui->detail->isVisible() && this->width() < 1160) {
        ui->lcdTime->hide();
    }
}

void MainWindow::onChMixMouseLeaved()
{
    if (ui->chMix->isLock())
        return;

    ui->chMix->hide();
    ui->expandChMix->hide();
}

void MainWindow::onChMixLockChanged(bool lock)
{
    if (lock) {
        settings->setValue("ChMixLock", true);
    } else {
        settings->setValue("ChMixLock", false);
    }
}

void MainWindow::hideUIFrame()
{
    ui->frameSearch->hide();
    ui->playlistWidget->hide();
    ui->songDetail->hide();

    if (ui->chMix->isLock()) {
        ui->chMix->show();
        ui->expandChMix->show();
    }
}

void MainWindow::showCurrentTime()
{
    QTime time = QTime::currentTime();
    QString currentTime = locale.toString(time, "hh:mm:ss");
    ui->lcdTime->display(currentTime);
}

void MainWindow::setFrameSearch(Song *s)
{
    ui->lbId->setText(s->id());
    ui->lbName->setText(" " + s->name());
    ui->lbBtw->setText("-");
    ui->lbArtist->setText(s->artist());
    if (s->key() != "")
        ui->lbTempoKey->setText(" (" + QString::number(s->tempo()) + "-" + s->key() + ")");
    else
        ui->lbTempoKey->setText(" (" + QString::number(s->tempo()) + ")");
    ui->lbType->setText("[" + s->songType() + "]");
    ui->lbLyrics->setText(s->lyrics().replace("\r\n", " "));
}

void MainWindow::showContextMenu(const QPoint &pos)
{
    QMenu menu(tr("Context menu"), this);
    menu.setMinimumWidth(230);

    QAction actionSettings("ตั้งค่า...", this);
    QAction actionMappChanel("แยกช่องสัญญาณ...", this);
    QAction actionShowSynthMixDlg("Handy Synth Mixer", this);
    QAction actionShowEqDlg("อีควอไลเซอร์", this);
    QAction actionShowReverbDlg("เอฟเฟ็กต์เสียงก้อง", this);
    QAction actionShowChorusDlg("เอฟเฟ็กต์เสียงประสาน", this);
    QAction actionMapSF("ตารางเลือกใช้ซาวด์ฟ้อนท์", this);
    QAction actionSecondMonitor("ระบบ 2 หน้าจอ", this);
    QAction actionFullScreen("เต็มหน้าจอ (ย่อ/ขยาย)", this);
    QAction actionAbout("เกี่ยวกับ...", this);
    QAction actionAboutQt("เกี่ยวกับ Qt...", this);
    QAction actionExit("ออกจากโปรแกรม", this);

    if (secondLyr != nullptr) {
        actionSecondMonitor.setCheckable(true);
        actionSecondMonitor.setChecked(true);
    }

    connect(&actionSettings, SIGNAL(triggered()), this, SLOT(showSettingsDialog()));
    connect(&actionMappChanel, SIGNAL(triggered()), this, SLOT(showMapMidiChannelDialog()));
    connect(&actionShowSynthMixDlg, SIGNAL(triggered()), synthMix, SLOT(show()));
    connect(&actionShowEqDlg, SIGNAL(triggered()), this, SLOT(showEqDialog()));
    connect(&actionShowReverbDlg, SIGNAL(triggered()), this, SLOT(showReverbDialog()));
    connect(&actionShowChorusDlg, SIGNAL(triggered()), this, SLOT(showChorusDialog()));
    connect(&actionMapSF, SIGNAL(triggered()), this, SLOT(showMapSFDialog()));
    connect(&actionSecondMonitor, SIGNAL(triggered()), this, SLOT(showSecondMonitor()));
    connect(&actionFullScreen, SIGNAL(triggered()), this, SLOT(showFullScreenOrNormal()));
    connect(&actionAbout, SIGNAL(triggered()), this, SLOT(showAboutDialog()));
    connect(&actionAboutQt, SIGNAL(triggered()), this, SLOT(showAboutQtDialog()));
    connect(&actionExit, SIGNAL(triggered()), this, SLOT(close()));

    menu.addAction(&actionSettings);
    menu.addSeparator();
    menu.addAction(&actionMappChanel);
    menu.addSeparator();
    menu.addAction(&actionShowSynthMixDlg);
    menu.addAction(&actionShowEqDlg);
    menu.addAction(&actionShowReverbDlg);
    menu.addAction(&actionShowChorusDlg);
    menu.addAction(&actionMapSF);
    menu.addSeparator();
    menu.addAction(&actionSecondMonitor);
    menu.addAction(&actionFullScreen);
    menu.addSeparator();
    menu.addAction(&actionAbout);
    menu.addAction(&actionAboutQt);
    menu.addAction(&actionExit);

    menu.exec(mapToGlobal(pos));
}

void MainWindow::showSettingsDialog()
{
    SettingsDialog d(this, this);
    d.setModal(true);
    d.adjustSize();
    d.setMinimumSize(d.size());
    d.exec();
}

void MainWindow::showEqDialog()
{
    if (Equalizer31BandDialog::isOpenned())
        return;

    Equalizer31BandDialog *dlg = new Equalizer31BandDialog(
                this, player->midiSynthesizer()->equalizer31BandFXs());
    dlg->adjustSize();
    dlg->setFixedSize(dlg->size());
    dlg->setAttribute(Qt::WA_DeleteOnClose);
    dlg->show();
}

void MainWindow::showReverbDialog()
{
    if (ReverbDialog::isOpenned())
        return;

    ReverbDialog *dlg = new ReverbDialog(this, player->midiSynthesizer()->reverbFXs());
    dlg->adjustSize();
    dlg->setFixedSize(dlg->size());
    dlg->setAttribute(Qt::WA_DeleteOnClose);
    dlg->show();
}

void MainWindow::showChorusDialog()
{
    if (ChorusDialog::isOpenned())
        return;

    ChorusDialog *dlg = new ChorusDialog(this, player->midiSynthesizer()->chorusFXs());
    dlg->adjustSize();
    dlg->setFixedSize(dlg->size());
    dlg->setAttribute(Qt::WA_DeleteOnClose);
    dlg->show();
}

void MainWindow::showMapMidiChannelDialog()
{
    MapChannelDialog dlg(this, player);
    dlg.setModal(true);
    dlg.adjustSize();
    dlg.setMinimumSize(dlg.size());
    dlg.exec();
}

void MainWindow::minimizeWindow()
{
    setWindowState(Qt::WindowMinimized);
}

void MainWindow::showMapSFDialog()
{
    MapSoundfontDialog msfDlg(this, player->midiSynthesizer());
    msfDlg.setModal(true);
    msfDlg.setMinimumSize(msfDlg.size());
    msfDlg.exec();
}

void MainWindow::showSecondMonitor()
{
    if (secondLyr == nullptr) {
        this->stop();
        secondMonitor = new SecondMonitorDialog(this, lyrWidget);
        secondMonitor->setWindowTitle("หน้าจอที่สอง - Handy Karaoke");
        #ifdef _WIN32
        secondMonitor->setWindowFlags(Qt::Dialog | Qt::WindowTitleHint);
        #else
        secondMonitor->setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint);
        #endif

        if (bgType == 0)
            secondMonitor->setBackgroundColor(bgColor);
        else
            secondMonitor->setBackgroundImage(bgImg);

        secondLyr = secondMonitor->lyrWidget();
        secondMonitor->show();

        if (qApp->screens().count() > 1) {
            secondMonitor->windowHandle()->setScreen(qApp->screens()[1]);
            secondMonitor->showFullScreen();
        }
    } else {
        secondLyr = nullptr;
        secondMonitor->close();
        delete secondMonitor;
        secondMonitor = nullptr;
    }
}

void MainWindow::showFullScreenOrNormal()
{
    if (this->isFullScreen()) {
        this->showNormal();
    } else {
        this->showFullScreen();
    }
}

void MainWindow::showAboutDialog()
{
    AboutDialog d(this);
    d.setModal(true);
    d.adjustSize();
    d.setFixedSize(d.size());
    d.exec();
}

void MainWindow::showAboutQtDialog()
{
    QMessageBox::aboutQt(this);
}

void MainWindow::onPositiomTimerTimeOut()
{
    int tick = player->positionTick();
    ui->sliderPosition->setValue(tick);
    #ifdef _WIN32
    taskbarButton->progress()->setValue(tick);
    #endif
    onPlayerPositionMSChanged(player->positionMs());
    ui->rhmWidget->setCurrentBeat( player->currentBeat() );
}

void MainWindow::onLyricsTimerTimeOut()
{
    lyrWidget->setPositionCursor(player->positionTick() + 25);
    if (secondLyr != nullptr)
        secondLyr->setPositionCursor(player->positionTick() + 25);
}

void MainWindow::onPlayerDurationMSChanged(qint64 d)
{
    QDateTime t = QDateTime::fromMSecsSinceEpoch(d);
    ui->lbDuration->setText( locale.toString(t, "mm:ss") );
}

void MainWindow::onPlayerPositionMSChanged(qint64 p)
{
    QDateTime t = QDateTime::fromMSecsSinceEpoch(p);
    ui->lbPosition->setText( locale.toString(t, "mm:ss") );
}

void MainWindow::onPlayerDurationTickChanged(int d)
{
    ui->sliderPosition->setMaximum(d);
}

void MainWindow::onSliderPositionPressed()
{
    playAfterSeek = player->isPlayerPlaying();
    pause();
}

void MainWindow::onSliderPositionReleased()
{
    if (player->isPlayerStopped()) {
        ui->sliderPosition->setValue(0);
        return;
    }

    #ifdef _WIN32
    taskbarButton->progress()->setValue(ui->sliderPosition->value());
    #endif

    player->setPositionTick(ui->sliderPosition->value());
    lyrWidget->setSeekPositionCursor(ui->sliderPosition->value());
    if (secondLyr != nullptr)
        secondLyr->setSeekPositionCursor(ui->sliderPosition->value());
    onPlayerPositionMSChanged(player->positionMs());

    // Seek beat
    ui->rhmWidget->setCurrentBeat(player->currentBeat());

    if (playAfterSeek) resume();
}

void MainWindow::on_btnVolumeMute_clicked()
{
    if (ui->sliderVolume->isEnabled()) {
        ui->sliderVolume->setEnabled(false);
        ui->btnVolumeMute->setIconFiles(":/Icons/volume-mute-white", ":/Icons/volume-mute-blue");
        player->setVolume(0);
    } else {
        ui->sliderVolume->setEnabled(true);
        ui->btnVolumeMute->setIconFiles(":/Icons/volume-white", ":/Icons/volume-blue");
        player->setVolume(ui->sliderVolume->value());
    }
}

void MainWindow::on_btnPlay_clicked()
{
    if (player->isPlayerPaused()) {
        resume();
    } else {
        if (playingSong.id() != "")
            play(-1);
    }
}


void MainWindow::onSliderVolumeValueChanged(int value)
{
    player->setVolume(value);

    ui->detail->setDetail("ระดับเสียง", QString::number(value));
    ui->detail->show();
    detailTimer->start(3000);
    if (this->width() < 1160 && ui->chMix->isVisible()) {
        ui->lcdTime->hide();
    }
}

void MainWindow::onPlayerThreadFinished()
{
    if (player->isPlayerFinished()) {
        playNext();
    }
}

void MainWindow::onDbUpdateChanged(int v)
{
    int p = (100 * v / db->updateCount());
    if (p < 0) p = 0;
    updateDetail->setValue(QString::number(p) + "%");
}

void MainWindow::onDetailTimerTimeout()
{
     ui->detail->hide();
     ui->lcdTime->show();
}

void MainWindow::addBpmSpeed(int speed)
{
    int bpm = player->currentBpm() + speed;
    int bpmSp = player->bpmSpeed() + speed;

    player->setBpmSpeed(bpmSp);

    if (playingSong.id() != "")
        playingSong.setBpmSpeed(bpmSp);
    ui->songDetail->setDetail(&playingSong);
    ui->songDetail->adjustSize();

    QString value = QString::number(bpm);
    if (bpmSp != 0) {
        value += bpmSp > 0 ? " (+" : " (";
        value += QString::number(bpmSp) + ")";
    }

    ui->detail->setDetail("ความเร็ว", value);
    ui->detail->show();
    detailTimer->start(3000);
    if (this->width() < 1160 && ui->chMix->isVisible()) {
        ui->lcdTime->hide();
    }
}

void MainWindow::preSetBpmSpeed(int speed)
{
    Song *song = db->currentSong();
    song->setBpmSpeed(speed);
    searchBoxChangeBpm = true;

    QString s;
    if (speed > 0)
        s = "+" + QString::number(speed);
    else
        s = QString::number(speed);

    ui->lbSearch->setText("ความเร็ว : " + QString::number(song->tempo() + speed) + " (" + s + ")");
    timer2->start(search_timeout);
}

void MainWindow::preSetTranspose(int transpose)
{
    Song *song = db->currentSong();
    song->setTranspose(transpose);
    searchBoxChangeBpm = true;

    QString s;
    if (transpose > 0)
        s = "+" + QString::number(transpose);
    else
        s = QString::number(transpose);


    ui->lbSearch->setText("คีย์เพลง : " + s );
    timer2->start(search_timeout);
}

void MainWindow::sendDrumPads(QKeyEvent *key, bool noteOn)
{
    MidiEvent e;
    e.setChannel(9);

    if (noteOn) {
        e.setData2(100);
        e.setEventType(MidiEventType::NoteOn);
    } else {
        e.setData2(0);
        e.setEventType(MidiEventType::NoteOff);
    }

    switch (key->key()) {
        case DrumPadsKey::bassDrum:
            e.setData1(36);
            player->sendEvent(&e);
            break;
        case DrumPadsKey::snare:
            e.setData1(38);
            player->sendEvent(&e);
            break;
        case DrumPadsKey::hihatClose:
            e.setData1(42);
            player->sendEvent(&e);
            break;
        case DrumPadsKey::hihatOpen:
            e.setData1(46);
            player->sendEvent(&e);
            break;
        case DrumPadsKey::cowbell:
            e.setData1(56);
            player->sendEvent(&e);
            break;
        case DrumPadsKey::tambourine:
            e.setData1(54);
            player->sendEvent(&e);
            break;
        case DrumPadsKey::tom1:
            e.setData1(50);
            player->sendEvent(&e);
            break;
        case DrumPadsKey::tom2:
            e.setData1(48);
            player->sendEvent(&e);
            break;
        case DrumPadsKey::crashCymbal:
            e.setData1(49);
            player->sendEvent(&e);
            break;
        default:
            break;
    }
}

void MainWindow::savePlaylist()
{
    if (playlist.count() == 0)
        return;

    QString fileName = QFileDialog::getSaveFileName(this, tr("บันทึกรายการเล่น"),
                                                    Utils::LAST_OPEN_DIR,
                                                    HANDY_PLAYLIST_FILTER_TEXT);

    if (fileName == "")
        return;

    Utils::LAST_OPEN_DIR = QFileInfo(fileName).dir().absolutePath();

    if (!Utils::savePlaylist(fileName, playlist))
    {
        QMessageBox::critical(this, tr("บันทึกรายการเล่นล้มเหลว"),
                              tr("ไม่สามารถบันทึกรายการเล่นได้\nโปรดลองอีกครั้ง"));
    }
}

void MainWindow::loadPlaylist()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("เปิดรายการเล่น"),
                                                    Utils::LAST_OPEN_DIR,
                                                    HANDY_PLAYLIST_FILTER_TEXT);

    if (fileName == "")
        return;

    Utils::LAST_OPEN_DIR = QFileInfo(fileName).dir().absolutePath();

    QList<Song*> songs;
    if (Utils::loadPlaylist(fileName, songs))
    {
        for (Song *s : playlist)
            delete s;
        playlist.clear();
        playlist = songs;
        ui->playlistWidget->setPlaylist(playlist);
        playingIndex = -1;
    }
    else
    {
        QMessageBox::critical(this, tr("เปิดรายการเล่นล้มเหลว"),
                              tr("ไม่สามารถเปิดรายการเล่นได้\nโปรดลองอีกครั้ง"));
    }
}
