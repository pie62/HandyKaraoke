#include "SettingsDialog.h"
#include "ui_SettingsDialog.h"

#include "Config.h"
#include "Utils.h"
#include "Midi/MidiHelper.h"
#include "Dialogs/MapSoundfontDialog.h"
#include "Dialogs/MapChannelDialog.h"

#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
#include <QFontDialog>
#include <QColorDialog>

SettingsDialog::SettingsDialog(QWidget *parent, MainWindow *m) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    this->mainWin = m;
    settings = new QSettings(CONFIG_APP_FILE_PATH, QSettings::IniFormat);
    db = mainWin->database();


    { // ทั่วไป
        ui->chbRemoveFromList->setChecked( mainWin->removeFromPlaylist() );
        ui->chbAutoPlaynext->setChecked( mainWin->autoPlayNext() );

        ui->spinSearch->setValue( mainWin->searchTimeout() );
        ui->spinPlaylist->setValue( mainWin->playlistTimout() );

        int bg = settings->value("BackgroundType", 0).toInt();
        QString color = settings->value("BackgroundColor", "#525252").toString();
        QString img = settings->value("BackgroundImage", "").toString();
        ui->lbBgColor->setText(color);
        ui->lbBgColor->setStyleSheet("background-color : " + color);
        ui->leBgImgPath->setText(img);
        if (bg == 0 || img == "") {
            ui->radioBgColor->setChecked(true);
            ui->lbBgImgN->setEnabled(false);
            ui->btnBgImg->setEnabled(false);
            ui->leBgImgPath->setEnabled(false);
        } else {
            ui->radioBgImg->setChecked(true);
            ui->lbBgColorN->setEnabled(false);
            ui->lbBgColor->setEnabled(false);
            ui->btnBgColor->setEnabled(false);
        }

        #ifdef _WIN32
        ui->chbAutoCheckUpdate->setChecked(win_sparkle_get_automatic_check_for_updates());
        ui->spinUpdateInterval->setValue(win_sparkle_get_update_check_interval() / 60 / 60);
        if (!ui->chbAutoCheckUpdate->isChecked())
        {
            ui->lbUpdateInterval->setEnabled(false);
            ui->spinUpdateInterval->setEnabled(false);
        }
        connect(ui->spinUpdateInterval, SIGNAL(valueChanged(int)),
                this, SLOT(onSpinUpdateValueChanged(int)));
        #else
        ui->groupBoxUpdate->hide();
        #endif

        connect(ui->spinSearch, SIGNAL(valueChanged(int)), this, SLOT(onSpinSearchValueChanged(int)));
        connect(ui->spinPlaylist, SIGNAL(valueChanged(int)), this, SLOT(onSpinPlaylistValueChanged(int)));

        connect(ui->radioBgColor, SIGNAL(toggled(bool)), this, SLOT(onRadioBgColorToggled(bool)));
        connect(ui->radioBgImg, SIGNAL(toggled(bool)), this, SLOT(onRadioBgImgToggled(bool)));


        // playlist
        //connect(ui->chbRemoveFromList, SIGNAL(toggled(bool)), this, SLOT(on_chbRemoveFromList_toggled(bool)));

        // background
        //connect(ui->radioBgColor, SIGNAL(toggled(bool)), this, SLOT(on_radioBgColor_toggled(bool)));
        //connect(ui->radioBgImg, SIGNAL(toggled(bool)), this, SLOT(on_radioBgImg_toggled(bool)));
    }


    // Lyrics
    {
        QFont font = mainWin->lyricsWidget()->textFont();
        setLabelFontInfo(&font);

        int l1y = mainWin->lyricsWidget()->line1Y();
        int l2y = mainWin->lyricsWidget()->line2Y();
        int anmtTime = mainWin->lyricsWidget()->animationTime();
        bool autosize = mainWin->lyricsWidget()->isAutoFontSize();
        ui->spinLint1Y->setValue(l1y);
        ui->spinLint2Y->setValue(l2y);
        ui->spinAnmtTime->setValue(anmtTime);
        ui->chbLyrAutoSize->setChecked(autosize);

        LinePosition l1 = mainWin->lyricsWidget()->line1Position();
        LinePosition l2 = mainWin->lyricsWidget()->line2Position();
        ui->cbLine1X->setCurrentIndex(static_cast<int>(l1));
        ui->cbLine2X->setCurrentIndex(static_cast<int>(l2));

        QString textColor =  mainWin->lyricsWidget()->textColor().name();
        QString textBorderColor = mainWin->lyricsWidget()->textBorderColor().name();
        QString textBorderOutColor = mainWin->lyricsWidget()->textBorderOutColor().name();
        int tW = mainWin->lyricsWidget()->textBorderWidth();
        int tOW = mainWin->lyricsWidget()->textBorderOutWidth();
        ui->lbTextColor->setText(textColor);
        ui->lbTextColor->setStyleSheet("background-color : " + textColor);
        ui->lbTextBorderColor->setText(textBorderColor);
        ui->lbTextBorderColor->setStyleSheet("background-color : " + textBorderColor);
        ui->lbTextBorderOutColor->setText(textBorderOutColor);
        ui->lbTextBorderOutColor->setStyleSheet("background-color : " + textBorderOutColor);
        ui->spinTextBorderWidth->setValue(tW);
        ui->spinTextBorderOutWidth->setValue(tOW);


        QString curColor =  mainWin->lyricsWidget()->curColor().name();
        ui->lbCurColor->setStyleSheet("background-color : " + curColor);
        ui->lbCurColor->setText(curColor);

        QString curBorderColor =  mainWin->lyricsWidget()->curBorderColor().name();
        QString curBorderOutColor = mainWin->lyricsWidget()->curBorderOutColor().name();
        ui->lbCurBorderColor->setStyleSheet("background-color : " + curBorderColor);
        ui->lbCurBorderColor->setText(curBorderColor);
        ui->lbCurBorderOutColor->setStyleSheet("background-color : " + curBorderOutColor);
        ui->lbCurBorderOutColor->setText(curBorderOutColor);

        int w = mainWin->lyricsWidget()->curBorderWidth();
        int w2 = mainWin->lyricsWidget()->curBorderOutWidth();
        ui->spinCurBorderWidth->setValue(w);
        ui->spinCurBorderOutWidth->setValue(w2);

        connect(ui->spinLint1Y, SIGNAL(valueChanged(int)),
                this, SLOT(onSpinLine1YValueChanged(int)));
        connect(ui->spinLint2Y, SIGNAL(valueChanged(int)),
                this, SLOT(onSpinLine2YValueChanged(int)));
        connect(ui->spinAnmtTime, SIGNAL(valueChanged(int)),
                this, SLOT(onSpinAnmtTimeValueChanged(int)));
        connect(ui->chbLyrAutoSize, SIGNAL(toggled(bool)),
                this, SLOT(onChbLyrAutoSizeToggled(bool)));

        connect(ui->spinTextBorderWidth, SIGNAL(valueChanged(int)),
                this, SLOT(onSpinTextBorderWidthValueChanged(int)));
        connect(ui->spinTextBorderOutWidth, SIGNAL(valueChanged(int)),
                this, SLOT(onSpinTextBorderOutWidthValueChanged(int)));

        connect(ui->spinCurBorderWidth, SIGNAL(valueChanged(int)),
                this, SLOT(onSpinCurBorderWidthValueChanged(int)));
        connect(ui->spinCurBorderOutWidth, SIGNAL(valueChanged(int)),
                this, SLOT(onSpinCurBorderOutWidthValueChanged(int)));
    }


    // Database
    ui->leNCNPath->setText(db->ncnPath());
    ui->leHNKPath->setText(db->hnkPath());
    ui->lbCountSongsValue->setText(QString::number(db->count()) + " เพลง");

    if (db->isRunning())
    {
        ui->barUpdateSongs->setMaximum(db->updateCount());
        ui->btnUpdateSongs->setEnabled(false);
        ui->btnNCNPath->setEnabled(false);
        ui->lbCountSongsText->setEnabled(false);
        ui->lbCountSongsValue->setEnabled(false);
    }
    else
    {
        ui->lbUpdateText->hide();
        ui->lbUpdateValue->hide();
    }

    connect(db, SIGNAL(updateCountChanged(int)), ui->barUpdateSongs, SLOT(setMaximum(int)));
    connect(db, SIGNAL(updatePositionChanged(int)), ui->barUpdateSongs, SLOT(setValue(int)));
    connect(db, SIGNAL(updateSongNameChanged(QString)), ui->lbUpdateValue, SLOT(setText(QString)));
    connect(db, SIGNAL(finished()), this, SLOT(on_upDbUpdateFinished()));


    // Device
    initDeviceTab();


    { // Synth tab
        MidiSynthesizer *synth =  mainWin->midiPlayer()->midiSynthesizer();

        ui->chbSfLoadAll->setChecked(synth->isLoadAllSoundfont());
        ui->listsfFiles->addItems(synth->soundfontFiles());

        if (synth->equalizer31BandFXs()[0]->isOn())
            ui->btnEq->setIcon(QIcon(":/Icons/circle_green.png"));
        else
            ui->btnEq->setIcon(QIcon(":/Icons/circle_red.png"));

        if (synth->reverbFXs()[0]->isOn())
            ui->btnReverb->setIcon(QIcon(":/Icons/circle_green.png"));
        else
            ui->btnReverb->setIcon(QIcon(":/Icons/circle_red.png"));

        if (synth->chorusFXs()[0]->isOn())
            ui->btnChorus->setIcon(QIcon(":/Icons/circle_green.png"));
        else
            ui->btnChorus->setIcon(QIcon(":/Icons/circle_red.png"));


        connect(ui->sliderSfVolume, SIGNAL(valueChanged(int)),
                   this, SLOT(onSliderSfValueChanged(int)));

        connect(ui->listsfFiles, SIGNAL(currentRowChanged(int)),
                this, SLOT(onListSfCurrentRowChanged(int)));

        connect(ui->chbSfLoadAll, SIGNAL(toggled(bool)),
                this, SLOT(onChbSfLoadAllToggled(bool)));
    }

}

