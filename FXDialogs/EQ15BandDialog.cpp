#include "EQ15BandDialog.h"
#include "ui_EQ15BandDialog.h"

EQ15BandDialog::EQ15BandDialog(QWidget *parent, Equalizer15BandFX *fx) :
    QDialog(parent),
    ui(new Ui::EQ15BandDialog)
{
    ui->setupUi(this);

    this->fx = fx;

    signalLevelMapper = new QSignalMapper(this);
    signalMouseDoubleClick = new QSignalMapper(this);

    this->mapLabels();
    this->mapSliders();

    for (EQFrequency15Range freq : lbdbMap.keys())
    {
        int i = static_cast<int>(freq);
        Slider* slider = sliderMap[freq];

        slider->setMaximumLevel(150);
        slider->setMinimumLevel(-150);
        slider->setChangeStep(10);
        slider->setTickCount(31);

        float gain = fx->gain()[freq];

        lbdbMap[freq]->setText(QString::number(gain, 'f', 1));
        slider->setLevel(gain * 10);

        connect(slider, SIGNAL(userLevelChanged(int)), signalLevelMapper, SLOT(map()));
        signalLevelMapper->setMapping(slider, i);

        connect(slider, SIGNAL(mouseDoubleClicked()), signalMouseDoubleClick, SLOT(map()));
        signalMouseDoubleClick->setMapping(slider, i);
    }

    connect(signalLevelMapper, SIGNAL(mapped(int)), this, SLOT(setGian(int)));
    connect(signalMouseDoubleClick, SIGNAL(mapped(int)), this, SLOT(resetGian(int)));
}

EQ15BandDialog::~EQ15BandDialog()
{
    delete signalLevelMapper;
    delete signalMouseDoubleClick;

    delete ui;
}

void EQ15BandDialog::setGian(int sliderIndex)
{
    EQFrequency15Range freq = sliderMap.keys()[sliderIndex];
    Slider *slider = sliderMap.values()[sliderIndex];

    fx->setGain(freq, slider->level() / 10.0f);
    lbdbMap[freq]->setText(QString::number(slider->level() / 10.0f, 'f', 1));
}

void EQ15BandDialog::resetGian(int sliderIndex)
{
    EQFrequency15Range freq = sliderMap.keys()[sliderIndex];

    fx->setGain(freq, 0.0f);

    sliderMap[freq]->setLevel(0);
    lbdbMap[freq]->setText(QString::number(0.0f, 'f', 1));
}

void EQ15BandDialog::on_btnReset_clicked()
{
    for (int i=0; i<15; i++)
    {
        resetGian(i);
    }
}

void EQ15BandDialog::mapLabels()
{
    lbdbMap[EQFrequency15Range::Frequency25Hz]      = ui->lbdb_1;
    lbdbMap[EQFrequency15Range::Frequency40Hz]      = ui->lbdb_2;
    lbdbMap[EQFrequency15Range::Frequency63Hz]      = ui->lbdb_3;
    lbdbMap[EQFrequency15Range::Frequency100Hz]     = ui->lbdb_4;
    lbdbMap[EQFrequency15Range::Frequency160Hz]     = ui->lbdb_5;
    lbdbMap[EQFrequency15Range::Frequency250Hz]     = ui->lbdb_6;
    lbdbMap[EQFrequency15Range::Frequency400Hz]     = ui->lbdb_7;
    lbdbMap[EQFrequency15Range::Frequency630Hz]     = ui->lbdb_8;
    lbdbMap[EQFrequency15Range::Frequency1000Hz]    = ui->lbdb_9;
    lbdbMap[EQFrequency15Range::Frequency1600Hz]    = ui->lbdb_10;
    lbdbMap[EQFrequency15Range::Frequency2500Hz]    = ui->lbdb_11;
    lbdbMap[EQFrequency15Range::Frequency4000Hz]    = ui->lbdb_12;
    lbdbMap[EQFrequency15Range::Frequency6300Hz]    = ui->lbdb_13;
    lbdbMap[EQFrequency15Range::Frequency10000Hz]   = ui->lbdb_14;
    lbdbMap[EQFrequency15Range::Frequency16000Hz]   = ui->lbdb_15;
}

void EQ15BandDialog::mapSliders()
{
    sliderMap[EQFrequency15Range::Frequency25Hz]    = ui->slider_1;
    sliderMap[EQFrequency15Range::Frequency40Hz]    = ui->slider_2;
    sliderMap[EQFrequency15Range::Frequency63Hz]    = ui->slider_3;
    sliderMap[EQFrequency15Range::Frequency100Hz]   = ui->slider_4;
    sliderMap[EQFrequency15Range::Frequency160Hz]   = ui->slider_5;
    sliderMap[EQFrequency15Range::Frequency250Hz]   = ui->slider_6;
    sliderMap[EQFrequency15Range::Frequency400Hz]   = ui->slider_7;
    sliderMap[EQFrequency15Range::Frequency630Hz]   = ui->slider_8;
    sliderMap[EQFrequency15Range::Frequency1000Hz]  = ui->slider_9;
    sliderMap[EQFrequency15Range::Frequency1600Hz]  = ui->slider_10;
    sliderMap[EQFrequency15Range::Frequency2500Hz]  = ui->slider_11;
    sliderMap[EQFrequency15Range::Frequency4000Hz]  = ui->slider_12;
    sliderMap[EQFrequency15Range::Frequency6300Hz]  = ui->slider_13;
    sliderMap[EQFrequency15Range::Frequency10000Hz] = ui->slider_14;
    sliderMap[EQFrequency15Range::Frequency16000Hz] = ui->slider_15;
}
