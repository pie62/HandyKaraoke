#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "Utils.h"
#include "SettingsDialog.h"
#include "Dialogs/AboutDialog.h"
#include "Dialogs/MapSoundfontDialog.h"
#include "Midi/MidiFile.h"
#include "Midi/HNKFile.h"

#include <QTime>
#include <QMenu>
#include <QCloseEvent>
#include <QMessageBox>
#include <QDebug>
#include <QDir>
#include <QDirIterator>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    BASS_Init(-1, 44100, 0, 0, NULL);

    lyrWidget = new LyricsWidget(this);
    updateDetail = new Detail(this);

    ui->setupUi(this);

    settings = new QSettings();
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

    songDetailTimer = new QTimer();
    songDetailTimer->setSingleShot(true);

    player = new MidiPlayer();

    locale = QLocale(QLocale::English, QLocale::UnitedStates);

    { // Channel Mixer

        ui->chMix->setPlayer(player);

        bool s = settings->value("ChMixShow", false).toBool();

        if (s) {
            ui->chMix->show();
            ui->expandChMix->show();
        }
        else {
            ui->chMix->hide();
            ui->expandChMix->hide();
        }

        connect(ui->chMix, SIGNAL(buttonCloseClicked()), this, SLOT(showHideChMix()));
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
            if (QFile::exists(img)) {
                setBackgroundImage(img);
            } else {
                QString color = settings->value("BackgroundColor", "#525252").toString();
                setBackgroundColor(color);
            }
        }

        int w     = settings->value("WindowWidth", this->minimumWidth()).toInt();
        int h     = settings->value("WindowHeight", this->minimumHeight()).toInt();
        bool max  = settings->value("WindowMaximized", false).toBool();
        bool full = settings->value("WindowFullScreen", false).toBool();

        this->resize(w, h);
        if (max) {
            this->showMaximized();
        } else {
            if (full) {
                this->showFullScreen();
            }
        }
    }


    { // Player
        int oPort   = settings->value("MidiOut", 0).toInt();
        int vl      = settings->value("MidiVolume", 50).toInt();
        bool lDrum  = settings->value("MidiLockDrum", false).toBool();
        bool lSnare = settings->value("MidiLockSnare", false).toBool();
        bool lBass  = settings->value("MidiLockBass", false).toBool();

        player->setMidiOut(oPort);
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

        connect(player, SIGNAL(finished()), this, SLOT(onPlayerThreadFinished()));
        connect(player, SIGNAL(bpmChanged(int)), ui->rhmWidget, SLOT(setBpm(int)));
    }


    { // Synth
        MidiSynthesizer *synth = player->midiSynthesizer();

        // Audio out
        int aout = settings->value("AudioOut", 1).toInt();
        synth->setOutputDevice(aout);

        // floating and fx
        bool useFloat = settings->value("SynthFloatPoint", true).toBool();
        bool useFX = settings->value("SynthUseFXRC", false).toBool();
        synth->setUsetFloattingPoint(useFloat);
        synth->setUseFXRC(useFX);

        // Synth soundfont
        //QList<QString> sfs;
        QStringList sfList = settings->value("SynthSoundfonts", QStringList()).toStringList();

        // Synth soundfont volume
        QList<int> sfvl;
        int idx=0;
        settings->beginReadArray("SynthSoundfontsVolume");
        for (const QString &s : sfList) {
            settings->setArrayIndex(idx);
            sfvl.append(settings->value("SoundfontVolume", 100).toInt());
            idx++;
        }
        settings->endArray();

        synth->setSoundFonts(sfList);

        for (int i=0; i<sfvl.size(); i++) {
            synth->setSoundfontVolume(i, sfvl.at(i) / 100.0f);
        }
        // -----------

        // Synth Map soundfont
        QList<int> sfMap = synth->getMapSoundfontIndex();
        settings->beginReadArray("SynthSoundfontsMap");
        for (int i=0; i<128; i++) {
            settings->setArrayIndex(i);
            sfMap[i] = settings->value("mapTo", 0).toInt();
        }
        settings->endArray();


        QList<int> sfDrumMap = synth->getDrumMapSfIndex();
        settings->beginReadArray("SynthSoundfontsDrumMap");
        for (int i=0; i<16; i++) {
            settings->setArrayIndex(i);
            sfDrumMap[i] = settings->value("mapTo", 0).toInt();
        }
        settings->endArray();

        synth->setMapSoundfontIndex(sfMap, sfDrumMap);


        // Synth EQ
        Equalizer31BandFX *eq = synth->equalizer31BandFX();
        std::map<EQFrequency31Range, float> eqgain = eq->gain();

        bool eqon = settings->value("SynthFXEQOn", false).toBool();
        if (eqon)
            eq->on();

        int gi =0;
        settings->beginReadArray("SynthFXEQGain");
        for (const auto& g : eqgain) {
            settings->setArrayIndex(gi);
            float gain = settings->value("gain", 0.0f).toFloat();
            eq->setGain(g.first, gain);
            gi++;
        }
        settings->endArray();


        // Synth reverb
        ReverbFX *reverb = synth->reverbFX();

        bool rvOn   = settings->value("SynthFXReverbOn", false).toBool();
        int rvGain  = settings->value("SynthFXReverbInGain", 0).toInt();
        int rvMix   = settings->value("SynthFXReverbMix", 0).toInt();
        int rvTime  = settings->value("SynthFXReverbTime", 1000).toInt();
        float rvHF  = settings->value("SynthFXReverbHF", 0.001).toFloat();

        if (rvOn)
            reverb->on();

        reverb->setInGain((float)rvGain);
        reverb->setReverbMix((float)rvMix);
        reverb->setReverbTime((float)rvTime);
        reverb->setHighFreqRTRatio(rvHF);


        // Synth chorus
        ChorusFX *chorus = synth->chorusFX();

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
            chorus->on();

        chorus->setWaveform(lWaveform);
        chorus->setPhase(lPhase);
        chorus->setWetDryMix((float)cWet);
        chorus->setDepth((float)cDep);
        chorus->setFeedback((float)cFb);
        chorus->setFrequency((float)cFq);
        chorus->setDelay((float)cDl);


        // Create Synth effect dialog
        eq31Dlg = new Equalizer31BandDialog(this, eq);
        eq31Dlg->setWindowTitle("อีควอไลเซอร์ : Equalizer");
        eq31Dlg->adjustSize();
        eq31Dlg->setFixedSize(eq31Dlg->size());

        reverbDlg = new ReverbDialog(this, reverb);
        reverbDlg->setWindowTitle("เอฟเฟ็กต์เสียงก้อง : Reverb");
        reverbDlg->adjustSize();
        reverbDlg->setFixedSize(reverbDlg->size());

        chorusDlg = new ChorusDialog(this, chorus);
        chorusDlg->setWindowTitle("เอฟเฟ็กต์เสียงประสาน : Chorus");
        chorusDlg->adjustSize();
        chorusDlg->setFixedSize(chorusDlg->size());

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
        setWindowIcon(QIcon(":/Icons/App/icon_sm.png"));

        updateDetail->hide();
        updateDetail->resize(250, 60);
        updateDetail->setText("กำลังปรับปรุงฐานข้อมูล");

        ui->detail->hide();
        ui->frameSearch->hide();
        ui->framePlaylist->hide();
        ui->songDetail->hide();
        ui->sliderVolume->setValue(player->volume());

        { // Playback control
            ui->btnPlay->setIconFiles(":/Icons/play-white", ":/Icons/play-blue");
            ui->btnPause->setIconFiles(":/Icons/pause-white", ":/Icons/pause-blue");
            ui->btnStop->setIconFiles(":/Icons/stop-white", ":/Icons/stop-blue");
            ui->btnPrevious->setIconFiles(":/Icons/previous-white", ":/Icons/previous-blue");
            ui->btnNext->setIconFiles(":/Icons/next-white", ":/Icons/next-blue");

            ui->btnVolumeMute->setIconFiles(":/Icons/volume-white", ":/Icons/volume-blue");
        }

        connect(db, SIGNAL(started()), updateDetail, SLOT(show()));
        connect(db, SIGNAL(finished()), updateDetail, SLOT(hide()));
        connect(db, SIGNAL(updatePositionChanged(int)), this, SLOT(onDbUpdateChanged(int)));

        connect(timer2, SIGNAL(timeout()), ui->frameSearch, SLOT(hide()));
        connect(timer2, SIGNAL(timeout()), ui->framePlaylist, SLOT(hide()));
        connect(timer1, SIGNAL(timeout()), this, SLOT(showCurrentTime()));
        connect(positionTimer, SIGNAL(timeout()), this, SLOT(onPositiomTimerTimeOut()));
        connect(lyricsTimer, SIGNAL(timeout()), this, SLOT(onLyricsTimerTimeOut()));

        connect(detailTimer, SIGNAL(timeout()), this, SLOT(onDetailTimerTimeout()));
        connect(songDetailTimer, SIGNAL(timeout()), ui->songDetail, SLOT(hide()));

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
        Equalizer31BandFX *eq = player->midiSynthesizer()->equalizer31BandFX();
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
        ReverbFX *reverb = player->midiSynthesizer()->reverbFX();
        settings->setValue("SynthFXReverbOn", reverb->isOn());
        settings->setValue("SynthFXReverbInGain", (int)reverb->inGain());
        settings->setValue("SynthFXReverbMix", (int)reverb->reverbMix());
        settings->setValue("SynthFXReverbTime", (int)reverb->reverbTime());
        settings->setValue("SynthFXReverbHF", reverb->highFreqRTRatio());


        // Synth chorus
        ChorusFX *chorus = player->midiSynthesizer()->chorusFX();
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
    // Delete Synth effect dialog
    delete eq31Dlg;
    delete reverbDlg;
    delete chorusDlg;


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

    delete songDetailTimer;
    delete detailTimer;

    delete lyricsTimer;
    delete positionTimer;
    delete timer2;
    delete timer1;

    delete db;
    delete settings;
    delete ui;

    delete updateDetail;
    delete lyrWidget;

    BASS_Free();
}