SettingsDialog::~SettingsDialog()
{
    MidiSynthesizer *synth = mainWin->midiPlayer()->midiSynthesizer();

    // setting soundfont volume
    settings->beginWriteArray("SynthSoundfontsVolume");
    for (int i=0; i<ui->listsfFiles->count(); i++) {
        settings->setArrayIndex(i);
        settings->setValue("SoundfontVolume", (int)(synth->soundfontVolume(i) * 100));
    }
    settings->endArray();

    delete settings;
    delete ui;
}

void SettingsDialog::setLabelFontInfo(QFont *font)
{
    QString fFamily = font->family();
    QString fSize = QString::number(font->pointSize());
    QString fStyle = "";

    switch (font->weight()) {
        case QFont::Thin:       fStyle = "Thin";        break;
        case QFont::ExtraLight: fStyle = "ExtraLight";  break;
        case QFont::Light:      fStyle = "Light";       break;
        case QFont::Normal:     fStyle = "Normal";      break;
        case QFont::Medium:     fStyle = "Medium";      break;
        case QFont::DemiBold:   fStyle = "DemiBold";    break;
        case QFont::Bold:       fStyle = "Bold";        break;
        case QFont::ExtraBold:  fStyle = "ExtraBold";   break;
        case QFont::Black:      fStyle = "Black";       break;
    }

    if (font->italic()) fStyle += " Italic";
    if (font->underline()) fStyle += " Underline";
    if (font->strikeOut()) fStyle += " StrikeOut";

    ui->leFont->setText(" " + fFamily + " " + fStyle + " " + fSize);
}

