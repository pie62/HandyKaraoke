#ifndef AUTOWAHFXDIALOG_H
#define AUTOWAHFXDIALOG_H

#include "BASSFX/AutoWahFX.h"

#include <QDialog>

namespace Ui {
class AutoWahFXDialog;
}

class AutoWahFXDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AutoWahFXDialog(QWidget *parent = 0, AutoWahFX *fx = 0);
    ~AutoWahFXDialog();

private slots:
    void connectAll();
    void disconnectAll();

    void onDialDryMixValueChanged(int value);
    void onDialWetMixValueChanged(int value);
    void onDialFeedbackValueChanged(int value);
    void onDialRateValueChanged(int value);
    void onDialRangeValueChanged(int value);
    void onDialFrequencyValueChanged(int value);

    void onSpinDryMixValueChanged(double arg1);
    void onSpinWetMixValueChanged(double arg1);
    void onSpinFeedbackValueChanged(double arg1);
    void onSpinRateValueChanged(double arg1);
    void onSpinRangeValueChanged(double arg1);
    void onSpinFrequencyValueChanged(int arg1);

    void on_btnReset_clicked();

private:
    Ui::AutoWahFXDialog *ui;

    AutoWahFX *fx;
};

#endif // AUTOWAHFXDIALOG_H
