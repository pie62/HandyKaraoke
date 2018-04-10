#include "Equalizer31Band.h"
#include "ui_Equalizer31Band.h"

Equalizer31Band::Equalizer31Band(QWidget *parent, QList<HFX> fxs) :
    QDialog(parent),
    ui(new Ui::Equalizer31Band)
{
    ui->setupUi(this);

    this->fxs = fxs;

    signalLevelMapper = new QSignalMapper(this);
    signalMouseDoubleClick = new QSignalMapper(this);

    this->mapLabels();
    this->mapSliders();

    for (EQFreq31Range freq : lbdbMap.keys())
    {
        int i = static_cast<int>(freq);
        Slider* slider = sliderMap[freq];

        slider->setMaximumLevel(150);
        slider->setMinimumLevel(-150);
        slider->setChangeStep(10);
        slider->setTickCount(31);

        BASS_BFX_PEAKEQ eq;
        eq.lBand = i;

        if (BASS_FXGetParameters(fxs[i], &eq))
        {
            lbdbMap[freq]->setText(QString::number(eq.fGain, 'f', 1));
            slider->setLevel(eq.fGain * 10);
        }

        connect(slider, SIGNAL(userLevelChanged(int)), signalLevelMapper, SLOT(map()));
        signalLevelMapper->setMapping(slider, i);

        connect(slider, SIGNAL(mouseDoubleClicked()), signalMouseDoubleClick, SLOT(map()));
        signalMouseDoubleClick->setMapping(slider, i);
    }

    connect(signalLevelMapper, SIGNAL(mapped(int)), this, SLOT(setGian(int)));
    connect(signalMouseDoubleClick, SIGNAL(mapped(int)), this, SLOT(resetGian(int)));
}

Equalizer31Band::~Equalizer31Band()
{
    delete ui;
}

void Equalizer31Band::setGian(int sliderIndex)
{
    EQFreq31Range freq = sliderMap.keys()[sliderIndex];
    Slider *slider = sliderMap.values()[sliderIndex];

    BASS_BFX_PEAKEQ eq;
    eq.lBand = sliderIndex;

    if (BASS_FXGetParameters(fxs[sliderIndex], &eq))
    {
        eq.fGain = slider->level() / 10.0f;
        BASS_FXSetParameters(fxs[sliderIndex], &eq);

        lbdbMap[freq]->setText(QString::number(eq.fGain, 'f', 1));
    }
}

void Equalizer31Band::resetGian(int sliderIndex)
{
    EQFreq31Range freq = sliderMap.keys()[sliderIndex];
    Slider *slider = sliderMap.values()[sliderIndex];

    BASS_BFX_PEAKEQ eq;
    eq.lBand = sliderIndex;

    if (BASS_FXGetParameters(fxs[sliderIndex], &eq))
    {
        eq.fGain = 0.0f;
        BASS_FXSetParameters(fxs[sliderIndex], &eq);

        slider->setLevel(0);
        lbdbMap[freq]->setText(QString::number(eq.fGain, 'f', 1));
    }
}


void Equalizer31Band::on_btnReset_clicked()
{
    for (int i=0; i<31; i++)
    {
        resetGian(i);
    }
}