void SettingsDialog::initDeviceTab()
{
    // Midi out device
    MidiPlayer *player = mainWin->midiPlayer();
    int dfd = player->midiOutPortNumber();


    for (QString d : MidiPlayer::midiDevices()) {
        ui->cbMidiOut->addItem(d);
    }
    ui->cbMidiOut->addItem("Midi Synthesizer (SoundFont)");
    if (dfd == -1)
        ui->cbMidiOut->setCurrentIndex( ui->cbMidiOut->count() - 1 );
    else
        ui->cbMidiOut->setCurrentIndex( dfd );


    // Midi In device
    ui->cbMidiIn->addItem("None");
    for (QString deviceName : MidiPlayer::midiInDevices()) {
        ui->cbMidiIn->addItem(deviceName);
    }
    ui->cbMidiIn->setCurrentIndex(player->midiInPortNumber()+1);


    // Audio devices
    QStringList dvnames = MidiSynthesizer::audioDevices();
    int aSelected = 0;
    for (int i=1; i<dvnames.count(); i++)
    {
        ui->cbAudioOut->addItem(dvnames[i]);
        if (i == player->midiSynthesizer()->defaultDevice())
            aSelected = i-1;
    }
    ui->cbAudioOut->setCurrentIndex(aSelected);


    { // lock drum, snare, bass
        ui->cbLockDrum->addItems(MidiHelper::drumKitNumberNames());
        ui->cbLockSnare->addItems(MidiHelper::snareNumberName());

        QStringList intsNames = MidiHelper::GMInstrumentNumberNames();
        for (int i=32; i<40; i++) {
            ui->cbLockBass->addItem(intsNames[i]);
        }

        ui->cbLockDrum->setCurrentIndex( player->lockDrumNumber() );
        ui->cbLockBass->setCurrentIndex( player->lockBassNumber() - 32 );
        if (player->lockSnareNumber() == 38)
            ui->cbLockSnare->setCurrentIndex(0);
        else
            ui->cbLockSnare->setCurrentIndex(1);

        // check lock or not
        if (player->isLockDrum())
            ui->chbLockDrum->setChecked(true);
        else
            ui->cbLockDrum->setEnabled(false);

        if (player->isLockSnare())
            ui->chbLockSnare->setChecked(true);
        else
            ui->cbLockSnare->setEnabled(false);

        if (player->isLockBass())
            ui->chbLockBass->setChecked(true);
        else
            ui->cbLockBass->setEnabled(false);
    }

    MidiSynthesizer *synth = player->midiSynthesizer();
    ui->chbSynthFloat->setChecked(synth->isUseFloattingPoint());
    ui->chbSynthFx->setChecked(synth->isUseFXRC());
    ui->sliderBuffer->setValue(BASS_GetConfig(BASS_CONFIG_BUFFER));

    connect(ui->chbLockDrum, SIGNAL(toggled(bool)), this, SLOT(onChbLockDrumToggled(bool)));
    connect(ui->chbLockSnare, SIGNAL(toggled(bool)), this, SLOT(onChbLockSnareToggled(bool)));
    connect(ui->chbLockBass, SIGNAL(toggled(bool)), this, SLOT(onChbLockBassToggled(bool)));

    connect(ui->chbSynthFloat, SIGNAL(toggled(bool)), this, SLOT(onChbFloatPointToggled(bool)));
    connect(ui->chbSynthFx, SIGNAL(toggled(bool)), this, SLOT(onChbUseFXToggled(bool)));
    connect(ui->sliderBuffer, SIGNAL(valueChanged(int)), this, SLOT(onSliderBufferValueChanged(int)));
}

void SettingsDialog::on_chbRemoveFromList_toggled(bool checked)
{
    settings->setValue("RemoveFromPlaylist", checked);
    mainWin->setRemoveFromPlaylist(checked);
}

void SettingsDialog::on_chbAutoPlaynext_toggled(bool checked)
{
    settings->setValue("AutoPlayNext", checked);
    mainWin->setAutoPlayNext(checked);
}

void SettingsDialog::onSpinSearchValueChanged(int i)
{
    settings->setValue("SearchTimeout", i);
    mainWin->setSearchTimeout(i);
}

void SettingsDialog::onSpinPlaylistValueChanged(int i)
{
    settings->setValue("PlaylistTimeout", i);
    mainWin->setPlaylistTimeout(i);
}

