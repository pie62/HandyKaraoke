#ifndef CHORUS2DIALOG_H
#define CHORUS2DIALOG_H

#include <QDialog>
#include "BASSFX/Chorus2FX.h"

namespace Ui {
class Chorus2Dialog;
}

class Chorus2Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Chorus2Dialog(QWidget *parent = 0, QList<Chorus2FX *> choruss = QList<Chorus2FX *>());
    ~Chorus2Dialog();

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

    void on_dialFeedback_valueChanged(int value);
    void on_spinFeedback_valueChanged(double arg1);

    void on_dialMin_valueChanged(int value);
    void on_spinMin_valueChanged(double arg1);

    void on_dialMax_valueChanged(int value);
    void on_spinMax_valueChanged(double arg1);

    void on_dialRate_valueChanged(int value);
    void on_spinRate_valueChanged(double arg1);

private:
    Ui::Chorus2Dialog *ui;

    QList<Chorus2FX *> choruss;

    static bool openned;
};

#endif // CHORUS2DIALOG_H
