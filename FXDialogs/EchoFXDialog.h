#ifndef ECHOFXDIALOG_H
#define ECHOFXDIALOG_H

#include "BASSFX/EchoFX.h"

#include <QDialog>

namespace Ui {
class EchoFXDialog;
}

class EchoFXDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EchoFXDialog(QWidget *parent = 0, EchoFX *fx = nullptr);
    ~EchoFXDialog();

private slots:
    void connectAll();
    void disconnectAll();

    void dialWetDryMixValueChanged(int value);
    void dialFeedbackValueChanged(int value);
    void dialLeftDelayValueChanged(int value);
    void dialRightDelayValueChanged(int value);

    void setWetDryMix(int value);
    void setFeedback(int value);
    void setLeftDelay(int value);
    void setRightDelay(int value);
    void setPanDelay(bool pan);

    void on_btnReset_clicked();

private:
    Ui::EchoFXDialog *ui;

    EchoFX *fx;
};

#endif // ECHOFXDIALOG_H
