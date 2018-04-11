#include "EchoFXDialog.h"
#include "ui_EchoFXDialog.h"

EchoFXDialog::EchoFXDialog(QWidget *parent, EchoFX *fx) :
    QDialog(parent),
    ui(new Ui::EchoFXDialog)
{
    ui->setupUi(this);

    this->fx = fx;

    ui->dialWetDryMix->setValue(fx->wetDryMix());
    ui->dialFeedback->setValue(fx->feedback());
    ui->dialLeftDelay->setValue(fx->leftDelay());
    ui->dialRightDelay->setValue(fx->rightDelay());

    ui->spinWetDryMix->setValue(fx->wetDryMix());
    ui->spinFeedback->setValue(fx->feedback());
    ui->spinLeftDelay->setValue(fx->leftDelay());
    ui->spinRightDelay->setValue(fx->rightDelay());

    if (fx->isPanDelay())
        ui->switchPan->on();
    else
        ui->switchPan->off();

    connectAll();
    connect(ui->switchPan, SIGNAL(switchChanged(bool)), this, SLOT(setPanDelay(bool)));
}

EchoFXDialog::~EchoFXDialog()
{
    delete ui;
}

void EchoFXDialog::connectAll()
{
    connect(ui->dialWetDryMix, SIGNAL(valueChanged(int)), this, SLOT(dialWetDryMixValueChanged(int)));
    connect(ui->dialFeedback, SIGNAL(valueChanged(int)), this, SLOT(dialFeedbackValueChanged(int)));
    connect(ui->dialLeftDelay, SIGNAL(valueChanged(int)), this,SLOT(dialLeftDelayValueChanged(int)));
    connect(ui->dialRightDelay, SIGNAL(valueChanged(int)), this, SLOT(dialRightDelayValueChanged(int)));

    connect(ui->spinWetDryMix, SIGNAL(valueChanged(int)), this, SLOT(setWetDryMix(int)));
    connect(ui->spinFeedback, SIGNAL(valueChanged(int)), this, SLOT(setFeedback(int)));
    connect(ui->spinLeftDelay, SIGNAL(valueChanged(int)), this,SLOT(setLeftDelay(int)));
    connect(ui->spinRightDelay, SIGNAL(valueChanged(int)), this, SLOT(setRightDelay(int)));
}

void EchoFXDialog::disconnectAll()
{
    disconnect(ui->dialWetDryMix, SIGNAL(valueChanged(int)), this, SLOT(dialWetDryMixValueChanged(int)));
    disconnect(ui->dialFeedback, SIGNAL(valueChanged(int)), this, SLOT(dialFeedbackValueChanged(int)));
    disconnect(ui->dialLeftDelay, SIGNAL(valueChanged(int)), this,SLOT(dialLeftDelayValueChanged(int)));
    disconnect(ui->dialRightDelay, SIGNAL(valueChanged(int)), this, SLOT(dialRightDelayValueChanged(int)));

    disconnect(ui->spinWetDryMix, SIGNAL(valueChanged(int)), this, SLOT(setWetDryMix(int)));
    disconnect(ui->spinFeedback, SIGNAL(valueChanged(int)), this, SLOT(setFeedback(int)));
    disconnect(ui->spinLeftDelay, SIGNAL(valueChanged(int)), this,SLOT(setLeftDelay(int)));
    disconnect(ui->spinRightDelay, SIGNAL(valueChanged(int)), this, SLOT(setRightDelay(int)));
}

void EchoFXDialog::dialWetDryMixValueChanged(int value)
{
    ui->spinWetDryMix->setValue(value);
}

void EchoFXDialog::dialFeedbackValueChanged(int value)
{
    ui->spinFeedback->setValue(value);
}

void EchoFXDialog::dialLeftDelayValueChanged(int value)
{
    ui->spinLeftDelay->setValue(value);
}

void EchoFXDialog::dialRightDelayValueChanged(int value)
{
    ui->spinRightDelay->setValue(value);
}

void EchoFXDialog::setWetDryMix(int value)
{
    disconnectAll();

    fx->setWetDryMix(value);

    ui->dialWetDryMix->setValue(value);

    connectAll();
}

void EchoFXDialog::setFeedback(int value)
{
    disconnectAll();

    fx->setFeedback(value);

    ui->dialFeedback->setValue(value);

    connectAll();
}

void EchoFXDialog::setLeftDelay(int value)
{
    disconnectAll();

    fx->setLeftDelay(value);

    ui->dialLeftDelay->setValue(value);

    connectAll();
}

void EchoFXDialog::setRightDelay(int value)
{
    disconnectAll();

    fx->setRightDelay(value);

    ui->dialRightDelay->setValue(value);

    connectAll();
}

void EchoFXDialog::setPanDelay(bool pan)
{
    fx->setPanDelay(pan);
}

void EchoFXDialog::on_btnReset_clicked()
{
    ui->spinWetDryMix->setValue(50);
    ui->spinFeedback->setValue(50);
    ui->spinLeftDelay->setValue(500);
    ui->spinRightDelay->setValue(500);

    ui->switchPan->off();
}
