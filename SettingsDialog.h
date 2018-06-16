#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include "MainWindow.h"
#include "SongDatabase.h"

#include <QDialog>
#include <QSettings>

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = 0, MainWindow *m = 0);
    ~SettingsDialog();

private slots:
    void setLabelFontInfo(QFont *font);
    void initDeviceTab();

    void on_chbRemoveFromList_toggled(bool checked);
    void on_chbAutoPlaynext_toggled(bool checked);
    void onSpinSearchValueChanged(int i);
    void onSpinPlaylistValueChanged(int i);
    void onRadioBgColorToggled(bool checked);
    void onRadioBgImgToggled(bool checked);
    void on_btnBgColor_clicked();
    void on_btnBgImg_clicked();

    void on_btnNCNPath_clicked();
    void on_btnHNKPath_clicked();
    void on_btnUpdateSongs_clicked();
    void on_upDbUpdateFinished();


    // Output tab
    void on_btnMapChannel_clicked();
    void on_cbMidiOut_activated(int index);
    void on_cbMidiIn_activated(int index);
    void on_cbAudioOut_activated(int index);

    void onChbLockDrumToggled(bool checked);
    void onChbLockSnareToggled(bool checked);
    void onChbLockBassToggled(bool checked);

    void on_cbLockDrum_activated(int index);
    void on_cbLockSnare_activated(int index);
    void on_cbLockBass_activated(int index);

    void onChbFloatPointToggled(bool checked);
    void onChbUseFXToggled(bool checked);
    void onSliderBufferValueChanged(int value);



    // Lyrics tab
    void on_btnFont_clicked();
    void onSpinLine1YValueChanged(int v);
    void onSpinLine2YValueChanged(int v);
    void onSpinAnmtTimeValueChanged(int v);
    void onChbLyrAutoSizeToggled(bool checked);

    void on_cbLine1X_activated(int index);
    void on_cbLine2X_activated(int index);

    void on_btnTextColor_clicked();
    void on_btnTextBorderColor_clicked();
    void on_btnTextBorderOutColor_clicked();
    void onSpinTextBorderWidthValueChanged(int arg1);
    void onSpinTextBorderOutWidthValueChanged(int arg1);

    void on_btnCurColor_clicked();
    void on_btnCurBorderColor_clicked();
    void on_btnCurBorderOutColor_clicked();
    void onSpinCurBorderWidthValueChanged(int arg1);
    void onSpinCurBorderOutWidthValueChanged(int arg1);



    // Synth tab
    void on_btnSfAdd_clicked();
    void on_btnSfRemove_clicked();
    void on_btnSfUp_clicked();
    void on_btnSfDown_clicked();
    void onSliderSfValueChanged(int value);
    void onListSfCurrentRowChanged(int currentRow);
    void onChbSfLoadAllToggled(bool value);
    void on_btnSfMap_clicked();
    void on_btnEq_clicked();
    void on_btnReverb_clicked();
    void on_btnChorus_clicked();

    void on_btnClose_clicked();

private:
    Ui::SettingsDialog *ui;
    MainWindow *mainWin;
    QSettings *settings;
    SongDatabase *db;

    QList<int> instMap, drumMap;
};

#endif // SETTINGSDIALOG_H
