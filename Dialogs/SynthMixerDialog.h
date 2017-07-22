#ifndef SYNTHMIXERDIALOG_H
#define SYNTHMIXERDIALOG_H

#include <QDialog>
#include <QMap>

#include "Midi/MidiPlayer.h"
#include "Widgets/InstCh.h"

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

private slots:
    void setBtnEqIcon(bool s);
    void setBtnReverbIcon(bool s);
    void setBtnChorusIcon(bool s);
    void setMute(InstrumentType t, bool m);
    void setSolo(InstrumentType t, bool s);
    void setMixLevel(InstrumentType t, int level);
    void resetMixLevel(InstrumentType t);
    void onPlayerPlayingEvents(MidiEvent *e);

    void on_btnSettingVu_clicked();

    void on_btnReset_clicked();

protected:
    void showEvent(QShowEvent *);
    void hideEvent(QHideEvent *event);

private:
    Ui::SynthMixerDialog *ui;

    MainWindow *mainWin;
    MidiPlayer *player;
    MidiSynthesizer *synth;

    QMap<InstrumentType, InstCh*> chInstMap;


    void mapChInstUI();
    void setChInstDetails();
};

#endif // SYNTHMIXERDIALOG_H
