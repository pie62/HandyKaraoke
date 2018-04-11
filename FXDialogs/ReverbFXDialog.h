#ifndef REVERBFXDIALOG_H
#define REVERBFXDIALOG_H

#include "BASSFX/ReverbFX.h"

#include <QDialog>

namespace Ui {
class ReverbFXDialog;
}

class ReverbFXDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ReverbFXDialog(QWidget *parent = 0, ReverbFX *reverbFX = nullptr);
    ~ReverbFXDialog();

private slots:
    void connectAll();
    void disconnectAll();

    void onDialHFValueChanged(int value);
    void onSpinHFValueChanged(double value);

    void setInGain(int ing);
    void setReverbMix(int m);
    void setReverbTime(int t);
    void setHighFreqRTRatio(float hf);

    void on_btnReset_clicked();

private:
    Ui::ReverbFXDialog *ui;

    ReverbFX *rv;
};

#endif // REVERBFXDIALOG_H
