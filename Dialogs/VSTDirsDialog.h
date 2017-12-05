#ifndef VSTDIRSDIALOG_H
#define VSTDIRSDIALOG_H

#include "Midi/MidiSynthesizer.h"
#include "SynthMixerDialog.h"
#include <QDialog>

namespace Ui {
class VSTDirsDialog;
}

class VSTDirsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit VSTDirsDialog(QWidget *parent = 0, SynthMixerDialog *mixDlg = nullptr,
                           MidiSynthesizer *synth = nullptr);
    ~VSTDirsDialog();

private slots:
    void on_btnClose_clicked();

    void on_btnAdd_clicked();

    void on_btnRemove_clicked();

    void on_btnUpdate_clicked();

private:
    Ui::VSTDirsDialog *ui;
    SynthMixerDialog *mixDlg;
    MidiSynthesizer *synth;
};

#endif // VSTDIRSDIALOG_H