void SettingsDialog::onRadioBgColorToggled(bool checked)
{
    if (checked) {
        ui->lbBgColorN->setEnabled(true);
        ui->lbBgColor->setEnabled(true);
        ui->btnBgColor->setEnabled(true);

        ui->lbBgImgN->setEnabled(false);
        ui->btnBgImg->setEnabled(false);
        ui->leBgImgPath->setEnabled(false);

        settings->setValue("BackgroundType", 0);
        mainWin->setBackgroundColor(ui->lbBgColor->text());
        if (mainWin->secondMonitorDlg() != nullptr)
            mainWin->secondMonitorDlg()->setBackgroundColor(ui->lbBgColor->text());
    }
}

void SettingsDialog::onRadioBgImgToggled(bool checked)
{
    if (checked) {
        ui->lbBgImgN->setEnabled(true);
        ui->btnBgImg->setEnabled(true);
        ui->leBgImgPath->setEnabled(true);

        ui->lbBgColorN->setEnabled(false);
        ui->lbBgColor->setEnabled(false);
        ui->btnBgColor->setEnabled(false);

        settings->setValue("BackgroundType", 1);
        mainWin->setBackgroundImage(ui->leBgImgPath->text());
        if (mainWin->secondMonitorDlg() != nullptr)
            mainWin->secondMonitorDlg()->setBackgroundImage(ui->leBgImgPath->text());
    }
}

void SettingsDialog::on_btnBgColor_clicked()
{
    QColor lC(ui->lbBgColor->text());
    QColor color = QColorDialog::getColor(lC, this, "Select Color",
                                          QColorDialog::DontUseNativeDialog);
    if (color.isValid()) {
        settings->setValue("BackgroundColor", color.name());
        ui->lbBgColor->setText(color.name());
        ui->lbBgColor->setStyleSheet("background-color : " + color.name());
        mainWin->setBackgroundColor(color.name());
        if (mainWin->secondMonitorDlg() != nullptr)
            mainWin->secondMonitorDlg()->setBackgroundColor(color.name());
    }
}

void SettingsDialog::on_btnBgImg_clicked()
{
    //QStringList filters;
    //filters << "Image files (*.png *.PNG *.jpg *.JPG)";
    QString f = QFileDialog::getOpenFileName(this, "Select File", Utils::LAST_OPEN_DIR,
                                             "Image files (*.png *.PNG *.jpg *.JPG)");
    if (f != "") {
        settings->setValue("BackgroundImage", f);
        ui->leBgImgPath->setText(f);
        mainWin->setBackgroundImage(f);
        if (mainWin->secondMonitorDlg() != nullptr)
            mainWin->secondMonitorDlg()->setBackgroundImage(f);

        Utils::LAST_OPEN_DIR = QFileInfo(f).dir().absolutePath();//QDir().absoluteFilePath(f);
    }
}

void SettingsDialog::on_chbAutoCheckUpdate_toggled(bool checked)
{
    #ifdef _WIN32
    win_sparkle_set_automatic_check_for_updates(checked);
    #endif

    ui->lbUpdateInterval->setEnabled(checked);
    ui->spinUpdateInterval->setEnabled(checked);
}

void SettingsDialog::onSpinUpdateValueChanged(int value)
{
    #ifdef _WIN32
    win_sparkle_set_update_check_interval(value * 60 * 60);
    #endif
}

