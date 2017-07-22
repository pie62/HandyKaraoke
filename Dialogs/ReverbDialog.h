#ifndef REVERBDIALOG_H
#define REVERBDIALOG_H

#include <QDialog>

#include "BASSFX/ReverbFX.h"


namespace Ui {
class ReverbDialog;
}

class ReverbDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ReverbDialog(QWidget *parent = 0, ReverbFX *reverbFX = nullptr);
    ~ReverbDialog();

signals:
    void switchChanged(bool s);

private slots:
    void onSwitchChanged(bool sw);

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
    Ui::ReverbDialog *ui;

    ReverbFX *rv;
};

#endif // REVERBDIALOG_H
