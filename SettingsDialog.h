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
    void on_btnUpdateSongs_clicked();
    void on_upDbUpdateFinished();
    void on_cbMidiOut_activated(int index);
    void on_cbAudioOut_activated(int index);

    void on_btnFont_clicked();
    void on_btnTextColor_clicked();
    void on_btnTextBorderColor_clicked();
    void onSpinTextBorderWidthValueChanged(int arg1);
    void on_btnCurColor_clicked();
    void on_btnCurBorderColor_clicked();
    void onSpinCurBorderWidthValueChanged(int arg1);

private:
    Ui::SettingsDialog *ui;
    MainWindow *mainWin;
    QSettings *settings;
    SongDatabase *db;
};

#endif // SETTINGSDIALOG_H
