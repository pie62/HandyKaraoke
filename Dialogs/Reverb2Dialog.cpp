#include "Reverb2Dialog.h"
#include "ui_Reverb2Dialog.h"

Reverb2Dialog::Reverb2Dialog(QWidget *parent, QList<Reverb2FX *> reverbs) :
    QDialog(parent),
    ui(new Ui::Reverb2Dialog)
{
    ui->setupUi(this);

    this->reverbs = reverbs;

    auto rv = reverbs[0];

    if (rv != nullptr)
    {
        if (rv->isOn())
            ui->btnSwitch->on();

        ui->spinDry->setValue(rv->dryMix());
        ui->spinWet->setValue(rv->wetMix());
        ui->spinRoom->setValue(rv->roomSize());
        ui->spinDamp->setValue(rv->damp());
        ui->spinWidth->setValue(rv->width());
    }

    connect(ui->btnSwitch, SIGNAL(userSwitchChanged(bool)), this, SLOT(onSwitchChanged(bool)));
}

Reverb2Dialog::~Reverb2Dialog()
{
    delete ui;
}

bool Reverb2Dialog::openned = false;

bool Reverb2Dialog::isOpenned()
{
    return openned;
}

void Reverb2Dialog::showEvent(QShowEvent *)
{
    openned = true;
}

void Reverb2Dialog::closeEvent(QCloseEvent *)
{
    openned = false;
}

void Reverb2Dialog::onSwitchChanged(bool sw)
{
    for (auto rv : reverbs)
    {
        if (sw)
            rv->on();
        else
            rv->off();
    }

    emit switchChanged(sw);
}

void Reverb2Dialog::on_btnReset_clicked()
{
    ui->spinDry->setValue(0.5f);
    ui->spinWet->setValue(2.0f);
    ui->spinRoom->setValue(0.5f);
    ui->spinDamp->setValue(0.5f);
    ui->spinWidth->setValue(1.0f);
}

// ----------------------------------------------------------------

void Reverb2Dialog::on_dialDry_valueChanged(int value)
{
    ui->spinDry->setValue(value / 100.0f);
}

void Reverb2Dialog::on_spinDry_valueChanged(double arg1)
{
    ui->dialDry->setValue(arg1 * 100);

    for (auto rv : reverbs) {
        rv->setDryMix(arg1);
    }
}

// ----------------------------------------------------------------

void Reverb2Dialog::on_dialWet_valueChanged(int value)
{
    ui->spinWet->setValue(value / 100.0f);
}

void Reverb2Dialog::on_spinWet_valueChanged(double arg1)
{
    ui->dialWet->setValue(arg1 * 100);

    for (auto rv : reverbs) {
        rv->setWetMix(arg1);
    }
}

// ----------------------------------------------------------------

void Reverb2Dialog::on_dialRoom_valueChanged(int value)
{
    ui->spinRoom->setValue(value / 100.0f);
}

void Reverb2Dialog::on_spinRoom_valueChanged(double arg1)
{
    ui->dialRoom->setValue(arg1 * 100);

    for (auto rv : reverbs) {
        rv->setRoomSize(arg1);
    }
}

// ----------------------------------------------------------------

void Reverb2Dialog::on_dialDamp_valueChanged(int value)
{
    ui->spinDamp->setValue(value / 100.0f);
}

void Reverb2Dialog::on_spinDamp_valueChanged(double arg1)
{
    ui->dialDamp->setValue(arg1 * 100);

    for (auto rv : reverbs) {
        rv->setDamp(arg1);
    }
}

// ----------------------------------------------------------------

void Reverb2Dialog::on_dialWidth_valueChanged(int value)
{
    ui->spinWidth->setValue(value / 100.0f);
}

void Reverb2Dialog::on_spinWidth_valueChanged(double arg1)
{
    ui->dialWidth->setValue(arg1 * 100);

    for (auto rv : reverbs) {
        rv->setWidth(arg1);
    }
}

// ----------------------------------------------------------------