void MainWindow::setBackgroundColor(QString colorName)
{
    bgType = 0;
    this->setStyleSheet("#MainWindow {background-color: " + colorName + ";}");
}

void MainWindow::setBackgroundImage(QString img)
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

void MainWindow::play(int index)
{
    stop();
    if (index == -1 && playingSong.id() != "") {
        lyrWidget->reset();
        player->start();
        lyrWidget->show();
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
        delete ui->playlist->takeItem(index);
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
            return;
        }

        lyrWidget->setLyrics(Utils::readLyrics(HNKFile::lyrData(p)),
            Utils::readCurFile(HNKFile::curData(p), player->midiFile()->resorution()));

    }

    onPlayerDurationTickChanged(player->durationTick());
    onPlayerDurationMSChanged(player->durationMs());

    // RHM
    ui->rhmWidget->setBeat(player->beatInBar(), player->beatCount());

    // SongDetail
    ui->songDetail->setDetail(&playingSong);
    ui->songDetail->adjustSize();
    ui->songDetail->show();
    songDetailTimer->start(5000);

    player->start();
    lyrWidget->show();
    positionTimer->start();
    lyricsTimer->start();
}

void MainWindow::pause()
{
    positionTimer->stop();
    lyricsTimer->stop();
    lyrWidget->stopAnimation();
    player->stop();
}

