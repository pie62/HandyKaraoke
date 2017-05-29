#include "SettingsDialog.h"
#include "ui_SettingsDialog.h"

#include <QDebug>
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
    settings = new QSettings();
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

        QString textColor =  mainWin->lyricsWidget()->textColor().name();
        QString textBorderColor = mainWin->lyricsWidget()->textBorderColor().name();
        int tW = mainWin->lyricsWidget()->textBorderWidth();
        ui->lbTextColor->setText(textColor);
        ui->lbTextColor->setStyleSheet("background-color : " + textColor);
        ui->lbTextBorderColor->setText(textBorderColor);
        ui->lbTextBorderColor->setStyleSheet("background-color : " + textBorderColor);
        ui->spinTextBorderWidth->setValue(tW);


        QString curColor =  mainWin->lyricsWidget()->curColor().name();
        ui->lbCurColor->setStyleSheet("background-color : " + curColor);
        ui->lbCurColor->setText(curColor);

        QString curBorderColor =  mainWin->lyricsWidget()->curBorderColor().name();
        ui->lbCurBorderColor->setStyleSheet("background-color : " + curBorderColor);
        ui->lbCurBorderColor->setText(curBorderColor);

        int w = mainWin->lyricsWidget()->curBorderWidth();
        ui->spinCurBorderWidth->setValue(w);

        connect(ui->spinTextBorderWidth, SIGNAL(valueChanged(int)), this, SLOT(onSpinTextBorderWidthValueChanged(int)));
        connect(ui->spinCurBorderWidth, SIGNAL(valueChanged(int)), this, SLOT(onSpinCurBorderWidthValueChanged(int)));

    }


    // Database
    QString appPath = QDir::currentPath();
    QString ncnPath = settings->value("NCNPath", QDir::toNativeSeparators(appPath+"/Songs/NCN")).toString();
    ui->leNCNPath->setText(ncnPath);
    ui->lbUpdateText->hide();
    ui->lbUpdateValue->hide();
    ui->lbCountSongsValue->setText(QString::number(db->count()) + " เพลง");

    connect(db, SIGNAL(updateLengthChanged(int)), ui->barUpdateSongs, SLOT(setMaximum(int)));
    connect(db, SIGNAL(updatePositionChanged(int)), ui->barUpdateSongs, SLOT(setValue(int)));
    connect(db, SIGNAL(updateSongNameChanged(QString)), ui->lbUpdateValue, SLOT(setText(QString)));
    connect(db, SIGNAL(updateFinished()), this, SLOT(on_upDbUpdateFinished()));


    // Device
    initDeviceTab();
}

SettingsDialog::~SettingsDialog()
{
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
    // Midi device
    MidiPlayer *player = mainWin->midiPlayer();
    int dfd = player->midiOut();
    //int select = 0;
    /*for (int i=0; i < player->midiDevices().count(); i++) {
        #ifdef __linux__
            ui->cbMidiOut->addItem( player->midiDevices().values()[i] + " " + player->midiDevices().keys()[i] );
        #else
            ui->cbMidiOut->addItem( player->midiDevices().values()[i] );
        #endif
        if (dfd == player->midiDevices().keys()[i])
            select = i;
    }*/
    ui->cbMidiOut->addItems(player->midiDevices());
    ui->cbMidiOut->addItem("SoundFont");
    if (dfd == -1)
        ui->cbMidiOut->setCurrentIndex( ui->cbMidiOut->count() - 1 );
    else
        ui->cbMidiOut->setCurrentIndex( dfd );


    // Audio devices
    // Get the total number of devices and set selected
    int a, count=0, aSelected = 1;
    BASS_DEVICEINFO info;
    for (a=0; BASS_GetDeviceInfo(a, &info); a++) {
        if (info.flags&BASS_DEVICE_ENABLED) { // device is enabled
            ui->cbAudioOut->addItem(QString::fromStdString(info.name));
            if (count == BASS_GetDevice())
                aSelected = count;
            count++; // count it
        }
    }
    ui->cbAudioOut->setCurrentIndex(aSelected);
    // Get default using audio device and set to selected
    /*BASS_DEVICEINFO dInfo;
    BASS_GetDeviceInfo(BASS_GetDevice(), &dInfo);
    QString dName = QString::fromStdString(dInfo.name);
    if (ui->cbAudioOut->ite)*/
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
    }
}

