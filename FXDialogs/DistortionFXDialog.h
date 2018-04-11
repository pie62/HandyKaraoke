#ifndef DISTORTIONFXDIALOG_H
#define DISTORTIONFXDIALOG_H

#include "BASSFX/DistortionFX.h"

#include <QDialog>

namespace Ui {
class DistortionFXDialog;
}

class DistortionFXDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DistortionFXDialog(QWidget *parent = 0, DistortionFX *fx = nullptr);
    ~DistortionFXDialog();

private slots:
    void connectAll();
    void disconnectAll();

    void dialGainValueChanged(int value);
    void dialEdgeValueChanged(int value);
    void dialCFValueChanged(int value);
    void dialFWValueChanged(int value);
    void dialLowValueChanged(int value);

    void setGain(int value);
    void setEdge(int value);
    void setCF(int value);
    void setFW(int value);
    void setLow(int value);

    void on_btnReset_clicked();

private:
    Ui::DistortionFXDialog *ui;

    DistortionFX *fx;
};

#endif // DISTORTIONFXDIALOG_H
