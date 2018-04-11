#include "EQ31BandDialog.h"
#include "ui_EQ31BandDialog.h"

EQ31BandDialog::EQ31BandDialog(QWidget *parent, Equalizer31BandFX *fx) :
    QDialog(parent),
    ui(new Ui::EQ31BandDialog)
{
    ui->setupUi(this);

    this->fx = fx;

    signalLevelMapper = new QSignalMapper(this);
    signalMouseDoubleClick = new QSignalMapper(this);

    this->mapLabels();
    this->mapSliders();

    for (EQFrequency31Range freq : lbdbMap.keys())
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

EQ31BandDialog::~EQ31BandDialog()
{
    delete signalLevelMapper;
    delete signalMouseDoubleClick;

    delete ui;
}

void EQ31BandDialog::setGian(int sliderIndex)
{
    EQFrequency31Range freq = sliderMap.keys()[sliderIndex];
    Slider *slider = sliderMap.values()[sliderIndex];

    fx->setGain(freq, slider->level() / 10.0f);
    lbdbMap[freq]->setText(QString::number(slider->level() / 10.0f, 'f', 1));
}

void EQ31BandDialog::resetGian(int sliderIndex)
{
    EQFrequency31Range freq = sliderMap.keys()[sliderIndex];

    fx->setGain(freq, 0.0f);

    sliderMap[freq]->setLevel(0);
    lbdbMap[freq]->setText(QString::number(0.0f, 'f', 1));
}


void EQ31BandDialog::on_btnReset_clicked()
{
    for (int i=0; i<31; i++)
    {
        resetGian(i);
    }
}

void EQ31BandDialog::mapLabels()
{
    lbdbMap[EQFrequency31Range::Frequency20Hz]    = ui->lbdb_1;
    lbdbMap[EQFrequency31Range::Frequency25Hz]    = ui->lbdb_2;
    lbdbMap[EQFrequency31Range::Frequency31d5Hz]  = ui->lbdb_3;
    lbdbMap[EQFrequency31Range::Frequency40Hz]    = ui->lbdb_4;
    lbdbMap[EQFrequency31Range::Frequency50Hz]    = ui->lbdb_5;
    lbdbMap[EQFrequency31Range::Frequency63Hz]    = ui->lbdb_6;
    lbdbMap[EQFrequency31Range::Frequency80Hz]    = ui->lbdb_7;
    lbdbMap[EQFrequency31Range::Frequency100Hz]   = ui->lbdb_8;
    lbdbMap[EQFrequency31Range::Frequency125Hz]   = ui->lbdb_9;
    lbdbMap[EQFrequency31Range::Frequency160Hz]   = ui->lbdb_10;
    lbdbMap[EQFrequency31Range::Frequency200Hz]   = ui->lbdb_11;
    lbdbMap[EQFrequency31Range::Frequency250Hz]   = ui->lbdb_12;
    lbdbMap[EQFrequency31Range::Frequency315Hz]   = ui->lbdb_13;
    lbdbMap[EQFrequency31Range::Frequency400Hz]   = ui->lbdb_14;
    lbdbMap[EQFrequency31Range::Frequency500Hz]   = ui->lbdb_15;
    lbdbMap[EQFrequency31Range::Frequency630Hz]   = ui->lbdb_16;
    lbdbMap[EQFrequency31Range::Frequency800Hz]   = ui->lbdb_17;
    lbdbMap[EQFrequency31Range::Frequency1000Hz]  = ui->lbdb_18;
    lbdbMap[EQFrequency31Range::Frequency1250Hz]  = ui->lbdb_19;
    lbdbMap[EQFrequency31Range::Frequency1600Hz]  = ui->lbdb_20;
    lbdbMap[EQFrequency31Range::Frequency2000Hz]  = ui->lbdb_21;
    lbdbMap[EQFrequency31Range::Frequency2500Hz]  = ui->lbdb_22;
    lbdbMap[EQFrequency31Range::Frequency3150Hz]  = ui->lbdb_23;
    lbdbMap[EQFrequency31Range::Frequency4000Hz]  = ui->lbdb_24;
    lbdbMap[EQFrequency31Range::Frequency5000Hz]  = ui->lbdb_25;
    lbdbMap[EQFrequency31Range::Frequency6300Hz]  = ui->lbdb_26;
    lbdbMap[EQFrequency31Range::Frequency8000Hz]  = ui->lbdb_27;
    lbdbMap[EQFrequency31Range::Frequency10000Hz] = ui->lbdb_28;
    lbdbMap[EQFrequency31Range::Frequency12500Hz] = ui->lbdb_29;
    lbdbMap[EQFrequency31Range::Frequency16000Hz] = ui->lbdb_30;
    lbdbMap[EQFrequency31Range::Frequency20000Hz] = ui->lbdb_31;
}

void EQ31BandDialog::mapSliders()
{
    sliderMap[EQFrequency31Range::Frequency20Hz]    = ui->slider_1;
    sliderMap[EQFrequency31Range::Frequency25Hz]    = ui->slider_2;
    sliderMap[EQFrequency31Range::Frequency31d5Hz]  = ui->slider_3;
    sliderMap[EQFrequency31Range::Frequency40Hz]    = ui->slider_4;
    sliderMap[EQFrequency31Range::Frequency50Hz]    = ui->slider_5;
    sliderMap[EQFrequency31Range::Frequency63Hz]    = ui->slider_6;
    sliderMap[EQFrequency31Range::Frequency80Hz]    = ui->slider_7;
    sliderMap[EQFrequency31Range::Frequency100Hz]   = ui->slider_8;
    sliderMap[EQFrequency31Range::Frequency125Hz]   = ui->slider_9;
    sliderMap[EQFrequency31Range::Frequency160Hz]   = ui->slider_10;
    sliderMap[EQFrequency31Range::Frequency200Hz]   = ui->slider_11;
    sliderMap[EQFrequency31Range::Frequency250Hz]   = ui->slider_12;
    sliderMap[EQFrequency31Range::Frequency315Hz]   = ui->slider_13;
    sliderMap[EQFrequency31Range::Frequency400Hz]   = ui->slider_14;
    sliderMap[EQFrequency31Range::Frequency500Hz]   = ui->slider_15;
    sliderMap[EQFrequency31Range::Frequency630Hz]   = ui->slider_16;
    sliderMap[EQFrequency31Range::Frequency800Hz]   = ui->slider_17;
    sliderMap[EQFrequency31Range::Frequency1000Hz]  = ui->slider_18;
    sliderMap[EQFrequency31Range::Frequency1250Hz]  = ui->slider_19;
    sliderMap[EQFrequency31Range::Frequency1600Hz]  = ui->slider_20;
    sliderMap[EQFrequency31Range::Frequency2000Hz]  = ui->slider_21;
    sliderMap[EQFrequency31Range::Frequency2500Hz]  = ui->slider_22;
    sliderMap[EQFrequency31Range::Frequency3150Hz]  = ui->slider_23;
    sliderMap[EQFrequency31Range::Frequency4000Hz]  = ui->slider_24;
    sliderMap[EQFrequency31Range::Frequency5000Hz]  = ui->slider_25;
    sliderMap[EQFrequency31Range::Frequency6300Hz]  = ui->slider_26;
    sliderMap[EQFrequency31Range::Frequency8000Hz]  = ui->slider_27;
    sliderMap[EQFrequency31Range::Frequency10000Hz] = ui->slider_28;
    sliderMap[EQFrequency31Range::Frequency12500Hz] = ui->slider_29;
    sliderMap[EQFrequency31Range::Frequency16000Hz] = ui->slider_30;
    sliderMap[EQFrequency31Range::Frequency20000Hz] = ui->slider_31;
}
