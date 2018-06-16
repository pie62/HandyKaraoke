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
    explicit ChorusDialog(QWidget *parent = 0, QList<ChorusFX *> choruss = QList<ChorusFX *>());
    ~ChorusDialog();

    static bool isOpenned();

signals:
    void switchChanged(bool s);

protected:
    void showEvent(QShowEvent *);
    void closeEvent(QCloseEvent *);

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

    QList<ChorusFX *> choruss;

    static bool openned;
};

#endif // CHORUSDIALOG_H
