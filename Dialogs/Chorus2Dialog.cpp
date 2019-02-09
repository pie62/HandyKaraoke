#include "Chorus2Dialog.h"
#include "ui_Chorus2Dialog.h"

Chorus2Dialog::Chorus2Dialog(QWidget *parent, QList<Chorus2FX *> choruss) :
    QDialog(parent),
    ui(new Ui::Chorus2Dialog)
{
    ui->setupUi(this);

    this->choruss = choruss;

    auto cr = choruss[0];

    if (cr != nullptr)
    {
        if (cr->isOn())
            ui->btnSwitch->on();

        ui->spinDry->setValue(cr->dryMix());
        ui->spinWet->setValue(cr->wetMix());
        ui->spinFeedback->setValue(cr->feedback());
        ui->spinMin->setValue(cr->minSweep());
        ui->spinMax->setValue(cr->maxSweep());
        ui->spinRate->setValue(cr->rate());
    }

    connect(ui->btnSwitch, SIGNAL(userSwitchChanged(bool)), this, SLOT(onSwitchChanged(bool)));
}

Chorus2Dialog::~Chorus2Dialog()
{
    delete ui;
}

bool Chorus2Dialog::openned = false;

bool Chorus2Dialog::isOpenned()
{
    return openned;
}

void Chorus2Dialog::showEvent(QShowEvent *)
{
    openned = true;
}

void Chorus2Dialog::closeEvent(QCloseEvent *)
{
    openned = false;
}

void Chorus2Dialog::onSwitchChanged(bool sw)
{
    for (auto chorus : choruss)
    {
        if (sw)
            chorus->on();
        else
            chorus->off();
    }

    emit switchChanged(sw);
}

void Chorus2Dialog::on_btnReset_clicked()
{
    ui->spinDry->setValue(0.9f);
    ui->spinWet->setValue(-0.2f);
    ui->spinFeedback->setValue(0.5f);
    ui->spinMin->setValue(1.0f);
    ui->spinMax->setValue(2.0f);
    ui->spinRate->setValue(10.0f);
}

// ----------------------------------------------------------------
void Chorus2Dialog::on_dialDry_valueChanged(int value)
{
    ui->spinDry->setValue(value / 100.0f);
}

void Chorus2Dialog::on_spinDry_valueChanged(double arg1)
{
    ui->dialDry->setValue(arg1 * 100);

    for (auto cr : choruss) {
        cr->setDryMix(arg1);
    }
}

// ----------------------------------------------------------------

void Chorus2Dialog::on_dialWet_valueChanged(int value)
{
    ui->spinWet->setValue(value / 100.0f);
}

void Chorus2Dialog::on_spinWet_valueChanged(double arg1)
{
    ui->dialWet->setValue(arg1 * 100);

    for (auto cr : choruss) {
        cr->setWetMix(arg1);
    }
}

// ----------------------------------------------------------------

void Chorus2Dialog::on_dialFeedback_valueChanged(int value)
{
    ui->spinFeedback->setValue(value / 100.0f);
}

void Chorus2Dialog::on_spinFeedback_valueChanged(double arg1)
{
    ui->dialFeedback->setValue(arg1 * 100);

    for (auto cr : choruss) {
        cr->setFeedback(arg1);
    }
}

// ----------------------------------------------------------------

void Chorus2Dialog::on_dialMin_valueChanged(int value)
{
    ui->spinMin->setValue(value);
}

void Chorus2Dialog::on_spinMin_valueChanged(double arg1)
{
    ui->dialMin->setValue(arg1);

    for (auto cr : choruss) {
        cr->setMinSweep(arg1);
    }
}

// ----------------------------------------------------------------

void Chorus2Dialog::on_dialMax_valueChanged(int value)
{
    ui->spinMax->setValue(value);
}

void Chorus2Dialog::on_spinMax_valueChanged(double arg1)
{
    ui->dialMax->setValue(arg1);

    for (auto cr : choruss) {
        cr->setMaxSweep(arg1);
    }
}

// ----------------------------------------------------------------
void Chorus2Dialog::on_dialRate_valueChanged(int value)
{
    ui->spinRate->setValue(value);
}

void Chorus2Dialog::on_spinRate_valueChanged(double arg1)
{
    ui->dialRate->setValue(arg1);

    for (auto cr : choruss) {
        cr->setRate(arg1);
    }
}

// ----------------------------------------------------------------








