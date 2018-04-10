#ifndef CHORUSFXDIALOG_H
#define CHORUSFXDIALOG_H

#include "BASSFX/ChorusFX.h"

#include <QDialog>

namespace Ui {
class ChorusFXDialog;
}

class ChorusFXDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChorusFXDialog(QWidget *parent = 0, ChorusFX *chorusFX = nullptr);
    ~ChorusFXDialog();

private slots:
    void connectDialAndSpin();
    void disconnectDialAndSpin();

    void on_cbWaveform_activated(int index);
    void on_cbPhase_activated(int index);

    void setWetDryMix(int wetDryMix);
    void setDepth(int depth);
    void setFeedback(int feedback);
    void setFrequency(int frequency);
    void setDelay(int delay);

    void on_btnReset_clicked();

private:
    Ui::ChorusFXDialog *ui;

    ChorusFX *chorus;
};

#endif // CHORUSFXDIALOG_H
