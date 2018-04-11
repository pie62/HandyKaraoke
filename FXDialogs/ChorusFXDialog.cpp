#include "ChorusFXDialog.h"
#include "ui_ChorusFXDialog.h"

ChorusFXDialog::ChorusFXDialog(QWidget *parent, ChorusFX *chorusFX) :
    QDialog(parent),
    ui(new Ui::ChorusFXDialog)
{
    ui->setupUi(this);

    chorus = chorusFX;

    if (chorus != nullptr)
    {
        ui->dialWetDryMix->setValue((int)chorus->wetDryMix());
        ui->spinWetDryMix->setValue((int)chorus->wetDryMix());

        ui->dialDepth->setValue((int)chorus->depth());
        ui->spinDepth->setValue((int)chorus->depth());

        ui->dialFeedback->setValue((int)chorus->feedback());
        ui->spinFeedback->setValue((int)chorus->feedback());

        ui->dialFrequency->setValue((int)chorus->frequency());
        ui->spinFrequency->setValue((int)chorus->frequency());

        ui->dialDelay->setValue((int)chorus->delay());
        ui->spinDelay->setValue((int)chorus->delay());

        if (chorus->waveform() == WaveformType::Triangle)
            ui->cbWaveform->setCurrentIndex(0);
        else
            ui->cbWaveform->setCurrentIndex(1);

        int i = 0;
        switch (chorus->phase()) {
        case PhaseType::PhaseNeg180: i = 0; break;
        case PhaseType::PhaseNeg90:  i = 1; break;
        case PhaseType::Phase0:      i = 2; break;
        case PhaseType::Phase90:     i = 3; break;
        case PhaseType::Phase180:    i = 4; break;
        }

        ui->cbPhase->setCurrentIndex(i);
    }

    connectDialAndSpin();
}

ChorusFXDialog::~ChorusFXDialog()
{
    delete ui;
}

void ChorusFXDialog::connectDialAndSpin()
{
    connect(ui->dialWetDryMix, SIGNAL(valueChanged(int)), this, SLOT(setWetDryMix(int)));
    connect(ui->spinWetDryMix, SIGNAL(valueChanged(int)), this, SLOT(setWetDryMix(int)));

    connect(ui->dialDepth, SIGNAL(valueChanged(int)), this, SLOT(setDepth(int)));
    connect(ui->spinDepth, SIGNAL(valueChanged(int)), this, SLOT(setDepth(int)));

    connect(ui->dialFeedback, SIGNAL(valueChanged(int)), this, SLOT(setFeedback(int)));
    connect(ui->spinFeedback, SIGNAL(valueChanged(int)), this, SLOT(setFeedback(int)));

    connect(ui->dialFrequency, SIGNAL(valueChanged(int)), this, SLOT(setFrequency(int)));
    connect(ui->spinFrequency, SIGNAL(valueChanged(int)), this, SLOT(setFrequency(int)));

    connect(ui->dialDelay, SIGNAL(valueChanged(int)), this, SLOT(setDelay(int)));
    connect(ui->spinDelay, SIGNAL(valueChanged(int)), this, SLOT(setDelay(int)));
}

void ChorusFXDialog::disconnectDialAndSpin()
{
    disconnect(ui->dialWetDryMix, SIGNAL(valueChanged(int)), this, SLOT(setWetDryMix(int)));
    disconnect(ui->spinWetDryMix, SIGNAL(valueChanged(int)), this, SLOT(setWetDryMix(int)));

    disconnect(ui->dialDepth, SIGNAL(valueChanged(int)), this, SLOT(setDepth(int)));
    disconnect(ui->spinDepth, SIGNAL(valueChanged(int)), this, SLOT(setDepth(int)));

    disconnect(ui->dialFeedback, SIGNAL(valueChanged(int)), this, SLOT(setFeedback(int)));
    disconnect(ui->spinFeedback, SIGNAL(valueChanged(int)), this, SLOT(setFeedback(int)));

    disconnect(ui->dialFrequency, SIGNAL(valueChanged(int)), this, SLOT(setFrequency(int)));
    disconnect(ui->spinFrequency, SIGNAL(valueChanged(int)), this, SLOT(setFrequency(int)));

    disconnect(ui->dialDelay, SIGNAL(valueChanged(int)), this, SLOT(setDelay(int)));
    disconnect(ui->spinDelay, SIGNAL(valueChanged(int)), this, SLOT(setDelay(int)));
}

void ChorusFXDialog::on_cbWaveform_activated(int index)
{
    if (index == 0)
        chorus->setWaveform(WaveformType::Triangle);
    else
        chorus->setWaveform(WaveformType::Sine);
}

void ChorusFXDialog::on_cbPhase_activated(int index)
{
    PhaseType pt = PhaseType::Phase90;

    switch (index) {
    case 0: pt = PhaseType::PhaseNeg180; break;
    case 1: pt = PhaseType::PhaseNeg90; break;
    case 2: pt = PhaseType::Phase0; break;
    case 3: pt = PhaseType::Phase90; break;
    case 4: pt = PhaseType::Phase180; break;
    }

    chorus->setPhase(pt);
}

void ChorusFXDialog::setWetDryMix(int wetDryMix)
{
    disconnectDialAndSpin();

    ui->dialWetDryMix->setValue(wetDryMix);
    ui->spinWetDryMix->setValue(wetDryMix);

    if (chorus != nullptr)
        chorus->setWetDryMix(wetDryMix);

    connectDialAndSpin();
}

void ChorusFXDialog::setDepth(int depth)
{
    disconnectDialAndSpin();

    ui->dialDepth->setValue(depth);
    ui->spinDepth->setValue(depth);

    if (chorus != nullptr)
        chorus->setDepth(depth);

    connectDialAndSpin();
}

void ChorusFXDialog::setFeedback(int feedback)
{
    disconnectDialAndSpin();

    ui->dialFeedback->setValue(feedback);
    ui->spinFeedback->setValue(feedback);

    if (chorus != nullptr)
        chorus->setFeedback(feedback);

    connectDialAndSpin();
}

void ChorusFXDialog::setFrequency(int frequency)
{
    disconnectDialAndSpin();

    ui->dialFrequency->setValue(frequency);
    ui->spinFrequency->setValue(frequency);

    if (chorus != nullptr)
        chorus->setFrequency(frequency);

    connectDialAndSpin();
}

void ChorusFXDialog::setDelay(int delay)
{
    disconnectDialAndSpin();

    ui->dialDelay->setValue(delay);
    ui->spinDelay->setValue(delay);

    if (chorus != nullptr)
        chorus->setDelay(delay);

    connectDialAndSpin();
}

void ChorusFXDialog::on_btnReset_clicked()
{
    ui->cbWaveform->setCurrentIndex(1);
    ui->cbPhase->setCurrentIndex(3);

    if (chorus != nullptr)
    {
        chorus->setWaveform(WaveformType::Sine);
        chorus->setPhase(PhaseType::Phase90);
    }

    setWetDryMix(50);
    setDepth(10);
    setFeedback(25);
    setFrequency(1);
    setDelay(16);
}
