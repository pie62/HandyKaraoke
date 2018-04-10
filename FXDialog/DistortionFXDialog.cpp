#include "DistortionFXDialog.h"
#include "ui_DistortionFXDialog.h"

DistortionFXDialog::DistortionFXDialog(QWidget *parent, DistortionFX *fx) :
    QDialog(parent),
    ui(new Ui::DistortionFXDialog)
{
    ui->setupUi(this);

    this->fx = fx;

    ui->dialGain->setValue(fx->gain());
    ui->dialEdge->setValue(fx->edge());
    ui->dialCF->setValue(fx->postEQCenterFrequency());
    ui->dialFW->setValue(fx->postEQBandwidth());
    ui->dialLow->setValue(fx->preLowpassCutoff());

    ui->spinGain->setValue(fx->gain());
    ui->spinEdge->setValue(fx->edge());
    ui->spinCF->setValue(fx->postEQCenterFrequency());
    ui->spinFW->setValue(fx->postEQBandwidth());
    ui->spinLow->setValue(fx->preLowpassCutoff());

    connectAll();
}

DistortionFXDialog::~DistortionFXDialog()
{
    delete ui;
}

void DistortionFXDialog::connectAll()
{
    connect(ui->dialGain, SIGNAL(valueChanged(int)), this, SLOT(dialGainValueChanged(int)));
    connect(ui->dialEdge, SIGNAL(valueChanged(int)), this, SLOT(dialEdgeValueChanged(int)));
    connect(ui->dialCF, SIGNAL(valueChanged(int)), this, SLOT(dialCFValueChanged(int)));
    connect(ui->dialFW, SIGNAL(valueChanged(int)), this, SLOT(dialFWValueChanged(int)));
    connect(ui->dialLow, SIGNAL(valueChanged(int)), this, SLOT(dialLowValueChanged(int)));

    connect(ui->spinGain, SIGNAL(valueChanged(int)), this, SLOT(setGain(int)));
    connect(ui->spinEdge, SIGNAL(valueChanged(int)), this, SLOT(setEdge(int)));
    connect(ui->spinCF, SIGNAL(valueChanged(int)), this, SLOT(setCF(int)));
    connect(ui->spinFW, SIGNAL(valueChanged(int)), this, SLOT(setFW(int)));
    connect(ui->spinLow, SIGNAL(valueChanged(int)), this, SLOT(setLow(int)));

}

void DistortionFXDialog::disconnectAll()
{
    disconnect(ui->dialGain, SIGNAL(valueChanged(int)), this, SLOT(dialGainValueChanged(int)));
    disconnect(ui->dialEdge, SIGNAL(valueChanged(int)), this, SLOT(dialEdgeValueChanged(int)));
    disconnect(ui->dialCF, SIGNAL(valueChanged(int)), this, SLOT(dialCFValueChanged(int)));
    disconnect(ui->dialFW, SIGNAL(valueChanged(int)), this, SLOT(dialFWValueChanged(int)));
    disconnect(ui->dialLow, SIGNAL(valueChanged(int)), this, SLOT(dialLowValueChanged(int)));

    disconnect(ui->spinGain, SIGNAL(valueChanged(int)), this, SLOT(setGain(int)));
    disconnect(ui->spinEdge, SIGNAL(valueChanged(int)), this, SLOT(setEdge(int)));
    disconnect(ui->spinCF, SIGNAL(valueChanged(int)), this, SLOT(setCF(int)));
    disconnect(ui->spinFW, SIGNAL(valueChanged(int)), this, SLOT(setFW(int)));
    disconnect(ui->spinLow, SIGNAL(valueChanged(int)), this, SLOT(setLow(int)));
}

void DistortionFXDialog::dialGainValueChanged(int value)
{
    ui->spinGain->setValue(value);
}

void DistortionFXDialog::dialEdgeValueChanged(int value)
{
    ui->spinEdge->setValue(value);
}

void DistortionFXDialog::dialCFValueChanged(int value)
{
    ui->spinCF->setValue(value);
}

void DistortionFXDialog::dialFWValueChanged(int value)
{
    ui->spinFW->setValue(value);
}

void DistortionFXDialog::dialLowValueChanged(int value)
{
    ui->spinLow->setValue(value);
}

void DistortionFXDialog::setGain(int value)
{
    disconnectAll();

    fx->setGain(value);

    ui->dialGain->setValue(value);

    connectAll();
}

void DistortionFXDialog::setEdge(int value)
{
    disconnectAll();

    fx->setEdge(value);

    ui->dialEdge->setValue(value);

    connectAll();
}

void DistortionFXDialog::setCF(int value)
{
    disconnectAll();

    fx->setPostEQCenterFrequency(value);

    ui->dialCF->setValue(value);

    connectAll();
}

void DistortionFXDialog::setFW(int value)
{
    disconnectAll();

    fx->setPostEQBandwidth(value);

    ui->dialFW->setValue(value);

    connectAll();
}

void DistortionFXDialog::setLow(int value)
{
    disconnectAll();

    fx->setPreLowpassCutoff(value);

    ui->dialLow->setValue(value);

    connectAll();
}

void DistortionFXDialog::on_btnReset_clicked()
{
    ui->spinGain->setValue(-18);
    ui->spinEdge->setValue(15);
    ui->spinCF->setValue(2400);
    ui->spinFW->setValue(2400);
    ui->spinLow->setValue(8000);
}
