#ifndef COMPRESSORFXDIALOG_H
#define COMPRESSORFXDIALOG_H

#include "BASSFX/CompressorFX.h"
#include <QDialog>

namespace Ui {
class CompressorFXDialog;
}

class CompressorFXDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CompressorFXDialog(QWidget *parent = 0, CompressorFX *fx = nullptr);
    ~CompressorFXDialog();

private slots:
    void connectAll();
    void disconnectAll();

    void dialGainValueChanged(int value);
    void dialThresholdValueChanged(int value);
    void dialRatioValueChanged(int value);
    void dialAttackValueChanged(int value);
    void dialReleaseValueChanged(int value);

    void setGain(int gain);
    void setThreshold(int threshold);
    void setRatio(int ratio);
    void setAttack(int attack);
    void setRelease(int release);

    void on_btnReset_clicked();

private:
    Ui::CompressorFXDialog *ui;

    CompressorFX *fx;
};

#endif // COMPRESSORFXDIALOG_H
