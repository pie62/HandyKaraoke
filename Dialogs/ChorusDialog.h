#ifndef CHORUSDIALOG_H
#define CHORUSDIALOG_H

#include <QDialog>

#include "BASSFX/ChorusFX.h"

namespace Ui {
class ChorusDialog;
}

class ChorusDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChorusDialog(QWidget *parent = 0, ChorusFX *chorusFX = nullptr);
    ~ChorusDialog();

signals:
    void switchChanged(bool s);

private slots:
    void connectDialAndSpin();
    void disconnectDialAndSpin();

    void onSwitchChanged(bool sw);

    void on_cbWaveform_activated(int index);
    void on_cbPhase_activated(int index);

    void setWetDryMix(int wetDryMix);
    void setDepth(int depth);
    void setFeedback(int feedback);
    void setFrequency(int frequency);
    void setDelay(int delay);

    void on_btnReset_clicked();

private:
    Ui::ChorusDialog *ui;

    ChorusFX *chorus;
};

#endif // CHORUSDIALOG_H
