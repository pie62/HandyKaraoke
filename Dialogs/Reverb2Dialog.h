#ifndef REVERB2DIALOG_H
#define REVERB2DIALOG_H

#include <QDialog>
#include "BASSFX/Reverb2FX.h"

namespace Ui {
class Reverb2Dialog;
}

class Reverb2Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Reverb2Dialog(QWidget *parent = 0, QList<Reverb2FX *> reverbs = QList<Reverb2FX *>());
    ~Reverb2Dialog();

    static bool isOpenned();

signals:
    void switchChanged(bool s);

protected:
    void showEvent(QShowEvent *);
    void closeEvent(QCloseEvent *);

private slots:
    void onSwitchChanged(bool sw);
    void on_btnReset_clicked();

    void on_dialDry_valueChanged(int value);
    void on_spinDry_valueChanged(double arg1);

    void on_dialWet_valueChanged(int value);
    void on_spinWet_valueChanged(double arg1);

    void on_dialRoom_valueChanged(int value);
    void on_spinRoom_valueChanged(double arg1);

    void on_dialDamp_valueChanged(int value);
    void on_spinDamp_valueChanged(double arg1);

    void on_dialWidth_valueChanged(int value);
    void on_spinWidth_valueChanged(double arg1);

private:
    Ui::Reverb2Dialog *ui;

    QList<Reverb2FX *> reverbs;

    static bool openned;
};

#endif // REVERB2DIALOG_H
