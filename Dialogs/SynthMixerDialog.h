#ifndef SYNTHMIXERDIALOG_H
#define SYNTHMIXERDIALOG_H

#include <QDialog>
#include <QMap>
#include <QSignalMapper>
#include <QButtonGroup>

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

    #ifndef __linux__
    void setVSTVendorMenu();
    #endif
    void setFXToSynth();
    InstCh* mixChannel(InstrumentType t);
    QMap<InstrumentType, InstCh *> mixChannelMap();
    QMap<InstrumentType, InstCh *> *mixChannelMapPtr();

public slots:
    void setSoundfontPresets(int presets);

private slots:
    void showEqDialog();
    void showReverbDialog();
    void showChorusDialog();

    void setMute(InstrumentType t, bool m);
    void setSolo(InstrumentType t, bool s);
    void setMixLevel(InstrumentType t, int level);
    void resetMixLevel(InstrumentType t);
    void showPeakVU(InstrumentType t, int bus, int ch, int note, int velocity);

    void showChannelMenu(InstrumentType type, const QPoint &pos);
    void setBusGroup(int group);
    FX* addFX(const QString &uidStr, bool bypass = false);
    void byPassFX(InstrumentType type, int fxIndex, bool bypass);
    void showFxDialog(InstrumentType type, int fxIndex);
    void showFXRemoveMenu(InstrumentType type, int fxIndex, const QPoint &pos);
    void showVSTiDialog(InstrumentType vstiIndexType);
    void removeFX();

    void on_btnMenu_clicked();
    void showBusDlg();
    void showSpeakersDlg();
    void showVSTDirsDlg();
    void showVuDlg();
    void resetChannel();
    void toggleWindowParent();
    void setStaysOnTop(bool stay);

    void changeSoundfontPresets(int presets);

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
    QSignalMapper *signalBFXActionMapper = nullptr;

    QButtonGroup btnPresets;

    int currentFxIndexToRemove;

    bool staysOnTop = false;

    void mapChInstUI();
    void setChInstDetails();
    void createBusActions(InstrumentType t, QMenu *busMenu);
};

#endif // SYNTHMIXERDIALOG_H
