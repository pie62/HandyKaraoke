#ifndef SYNTHMIXERDIALOG_H
#define SYNTHMIXERDIALOG_H

#include <QDialog>
#include <QMap>
#include <QSignalMapper>

#include "Midi/MidiPlayer.h"
#include "Widgets/InstCh.h"

class QMenu;

class MainWindow;

namespace Ui {
class SynthMixerDialog;
}

class SynthMixerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SynthMixerDialog(QWidget *parent = 0, MainWindow *mainWin = 0);//, MainWindow *mainWin = 0);
    ~SynthMixerDialog();

    void setVSTVendorMenu();
    void setVSTToSynth();

private slots:
    void setBtnEqIcon(bool s);
    void setBtnReverbIcon(bool s);
    void setBtnChorusIcon(bool s);
    void setMute(InstrumentType t, bool m);
    void setSolo(InstrumentType t, bool s);
    void setMixLevel(InstrumentType t, int level);
    void resetMixLevel(InstrumentType t);
    void showPeakVU(InstrumentType t, int bus, int ch, int note, int velocity);

    void on_btnSettingVu_clicked();
    void on_btnReset_clicked();

    void showChannelMenu(InstrumentType type, const QPoint &pos);
    void setBusGroup(int group);
    DWORD addVST(const QString &uidStr, bool bypass = false);
    void byPassVST(InstrumentType type, int fxIndex, bool bypass);
    void showVSTFxDialog(InstrumentType type, int fxIndex);
    void onVSTDialogClosing(uint32_t fx);
    void showVSTRemoveMenu(InstrumentType type, int fxIndex, const QPoint &pos);
    void removeVST();

    void on_btnBus_clicked();

    void on_btnVSTDirs_clicked();

protected:
    void showEvent(QShowEvent *);
    void hideEvent(QHideEvent *event);

private:
    Ui::SynthMixerDialog *ui;

    MainWindow *mainWin;
    MidiPlayer *player;
    MidiSynthesizer *synth;
    InstrumentType currentType;

    QMap<InstrumentType, InstCh*> chInstMap;

    QList<QMenu*> vstVendorMenus;
    QSignalMapper signalVstActionMapper;
    QSignalMapper signalBusActionMapper;

    int currentFxIndexToRemove;

    void mapChInstUI();
    void setChInstDetails();
    void createBusActions(InstrumentType t, QMenu *busMenu);
};

#endif // SYNTHMIXERDIALOG_H