void Equalizer31Band::mapLabels()
{
    lbdbMap[EQFreq31Range::Frequency20Hz]    = ui->lbdb_1;
    lbdbMap[EQFreq31Range::Frequency25Hz]    = ui->lbdb_2;
    lbdbMap[EQFreq31Range::Frequency31d5Hz]  = ui->lbdb_3;
    lbdbMap[EQFreq31Range::Frequency40Hz]    = ui->lbdb_4;
    lbdbMap[EQFreq31Range::Frequency50Hz]    = ui->lbdb_5;
    lbdbMap[EQFreq31Range::Frequency63Hz]    = ui->lbdb_6;
    lbdbMap[EQFreq31Range::Frequency80Hz]    = ui->lbdb_7;
    lbdbMap[EQFreq31Range::Frequency100Hz]   = ui->lbdb_8;
    lbdbMap[EQFreq31Range::Frequency125Hz]   = ui->lbdb_9;
    lbdbMap[EQFreq31Range::Frequency160Hz]   = ui->lbdb_10;
    lbdbMap[EQFreq31Range::Frequency200Hz]   = ui->lbdb_11;
    lbdbMap[EQFreq31Range::Frequency250Hz]   = ui->lbdb_12;
    lbdbMap[EQFreq31Range::Frequency315Hz]   = ui->lbdb_13;
    lbdbMap[EQFreq31Range::Frequency400Hz]   = ui->lbdb_14;
    lbdbMap[EQFreq31Range::Frequency500Hz]   = ui->lbdb_15;
    lbdbMap[EQFreq31Range::Frequency630Hz]   = ui->lbdb_16;
    lbdbMap[EQFreq31Range::Frequency800Hz]   = ui->lbdb_17;
    lbdbMap[EQFreq31Range::Frequency1000Hz]  = ui->lbdb_18;
    lbdbMap[EQFreq31Range::Frequency1250Hz]  = ui->lbdb_19;
    lbdbMap[EQFreq31Range::Frequency1600Hz]  = ui->lbdb_20;
    lbdbMap[EQFreq31Range::Frequency2000Hz]  = ui->lbdb_21;
    lbdbMap[EQFreq31Range::Frequency2500Hz]  = ui->lbdb_22;
    lbdbMap[EQFreq31Range::Frequency3150Hz]  = ui->lbdb_23;
    lbdbMap[EQFreq31Range::Frequency4000Hz]  = ui->lbdb_24;
    lbdbMap[EQFreq31Range::Frequency5000Hz]  = ui->lbdb_25;
    lbdbMap[EQFreq31Range::Frequency6300Hz]  = ui->lbdb_26;
    lbdbMap[EQFreq31Range::Frequency8000Hz]  = ui->lbdb_27;
    lbdbMap[EQFreq31Range::Frequency10000Hz] = ui->lbdb_28;
    lbdbMap[EQFreq31Range::Frequency12500Hz] = ui->lbdb_29;
    lbdbMap[EQFreq31Range::Frequency16000Hz] = ui->lbdb_30;
    lbdbMap[EQFreq31Range::Frequency20000Hz] = ui->lbdb_31;
}

void Equalizer31Band::mapSliders()
{
    sliderMap[EQFreq31Range::Frequency20Hz]    = ui->slider_1;
    sliderMap[EQFreq31Range::Frequency25Hz]    = ui->slider_2;
    sliderMap[EQFreq31Range::Frequency31d5Hz]  = ui->slider_3;
    sliderMap[EQFreq31Range::Frequency40Hz]    = ui->slider_4;
    sliderMap[EQFreq31Range::Frequency50Hz]    = ui->slider_5;
    sliderMap[EQFreq31Range::Frequency63Hz]    = ui->slider_6;
    sliderMap[EQFreq31Range::Frequency80Hz]    = ui->slider_7;
    sliderMap[EQFreq31Range::Frequency100Hz]   = ui->slider_8;
    sliderMap[EQFreq31Range::Frequency125Hz]   = ui->slider_9;
    sliderMap[EQFreq31Range::Frequency160Hz]   = ui->slider_10;
    sliderMap[EQFreq31Range::Frequency200Hz]   = ui->slider_11;
    sliderMap[EQFreq31Range::Frequency250Hz]   = ui->slider_12;
    sliderMap[EQFreq31Range::Frequency315Hz]   = ui->slider_13;
    sliderMap[EQFreq31Range::Frequency400Hz]   = ui->slider_14;
    sliderMap[EQFreq31Range::Frequency500Hz]   = ui->slider_15;
    sliderMap[EQFreq31Range::Frequency630Hz]   = ui->slider_16;
    sliderMap[EQFreq31Range::Frequency800Hz]   = ui->slider_17;
    sliderMap[EQFreq31Range::Frequency1000Hz]  = ui->slider_18;
    sliderMap[EQFreq31Range::Frequency1250Hz]  = ui->slider_19;
    sliderMap[EQFreq31Range::Frequency1600Hz]  = ui->slider_20;
    sliderMap[EQFreq31Range::Frequency2000Hz]  = ui->slider_21;
    sliderMap[EQFreq31Range::Frequency2500Hz]  = ui->slider_22;
    sliderMap[EQFreq31Range::Frequency3150Hz]  = ui->slider_23;
    sliderMap[EQFreq31Range::Frequency4000Hz]  = ui->slider_24;
    sliderMap[EQFreq31Range::Frequency5000Hz]  = ui->slider_25;
    sliderMap[EQFreq31Range::Frequency6300Hz]  = ui->slider_26;
    sliderMap[EQFreq31Range::Frequency8000Hz]  = ui->slider_27;
    sliderMap[EQFreq31Range::Frequency10000Hz] = ui->slider_28;
    sliderMap[EQFreq31Range::Frequency12500Hz] = ui->slider_29;
    sliderMap[EQFreq31Range::Frequency16000Hz] = ui->slider_30;
    sliderMap[EQFreq31Range::Frequency20000Hz] = ui->slider_31;
}