void SettingsDialog::on_btnBgImg_clicked()
{
    //QStringList filters;
    //filters << "Image files (*.png *.PNG *.jpg *.JPG)";
    QString f = QFileDialog::getOpenFileName(this, "Select File", QDir::currentPath(),
                                             "Image files (*.png *.PNG *.jpg *.JPG)");
    if (f != "") {
        settings->setValue("BackgroundImage", f);
        ui->leBgImgPath->setText(f);
        mainWin->setBackgroundImage(f);
    }
}

void SettingsDialog::on_btnNCNPath_clicked()
{
    QString path = QFileDialog::getExistingDirectory(this, tr("เลือกที่เก็บไฟล์เพลง NCN"), QDir::currentPath(),
                                                    QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if (path == "")
        return;

    if (db->isNCNPath(path)) {
        settings->setValue("NCNPath", path);
        ui->leNCNPath->setText(path);
    } else {
        QString title = "ที่เก็บเพลงไม่ถูกต้อง";
        QString msg = "ต้องมีโฟลเดอร์ Cursor, Lyrics, และ Song อยู่ในที่เก็บเพลง";
        QMessageBox::information(this, title, msg, QMessageBox::Ok);
    }

}

void SettingsDialog::on_btnUpdateSongs_clicked()
{
    if (!db->isNCNPath(ui->leNCNPath->text())) {
        QString title = "ไม่สามารถปรับปรุงฐานข้อมูลได้";
        QString msg = "ไม่มีโฟลเดอร์ Cursor, Lyrics, หรือ Song อยู่ในที่เก็บเพลง NCN"
                      "\nโปรดเลือกตำแหน่งที่เก็บเพลงให้ถูกต้องถูกต้อง";
        QMessageBox::information(this, title, msg, QMessageBox::Ok);
        return;
    }

    settings->setValue("NCNPath", ui->leNCNPath->text());
    ui->btnUpdateSongs->setEnabled(false);
    ui->lbUpdateText->show();
    ui->lbUpdateValue->show();

    ui->lbCountSongsText->setEnabled(false);
    ui->lbCountSongsValue->setEnabled(false);

    db->updateInThread();
}

void SettingsDialog::on_upDbUpdateFinished()
{ 
    ui->lbCountSongsText->setEnabled(true);
    ui->lbCountSongsValue->setEnabled(true);
    ui->lbUpdateText->hide();
    ui->lbUpdateValue->hide();
    ui->btnUpdateSongs->setEnabled(true);
    ui->lbCountSongsValue->setText(QString::number(db->count()) + " เพลง");
}

void SettingsDialog::on_cbMidiOut_activated(int index)
{
    //bool playAfterChange = mainWin->midiPlayer()->isPlaying();

    //if (playAfterChange) mainWin->stop();
    mainWin->stop();

    if (ui->cbMidiOut->currentText() == "SoundFont") {
        settings->setValue("MidiOut", -1);
        mainWin->midiPlayer()->setMidiOut(-1);
    } else {
        //QString dv = mainWin->midiPlayer()->midiDevices().keys()[index];
        //settings->setValue("MidiOut", dv);
        //mainWin->midiPlayer()->setMidiOut(dv);
        //mainWin->midiPlayer()->unsetSoundFonts();
        settings->setValue("MidiOut", index);
        mainWin->midiPlayer()->setMidiOut(index);
    }

    //if (playAfterChange) mainWin->play(-1);
}

void SettingsDialog::on_cbAudioOut_activated(int index)
{
    if (ui->cbAudioOut->currentText() == "Default") {
        BASS_SetDevice(-1);
        settings->setValue("AudioOut", -1);
        return;
    }
    if (index != BASS_GetDevice()) {
        BASS_SetDevice(index);
        settings->setValue("AudioOut", index);
    }
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
    }
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
    }
}

void SettingsDialog::onSpinTextBorderWidthValueChanged(int arg1)
{
    settings->setValue("LyricsTextBorderWidth", arg1);
    mainWin->lyricsWidget()->setTextBorderWidth(arg1);
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
    }
}

void SettingsDialog::onSpinCurBorderWidthValueChanged(int arg1)
{
    settings->setValue("LyricsCurBorderWidth", arg1);
    mainWin->lyricsWidget()->setCurBorderWidth(arg1);
}