void MainWindow::resume()
{
    player->start();
    positionTimer->start();
    lyricsTimer->start();
}

void MainWindow::stop()
{
    positionTimer->stop();
    lyricsTimer->stop();
    player->stop(true);

    ui->sliderPosition->setValue(0);
    lyrWidget->hide();
    lyrWidget->reset();
    ui->sliderPosition->setValue(0);
    onPlayerPositionMSChanged(0);

    ui->rhmWidget->reset();
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

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{

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
        event->accept();
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->modifiers() & Qt::ControlModifier) {
        switch (event->key()) {
        case Qt::Key_Up:
            ui->sliderVolume->setValue(ui->sliderVolume->value() + 5);
            break;
        case Qt::Key_Down:
            ui->sliderVolume->setValue(ui->sliderVolume->value() - 5);
            break;
        case Qt::Key_X:
            if (ui->frameSearch->isVisible()) {
                setFrameSearch( db->search("") );
                ui->lbSearch->setText("_");
                ui->frameSearch->show();
                timer2->start(search_timeout);
            }
            break;
        default:
            break;
        }
        return;
    }

    switch (event->key()) {
    case Qt::Key_Escape: {
        ui->frameSearch->hide();
        ui->framePlaylist->hide();
        ui->songDetail->hide();
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
        player->setTranspose(player->transpose()+1);
        int trp = player->transpose();
        QString t;
        if (trp > 0) t = "+" + QString::number(trp);
        else t = QString::number(trp);
        ui->detail->setDetail("คีย์เพลง ", t);
        ui->detail->show();
        detailTimer->start(3000);
        if (this->width() < 1150 && ui->chMix->isVisible()) {
            ui->lcdTime->hide();
        }
        break;
    }
    case Qt::Key_Delete: {
        if (ui->framePlaylist->isVisible()) {
            int i = ui->playlist->currentRow();
            delete playlist.at(i);
            playlist.removeAt(i);
            delete ui->playlist->takeItem(i);
            ui->framePlaylist->show();
            timer2->start(playlist_timeout);
            break;
        }
        player->setTranspose(player->transpose()-1);
        int trp = player->transpose();
        QString t;
        if (trp > 0) t = "+" + QString::number(trp);
        else t = QString::number(trp);
        ui->detail->setDetail("คีย์เพลง ", t);
        ui->detail->show();
        detailTimer->start(3000);
        if (this->width() < 1150 && ui->chMix->isVisible()) {
            ui->lcdTime->hide();
        }
        break;
    }
    case Qt::Key_PageUp:
        if (ui->playlist->isVisible()) {
            int i = ui->playlist->currentRow();
            if (i == 0)
                break;

            playlist.swap(i, i-1);
            QListWidgetItem *item = ui->playlist->takeItem(i);
            ui->playlist->insertItem(i-1, item);
            ui->playlist->setCurrentRow(i-1);
            ui->playlist->show();
            timer2->start(playlist_timeout);
        }
        else {
            //setBpmSpeed(player->bpmSpeed() + 1);
            addBpmSpeed(1);
        }
        break;
    case Qt::Key_PageDown:
        if (ui->playlist->isVisible()) {
            int i = ui->playlist->currentRow();
            if (i >= ui->playlist->count() -1)
                break;

            playlist.swap(i, i+1);
            QListWidgetItem *item = ui->playlist->takeItem(i);
            ui->playlist->insertItem(i+1, item);
            ui->playlist->setCurrentRow(i+1);
            ui->playlist->show();
            timer2->start(playlist_timeout);
        }
        else {
            //setBpmSpeed(player->bpmSpeed() - 1);
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
        if (ui->framePlaylist->isVisible()) {
            if (ui->playlist->currentRow() > 0 )
                ui->playlist->setCurrentRow( ui->playlist->currentRow() - 1 );
            timer2->start(playlist_timeout);
        } else {
            ui->frameSearch->hide();
            if (ui->playlist->currentRow() == -1)
                ui->playlist->setCurrentRow(0);
            ui->framePlaylist->show();
            timer2->start(playlist_timeout);
        }
        break;
    case Qt::Key_Down:
        if (ui->framePlaylist->isVisible()) {
            if (ui->playlist->currentRow() < ui->playlist->count() - 1 )
                ui->playlist->setCurrentRow( ui->playlist->currentRow() + 1 );
            timer2->start(playlist_timeout);
        } else {
            ui->frameSearch->hide();
            if (playingIndex == -1)
                ui->playlist->setCurrentRow(0);
            else
                ui->playlist->setCurrentRow(playingIndex);
            ui->framePlaylist->show();
            timer2->start(playlist_timeout);
        }
        break;
    case Qt::Key_Right:
        if (ui->frameSearch->isVisible()) {
            setFrameSearch( db->searchNext() );
            timer2->start(search_timeout);
        } else {
            ui->framePlaylist->hide();
            if (ui->lbId->text() == "")
                setFrameSearch( db->search("") );
            ui->lbSearch->setText("_");
            ui->frameSearch->show();
            timer2->start(search_timeout);
        }
        break;
    case Qt::Key_Left:
        if (ui->frameSearch->isVisible()) {
            setFrameSearch( db->searchPrevious() );
            timer2->start(search_timeout);
        } else {
            ui->framePlaylist->hide();
            if (ui->lbId->text() == "")
                setFrameSearch( db->search("") );
            ui->lbSearch->setText("_");
            ui->frameSearch->show();
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
            ui->playlist->addItem(s->detail());
            QSize size;
            size.setHeight(41);
            ui->playlist->item(ui->playlist->count() - 1)->setSizeHint(size);
            ui->frameSearch->hide();

            // Test
            Song *sToAdd = new Song();
            *sToAdd = *s;
            playlist.append(sToAdd);

            if (auto_playnext && playlist.count() == 1 && player->isPlayerStopped()) {
                play(0);
            }
        }
        if (ui->framePlaylist->isVisible()) {
            ui->framePlaylist->hide();
            play(ui->playlist->currentRow());
        }
        break;
    case Qt::Key_Space:
        if (ui->frameSearch->isHidden()) {
            ui->framePlaylist->hide();
            ui->songDetail->show();
            songDetailTimer->start(5000);
            break;
        }
    default:
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
            ui->framePlaylist->hide();
            ui->frameSearch->show();
            timer2->start(search_timeout);
        }
        break;
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

    QAction actionSettings("ตั้งค่า", this);
    QAction actionShowHideChMix("ช่องสัญญาณมิกเซอร์ (แสดง/ซ่อน)", this);
    QAction actionShowSynthMixDlg("Handy Synth Mixer", this);
    QAction actionShowEqDlg("อีควอไลเซอร์", this);
    QAction actionShowReverbDlg("เอฟเฟ็กต์เสียงก้อง", this);
    QAction actionShowChorusDlg("เอฟเฟ็กต์เสียงประสาน", this);
    QAction actionMapSF("การเลือกใช้ซาวด์ฟ้อนท์", this);
    //QAction actionMinimize("ยุบหน้าจอ", this);
    QAction actionFullScreen("เต็มหน้าจอ (ย่อ/ขยาย)", this);
    QAction actionAbout("เกี่ยวกับ", this);
    QAction actionExit("ออกจากโปรแกรม", this);

    connect(&actionSettings, SIGNAL(triggered()), this, SLOT(showSettingsDialog()));
    connect(&actionShowHideChMix, SIGNAL(triggered()), this, SLOT(showHideChMix()));
    connect(&actionShowSynthMixDlg, SIGNAL(triggered()), synthMix, SLOT(show()));
    connect(&actionShowEqDlg, SIGNAL(triggered()), eq31Dlg, SLOT(show()));
    connect(&actionShowReverbDlg, SIGNAL(triggered()), reverbDlg, SLOT(show()));
    connect(&actionShowChorusDlg, SIGNAL(triggered()), chorusDlg, SLOT(show()));
    connect(&actionMapSF, SIGNAL(triggered()), this, SLOT(showMapSFDialog()));
    //connect(&actionMinimize, SIGNAL(triggered()), this, SLOT(minimizeWindow()));
    connect(&actionFullScreen, SIGNAL(triggered()), this, SLOT(showFullScreenOrNormal()));
    connect(&actionAbout, SIGNAL(triggered()), this, SLOT(showAboutDialog()));
    connect(&actionExit, SIGNAL(triggered()), this, SLOT(close()));

    menu.addAction(&actionSettings);
    menu.addSeparator();
    menu.addAction(&actionShowHideChMix);
    menu.addSeparator();
    menu.addAction(&actionShowSynthMixDlg);
    menu.addAction(&actionShowEqDlg);
    menu.addAction(&actionShowReverbDlg);
    menu.addAction(&actionShowChorusDlg);
    menu.addAction(&actionMapSF);
    menu.addSeparator();
    //menu.addAction(&actionMinimize);
    menu.addAction(&actionFullScreen);
    menu.addSeparator();
    menu.addAction(&actionAbout);
    menu.addAction(&actionExit);

    menu.exec(mapToGlobal(pos));
}

void MainWindow::showSettingsDialog()
{
    SettingsDialog d(this, this);
    d.setModal(true);
    d.setMinimumSize(550, 400);
    d.exec();
}

void MainWindow::showHideChMix()
{
    if (ui->chMix->isVisible()) {
        ui->chMix->hide();
        ui->expandChMix->hide();
        settings->setValue("ChMixShow", false);
    }
    else {
        ui->chMix->show();
        ui->expandChMix->show();
        settings->setValue("ChMixShow", true);
    }
}

void MainWindow::minimizeWindow()
{
    setWindowState(Qt::WindowMinimized);
}

void MainWindow::showMapSFDialog()
{
    MapSoundfontDialog msfDlg(this, player);
    msfDlg.setModal(true);
    msfDlg.setMinimumSize(msfDlg.size());
    msfDlg.exec();
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
    d.exec();
}

void MainWindow::onPositiomTimerTimeOut()
{
    ui->sliderPosition->setValue(player->positionTick());
    onPlayerPositionMSChanged(player->positionMs());
    ui->rhmWidget->setCurrentBeat( player->currentBeat() );
}

void MainWindow::onLyricsTimerTimeOut()
{
    lyrWidget->setPositionCursor(player->positionTick());
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

    player->setPositionTick(ui->sliderPosition->value());
    lyrWidget->setSeekPositionCursor(ui->sliderPosition->value());
    onPlayerPositionMSChanged(player->positionMs());

    // Seek beat
    ui->rhmWidget->setSeekBeat(player->currentBeat());

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