void SettingsDialog::on_btnNCNPath_clicked()
{
    QString path = QFileDialog::getExistingDirectory(this, tr("เลือกที่เก็บไฟล์เพลง NCN"), Utils::LAST_OPEN_DIR,
                                                    QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if (path == "")
        return;

    Utils::LAST_OPEN_DIR = path;

    if (db->isNCNPath(path)) {
        settings->setValue("NCNPath", path);
        ui->leNCNPath->setText(path);
        db->setNcnPath(path);
    } else {
        QString title = "ที่เก็บเพลงไม่ถูกต้อง";
        QString msg = "ต้องมีโฟลเดอร์ Cursor, Lyrics, และ Song อยู่ในที่เก็บเพลง";
        QMessageBox::information(this, title, msg, QMessageBox::Ok);
    }

}

void SettingsDialog::on_btnHNKPath_clicked()
{
    QString path = QFileDialog::getExistingDirectory(
                this, tr("เลือกที่เก็บไฟล์เพลง HNK"), Utils::LAST_OPEN_DIR,
                QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if (path == "")
        return;

    Utils::LAST_OPEN_DIR = path;

    ui->leHNKPath->setText(path);
    db->setHNKPath(path);
    settings->setValue("HNKPath", path);
}

void SettingsDialog::on_btnUpdateSongs_clicked()
{
    if (!db->isNCNPath(ui->leNCNPath->text())) {
        QString title = "ไม่สามารถปรับปรุงฐานข้อมูลได้";
        QString msg = "ไม่มีโฟลเดอร์ Cursor, Lyrics, หรือ Song อยู่ในที่เก็บเพลง NCN"
                      "\nโปรดเลือกตำแหน่งที่เก็บเพลงให้ถูกต้อง";
        QMessageBox::information(this, title, msg, QMessageBox::Ok);
        return;
    }

    settings->setValue("NCNPath", ui->leNCNPath->text());
    settings->setValue("HNKPath", ui->leHNKPath->text());

    ui->lbUpdateText->show();
    ui->lbUpdateValue->show();

    ui->btnUpdateSongs->setEnabled(false);
    ui->btnNCNPath->setEnabled(false);
    ui->btnHNKPath->setEnabled(false);
    ui->lbCountSongsText->setEnabled(false);
    ui->lbCountSongsValue->setEnabled(false);

    db->setUpdateType(UpdateType::UpdateAll);
    db->start();
}

void SettingsDialog::on_upDbUpdateFinished()
{ 
    ui->btnHNKPath->setEnabled(true);
    ui->btnNCNPath->setEnabled(true);
    ui->lbCountSongsText->setEnabled(true);
    ui->lbCountSongsValue->setEnabled(true);
    ui->lbUpdateText->hide();
    ui->lbUpdateValue->hide();
    ui->btnUpdateSongs->setEnabled(true);
    ui->lbCountSongsValue->setText(QString::number(db->count()) + " เพลง");
}

void SettingsDialog::on_btnMapChannel_clicked()
{
    MapChannelDialog d(this, mainWin->midiPlayer());
    d.setModal(true);
    d.adjustSize();
    d.setMinimumSize(d.size());
    d.exec();
}

void SettingsDialog::on_cbMidiOut_activated(int index)
{
    mainWin->stop();

    if (index == (ui->cbMidiOut->count() - 1)) {
        settings->setValue("MidiOut", -1);
        mainWin->midiPlayer()->setMidiOut(-1);
    } else {
        settings->setValue("MidiOut", index);
        mainWin->midiPlayer()->setMidiOut(index);
    }

    QList<int> ports;
    for (int i=0; i<16; i++) {
        ports.append(mainWin->midiPlayer()->midiChannel()[i].port());
    }
    settings->setValue("MidiChannelMapper", QVariant::fromValue(ports));
}

void SettingsDialog::on_cbMidiIn_activated(int index)
{
    mainWin->midiPlayer()->setMidiIn(index-1);
    settings->setValue("MidiIn", index-1);
}

void SettingsDialog::on_cbAudioOut_activated(int index)
{
    /*if (ui->cbAudioOut->currentText() == "Default") {
        BASS_SetDevice(-1);
        settings->setValue("AudioOut", -1);
        return;
    }*/
    MidiSynthesizer *synth = mainWin->midiPlayer()->midiSynthesizer();

    synth->setDefaultDevice(index+1);
    settings->setValue("SynthDefaultDevice", index+1);
    /*if (BASS_SetDevice(index) == BASS_OK) {
        settings->setValue("defaultAudioDevice", index);
    } else {
        QString title = "ไม่สามรถใช้อุปกรณ์เสียงที่เลือกได้";
        QString msg = "โปรดเลือกอุปกรณ์เสียงขาออกตัวอื่น หรือลองใหม่ในภายหลัง";
        QMessageBox::information(this, title, msg, QMessageBox::Ok);
        BASS_SetDevice(-1);
        ui->cbAudioOut->setCurrentIndex( BASS_GetDevice() );
    }*/
}

void SettingsDialog::onChbLockDrumToggled(bool checked)
{
    if (checked) {
        ui->cbLockDrum->setEnabled(true);
        mainWin->midiPlayer()->setLockDrum(true, ui->cbLockDrum->currentIndex());
    }
    else {
        ui->cbLockDrum->setEnabled(false);
        mainWin->midiPlayer()->setLockDrum(false);
    }

    settings->setValue("MidiLockDrum", checked);
}

void SettingsDialog::onChbLockSnareToggled(bool checked)
{
    if (checked) {
        ui->cbLockSnare->setEnabled(true);
        if (ui->cbLockSnare->currentIndex() == 0)
            mainWin->midiPlayer()->setLockSnare(true, 38);
        else
            mainWin->midiPlayer()->setLockSnare(true, 40);
    }
    else {
        ui->cbLockSnare->setEnabled(false);
        mainWin->midiPlayer()->setLockSnare(false);
    }

    settings->setValue("MidiLockSnare", checked);
}

void SettingsDialog::onChbLockBassToggled(bool checked)
{
    if (checked) {
        ui->cbLockBass->setEnabled(true);
        mainWin->midiPlayer()->setLockBass(true, ui->cbLockBass->currentIndex() + 32);
    }
    else {
        ui->cbLockBass->setEnabled(false);
        mainWin->midiPlayer()->setLockBass(false);
    }

    settings->setValue("MidiLockBass", checked);
}

void SettingsDialog::on_cbLockDrum_activated(int index)
{
    mainWin->midiPlayer()->setLockDrum(true, index);
    settings->setValue("MidiLockDrumNumber", index);
}

void SettingsDialog::on_cbLockSnare_activated(int index)
{
    int n = 38;
    if (index == 0)
        n = 38;
    else
        n = 40;

    mainWin->midiPlayer()->setLockSnare(true, n);
    settings->setValue("MidiLockSnareNumber", n);
}

void SettingsDialog::on_cbLockBass_activated(int index)
{
    mainWin->midiPlayer()->setLockBass(true, index + 32);
    settings->setValue("MidiLockBassNumber", index + 32);
}

void SettingsDialog::onChbFloatPointToggled(bool checked)
{
    if (mainWin->midiPlayer()->midiOutPortNumber() == -1)
        mainWin->stop();

    mainWin->midiPlayer()->midiSynthesizer()->setUsetFloattingPoint(checked);
    settings->setValue("SynthFloatPoint", checked);
}

void SettingsDialog::onChbUseFXToggled(bool checked)
{
    mainWin->midiPlayer()->midiSynthesizer()->setUseFXRC(checked);
    settings->setValue("SynthUseFXRC", checked);
}

void SettingsDialog::onSliderBufferValueChanged(int value)
{
    MidiSynthesizer *synth = mainWin->midiPlayer()->midiSynthesizer();

    if (!mainWin->midiPlayer()->isPlayerStopped())
        mainWin->stop();

    if (synth->isOpened()) {
        synth->close();
        BASS_SetConfig(BASS_CONFIG_BUFFER, value);
        synth->open();
    } else {
        BASS_SetConfig(BASS_CONFIG_BUFFER, value);
    }

    settings->setValue("SynthBuffer", value);
}

void SettingsDialog::on_btnFont_clicked()
{
    bool ok;
    QFont f = QFontDialog::getFont(&ok, mainWin->lyricsWidget()->textFont(),
                                   this, "Select Font", QFontDialog::DontUseNativeDialog);
    if (ok) {
        settings->setValue("LyricsFamily", f.family());
        settings->setValue("LyricsSize", f.pointSize());
        settings->setValue("LyricsWeight", f.weight());
        settings->setValue("LyricsItalic", f.italic());
        settings->setValue("LyricsUnderline", f.underline());
        settings->setValue("LyricsStrikeOut", f.strikeOut());

        setLabelFontInfo(&f);

        mainWin->lyricsWidget()->setTextFont(f);
        if (mainWin->secondLyrics() != nullptr)
            mainWin->secondLyrics()->setTextFont(f);
    }
}

void SettingsDialog::onSpinLine1YValueChanged(int v)
{
    settings->setValue("LyricsLine1Y", v);
    mainWin->lyricsWidget()->setLine1Y(v);
    if (mainWin->secondLyrics() != nullptr)
        mainWin->secondLyrics()->setLine1Y(v);
}

void SettingsDialog::onSpinLine2YValueChanged(int v)
{
    settings->setValue("LyricsLine2Y", v);
    mainWin->lyricsWidget()->setLine2Y(v);
    if (mainWin->secondLyrics() != nullptr)
        mainWin->secondLyrics()->setLine2Y(v);
}

void SettingsDialog::onSpinAnmtTimeValueChanged(int v)
{
    settings->setValue("LyricsAnimationTime", v);
    mainWin->lyricsWidget()->setAnimationTime(v);
    if (mainWin->secondLyrics() != nullptr)
        mainWin->secondLyrics()->setAnimationTime(v);
}

void SettingsDialog::onChbLyrAutoSizeToggled(bool checked)
{
    settings->setValue("LyricsAutoFontSize", checked);
    mainWin->lyricsWidget()->setAutoFontSize(checked);
    if (mainWin->secondLyrics() != nullptr)
        mainWin->secondLyrics()->setAutoFontSize(checked);
}

void SettingsDialog::on_cbLine1X_activated(int index)
{
    settings->setValue("LyricsLine1X", index);
    LinePosition p = static_cast<LinePosition>(index);
    mainWin->lyricsWidget()->setLine1Position(p);
    if (mainWin->secondLyrics() != nullptr)
        mainWin->secondLyrics()->setLine1Position(p);
}

void SettingsDialog::on_cbLine2X_activated(int index)
{
    settings->setValue("LyricsLine2X", index);
    LinePosition p = static_cast<LinePosition>(index);
    mainWin->lyricsWidget()->setLine2Position(p);
    if (mainWin->secondLyrics() != nullptr)
        mainWin->secondLyrics()->setLine2Position(p);
}

void SettingsDialog::on_btnTextColor_clicked()
{
    QColor lC = mainWin->lyricsWidget()->textColor();
    QColor color = QColorDialog::getColor(lC, this, "Select Color",
                                          QColorDialog::DontUseNativeDialog);

    if (color.isValid()) {
        QString cn = color.name();
        settings->setValue("LyricsTextColor", cn);
        ui->lbTextColor->setStyleSheet("background-color : " + cn);
        ui->lbTextColor->setText(cn);

        mainWin->lyricsWidget()->setTextColor(color);
        if (mainWin->secondLyrics() != nullptr)
            mainWin->secondLyrics()->setTextColor(color);
    }
}

void SettingsDialog::on_btnTextBorderColor_clicked()
{
    QColor lC = mainWin->lyricsWidget()->textBorderColor();
    QColor color = QColorDialog::getColor(lC, this, "Select Color",
                                          QColorDialog::DontUseNativeDialog);

    if (color.isValid()) {
        QString cn = color.name();
        settings->setValue("LyricsTextBorderColor", cn);
        ui->lbTextBorderColor->setStyleSheet("background-color : " + cn);
        ui->lbTextBorderColor->setText(cn);

        mainWin->lyricsWidget()->setTextBorderColor(color);
        if (mainWin->secondLyrics() != nullptr)
            mainWin->secondLyrics()->setTextBorderColor(color);
    }
}

void SettingsDialog::on_btnTextBorderOutColor_clicked()
{
    QColor lC = mainWin->lyricsWidget()->textBorderOutColor();
    QColor color = QColorDialog::getColor(lC, this, "Select Color",
                                          QColorDialog::DontUseNativeDialog);

    if (color.isValid()) {
        QString cn = color.name();
        settings->setValue("LyricsTextBorderOutColor", cn);
        ui->lbTextBorderOutColor->setStyleSheet("background-color : " + cn);
        ui->lbTextBorderOutColor->setText(cn);

        mainWin->lyricsWidget()->setTextBorderOutColor(color);
        if (mainWin->secondLyrics() != nullptr)
            mainWin->secondLyrics()->setTextBorderOutColor(color);
    }
}

void SettingsDialog::onSpinTextBorderWidthValueChanged(int arg1)
{
    settings->setValue("LyricsTextBorderWidth", arg1);
    mainWin->lyricsWidget()->setTextBorderWidth(arg1);
    if (mainWin->secondLyrics() != nullptr)
        mainWin->secondLyrics()->setTextBorderWidth(arg1);
}

void SettingsDialog::onSpinTextBorderOutWidthValueChanged(int arg1)
{
    settings->setValue("LyricsTextBorderOutWidth", arg1);
    mainWin->lyricsWidget()->setTextBorderOutWidth(arg1);
    if (mainWin->secondLyrics() != nullptr)
        mainWin->secondLyrics()->setTextBorderOutWidth(arg1);
}

void SettingsDialog::on_btnCurColor_clicked()
{
    QColor lC = mainWin->lyricsWidget()->curColor();
    QColor color = QColorDialog::getColor(lC, this, "Select Color",
                                          QColorDialog::DontUseNativeDialog);

    if (color.isValid()) {
        QString cn = color.name();
        settings->setValue("LyricsCurColor", cn);
        ui->lbCurColor->setStyleSheet("background-color : " + cn);
        ui->lbCurColor->setText(cn);

        mainWin->lyricsWidget()->setCurColor(color);
        if (mainWin->secondLyrics() != nullptr)
            mainWin->secondLyrics()->setCurColor(color);
    }
}

void SettingsDialog::on_btnCurBorderColor_clicked()
{
    QColor lC = mainWin->lyricsWidget()->curBorderColor();
    QColor color = QColorDialog::getColor(lC, this, "Select Color",
                                          QColorDialog::DontUseNativeDialog);

    if (color.isValid()) {
        QString cn = color.name();
        settings->setValue("LyricsCurBorderColor", cn);
        ui->lbCurBorderColor->setStyleSheet("background-color : " + cn);
        ui->lbCurBorderColor->setText(cn);

        mainWin->lyricsWidget()->setCurBorderColor(color);
        if (mainWin->secondLyrics() != nullptr)
            mainWin->secondLyrics()->setCurBorderColor(color);
    }
}

void SettingsDialog::on_btnCurBorderOutColor_clicked()
{
    QColor lC = mainWin->lyricsWidget()->curBorderOutColor();
    QColor color = QColorDialog::getColor(lC, this, "Select Color",
                                          QColorDialog::DontUseNativeDialog);

    if (color.isValid()) {
        QString cn = color.name();
        settings->setValue("LyricsCurBorderOutColor", cn);
        ui->lbCurBorderOutColor->setStyleSheet("background-color : " + cn);
        ui->lbCurBorderOutColor->setText(cn);

        mainWin->lyricsWidget()->setCurBorderOutColor(color);
        if (mainWin->secondLyrics() != nullptr)
            mainWin->secondLyrics()->setCurBorderOutColor(color);
    }
}

void SettingsDialog::onSpinCurBorderWidthValueChanged(int arg1)
{
    settings->setValue("LyricsCurBorderWidth", arg1);
    mainWin->lyricsWidget()->setCurBorderWidth(arg1);
    if (mainWin->secondLyrics() != nullptr)
        mainWin->secondLyrics()->setCurBorderWidth(arg1);
}

void SettingsDialog::onSpinCurBorderOutWidthValueChanged(int arg1)
{
    settings->setValue("LyricsCurBorderOutWidth", arg1);
    mainWin->lyricsWidget()->setCurBorderOutWidth(arg1);
    if (mainWin->secondLyrics() != nullptr)
        mainWin->secondLyrics()->setCurBorderOutWidth(arg1);
}

void SettingsDialog::on_btnSfAdd_clicked()
{
    QStringList sfFiles = QFileDialog::getOpenFileNames(this,
                                                        "เลือกไฟล์ซาวด์ฟ้อนท์",
                                                        Utils::LAST_OPEN_DIR,
                                                        "SoundFont (*.sf2 *.SF2 *.sfz *.SFZ)");

    if (sfFiles.count() > 0)
        Utils::LAST_OPEN_DIR = QFileInfo(sfFiles[0]).dir().absolutePath();

    MidiSynthesizer *synth = mainWin->midiPlayer()->midiSynthesizer();
    for (const QString &sf : sfFiles)
    {
        if (synth->addSoundfont(sf))
            ui->listsfFiles->addItem(sf);
        else
        {
            QString title = "ไฟล์ซาวด์ฟ้อนท์ไม่ถูกต้อง";
            QString msg = "ไม่สามารถอ่านไฟล์ " + sf + " ได้"
                          "\nโปรดลองเลือกไฟล์อื่น";
            QMessageBox::warning(this, title, msg, QMessageBox::Ok);
        }
    }
    settings->setValue("SynthSoundfonts", QVariant::fromValue(synth->soundfontFiles()));
    settings->setValue("SynthSoundfontsMap", QVariant::fromValue(synth->getMapSoundfontIndex()));
    settings->setValue("SynthSoundfontsDrumMap", QVariant::fromValue(synth->getDrumMapSfIndex()));
}

void SettingsDialog::on_btnSfRemove_clicked()
{
    int i = ui->listsfFiles->currentRow();
    if (i < 0)
        return;

    delete ui->listsfFiles->takeItem(i);

    MidiSynthesizer *synth = mainWin->midiPlayer()->midiSynthesizer();
    synth->removeSoundfont(i);

    settings->setValue("SynthSoundfonts", QVariant::fromValue(synth->soundfontFiles()));
    settings->setValue("SynthSoundfontsMap", QVariant::fromValue(synth->getMapSoundfontIndex()));
    settings->setValue("SynthSoundfontsDrumMap", QVariant::fromValue(synth->getDrumMapSfIndex()));
}

void SettingsDialog::on_btnSfUp_clicked()
{
    int i = ui->listsfFiles->currentRow();
    if (i <= 0)
        return;

    QListWidgetItem *item = ui->listsfFiles->takeItem(i);
    ui->listsfFiles->insertItem(i-1, item);
    ui->listsfFiles->setCurrentRow(i-1);

    MidiSynthesizer *synth = mainWin->midiPlayer()->midiSynthesizer();
    synth->swapSoundfont(i, i-1);

    settings->setValue("SynthSoundfonts", QVariant::fromValue(synth->soundfontFiles()));
    settings->setValue("SynthSoundfontsMap", QVariant::fromValue(synth->getMapSoundfontIndex()));
    settings->setValue("SynthSoundfontsDrumMap", QVariant::fromValue(synth->getDrumMapSfIndex()));
}

void SettingsDialog::on_btnSfDown_clicked()
{
    int i = ui->listsfFiles->currentRow();
    if (i == (ui->listsfFiles->count() - 1) )
        return;

    QListWidgetItem *item = ui->listsfFiles->takeItem(i);
    ui->listsfFiles->insertItem(i+1, item);
    ui->listsfFiles->setCurrentRow(i+1);

    MidiSynthesizer *synth = mainWin->midiPlayer()->midiSynthesizer();
    synth->swapSoundfont(i, i+1);

    settings->setValue("SynthSoundfonts", QVariant::fromValue(synth->soundfontFiles()));
    settings->setValue("SynthSoundfontsMap", QVariant::fromValue(synth->getMapSoundfontIndex()));
    settings->setValue("SynthSoundfontsDrumMap", QVariant::fromValue(synth->getDrumMapSfIndex()));
}

void SettingsDialog::onSliderSfValueChanged(int value)
{
    int i = ui->listsfFiles->currentRow();
    if (i == -1)
        return;

    MidiSynthesizer *synth = mainWin->midiPlayer()->midiSynthesizer();
    synth->setSoundfontVolume(i, value / 100.0f);
    ui->lbSfVolume->setText(QString::number(value));
}

void SettingsDialog::onListSfCurrentRowChanged(int currentRow)
{
    if (currentRow == -1)
        return;

    disconnect(ui->sliderSfVolume, SIGNAL(valueChanged(int)),
               this, SLOT(onSliderSfValueChanged(int)));

    MidiSynthesizer *synth = mainWin->midiPlayer()->midiSynthesizer();
    int v = synth->soundfontVolume(currentRow) * 100;
    ui->sliderSfVolume->setValue(v);
    ui->lbSfVolume->setText(QString::number(v));

    connect(ui->sliderSfVolume, SIGNAL(valueChanged(int)),
            this, SLOT(onSliderSfValueChanged(int)));
}

void SettingsDialog::onChbSfLoadAllToggled(bool value)
{
    mainWin->midiPlayer()->midiSynthesizer()->setLoadAllSoundfont(value);
    settings->setValue("SynthSoundfontsLoadAll", value);
}

void SettingsDialog::on_btnSfMap_clicked()
{
    MapSoundfontDialog msfDlg(this, mainWin->midiPlayer()->midiSynthesizer());
    msfDlg.setModal(true);
    msfDlg.setMinimumSize(msfDlg.size());
    msfDlg.exec();
}

void SettingsDialog::on_btnEq_clicked()
{
    if (Equalizer31BandDialog::isOpenned())
        return;

    MidiSynthesizer *synth = mainWin->midiPlayer()->midiSynthesizer();

    Equalizer31BandDialog eqdlg(this, synth->equalizer31BandFXs());
    eqdlg.setModal(true);
    eqdlg.adjustSize();
    eqdlg.setFixedSize(eqdlg.size());
    eqdlg.exec();

    if (synth->equalizer31BandFXs()[0]->isOn())
        ui->btnEq->setIcon(QIcon(":/Icons/circle_green.png"));
    else
        ui->btnEq->setIcon(QIcon(":/Icons/circle_red.png"));
}

void SettingsDialog::on_btnReverb_clicked()
{
    if (ReverbDialog::isOpenned())
        return;

    MidiSynthesizer *synth = mainWin->midiPlayer()->midiSynthesizer();

    ReverbDialog rvDlg(this, synth->reverbFXs());
    rvDlg.setModal(true);
    rvDlg.adjustSize();
    rvDlg.setFixedSize(rvDlg.size());
    rvDlg.exec();

    if (synth->reverbFXs()[0]->isOn())
        ui->btnReverb->setIcon(QIcon(":/Icons/circle_green.png"));
    else
        ui->btnReverb->setIcon(QIcon(":/Icons/circle_red.png"));
}

void SettingsDialog::on_btnChorus_clicked()
{
    if (ChorusDialog::isOpenned())
        return;

    MidiSynthesizer *synth = mainWin->midiPlayer()->midiSynthesizer();

    ChorusDialog crDlg(this, synth->chorusFXs());
    crDlg.setModal(true);
    crDlg.adjustSize();
    crDlg.setFixedSize(crDlg.size());
    crDlg.exec();

    if (synth->chorusFXs()[0]->isOn())
        ui->btnChorus->setIcon(QIcon(":/Icons/circle_green.png"));
    else
        ui->btnChorus->setIcon(QIcon(":/Icons/circle_red.png"));
}

void SettingsDialog::on_btnClose_clicked()
{
    close();
}
