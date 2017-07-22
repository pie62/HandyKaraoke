#include "ReverbDialog.h"
#include "ui_ReverbDialog.h"

ReverbDialog::ReverbDialog(QWidget *parent, ReverbFX *reverbFX) :
    QDialog(parent),
    ui(new Ui::ReverbDialog)
{
    ui->setupUi(this);

    rv = reverbFX;

    if (rv != nullptr)
    {
        if (rv->isOn())
            ui->btnSwitch->on();

        ui->dialInGain->setValue(rv->inGain());
        ui->spinInGain->setValue(rv->inGain());

        ui->dialRvMix->setValue(rv->reverbMix());
        ui->spinRvMix->setValue(rv->reverbMix());

        ui->dialRvTime->setValue(rv->reverbTime());
        ui->spinRvTime->setValue(rv->reverbTime());

        ui->dialHF->setValue(rv->highFreqRTRatio() * 1000);
        ui->spinHF->setValue(rv->highFreqRTRatio());
    }

    connect(ui->btnSwitch, SIGNAL(userSwitchChanged(bool)), this, SLOT(onSwitchChanged(bool)));
    connectAll();
}

ReverbDialog::~ReverbDialog()
{
    delete ui;
}

void ReverbDialog::onSwitchChanged(bool sw)
{
    if (rv == nullptr)
        return;

    if (sw)
        rv->on();
    else
        rv->off();

    emit switchChanged(sw);
}

void ReverbDialog::connectAll()
{
    connect(ui->dialInGain, SIGNAL(valueChanged(int)), this, SLOT(setInGain(int)));
    connect(ui->spinInGain, SIGNAL(valueChanged(int)), this, SLOT(setInGain(int)));

    connect(ui->dialRvMix, SIGNAL(valueChanged(int)), this, SLOT(setReverbMix(int)));
    connect(ui->spinRvMix, SIGNAL(valueChanged(int)), this, SLOT(setReverbMix(int)));

    connect(ui->dialRvTime, SIGNAL(valueChanged(int)), this, SLOT(setReverbTime(int)));
    connect(ui->spinRvTime, SIGNAL(valueChanged(int)), this, SLOT(setReverbTime(int)));

    connect(ui->dialHF, SIGNAL(valueChanged(int)), this, SLOT(onDialHFValueChanged(int)));
    connect(ui->spinHF, SIGNAL(valueChanged(double)), this, SLOT(onSpinHFValueChanged(double)));
}

void ReverbDialog::disconnectAll()
{
    disconnect(ui->dialInGain, SIGNAL(valueChanged(int)), this, SLOT(setInGain(int)));
    disconnect(ui->spinInGain, SIGNAL(valueChanged(int)), this, SLOT(setInGain(int)));

    disconnect(ui->dialRvMix, SIGNAL(valueChanged(int)), this, SLOT(setReverbMix(int)));
    disconnect(ui->spinRvMix, SIGNAL(valueChanged(int)), this, SLOT(setReverbMix(int)));

    disconnect(ui->dialRvTime, SIGNAL(valueChanged(int)), this, SLOT(setReverbTime(int)));
    disconnect(ui->spinRvTime, SIGNAL(valueChanged(int)), this, SLOT(setReverbTime(int)));

    disconnect(ui->dialHF, SIGNAL(valueChanged(int)), this, SLOT(onDialHFValueChanged(int)));
    disconnect(ui->spinHF, SIGNAL(valueChanged(double)), this, SLOT(onSpinHFValueChanged(double)));
}

void ReverbDialog::onDialHFValueChanged(int value)
{
    setHighFreqRTRatio(value / 1000.0f);
}

void ReverbDialog::onSpinHFValueChanged(double value)
{
    setHighFreqRTRatio((float)value);
}

void ReverbDialog::setInGain(int ing)
{
    disconnectAll();

    ui->dialInGain->setValue(ing);
    ui->spinInGain->setValue(ing);

    if (rv != nullptr)
        rv->setInGain((float)ing);

    connectAll();
}

void ReverbDialog::setReverbMix(int m)
{
    disconnectAll();

    ui->dialRvMix->setValue(m);
    ui->spinRvMix->setValue(m);

    if (rv != nullptr)
        rv->setReverbMix((float)m);

    connectAll();
}

void ReverbDialog::setReverbTime(int t)
{
    disconnectAll();

    ui->dialRvTime->setValue(t);
    ui->spinRvTime->setValue(t);

    if (rv != nullptr)
        rv->setReverbTime((float)t);

    connectAll();
}

void ReverbDialog::setHighFreqRTRatio(float hf)
{
    disconnectAll();

    ui->dialHF->setValue(hf*1000);
    ui->spinHF->setValue(hf);

    if (rv != nullptr)
        rv->setHighFreqRTRatio(hf);

    connectAll();
}

void ReverbDialog::on_btnReset_clicked()
{
    setInGain(0);
    setReverbMix(0);
    setReverbTime(1000);
    setHighFreqRTRatio(0.001);
}
