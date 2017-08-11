#include "Equalizer24BandDialog.h"
#include "ui_Equalizer24BandDialog.h"

#include <QDebug>

Equalizer24BandDialog::Equalizer24BandDialog(QWidget *parent, Equalizer24BandFX *eqfx) :
    QDialog(parent),
    ui(new Ui::Equalizer24BandDialog)
{
    ui->setupUi(this);

    this->eqfx = eqfx;

    signalLevelMapper = new QSignalMapper(this);
    signalUserLevelMapper = new QSignalMapper(this);
    signalMouseDoubleClick = new QSignalMapper(this);

    mapLabelDb();
    mapSlider();

    int a=0;

    if (eqfx != nullptr)
    {
        if (eqfx->isOn())
            ui->btnSwitch->on();

        for (auto const& x : eqfx->gain())
        {
            sliderMap[x.first]->setLevel(x.second*10);
        }
    }

    connect(ui->btnSwitch, SIGNAL(userSwitchChanged(bool)),
            this, SLOT(onBtnSwitchUserSwitchChanged(bool)));
}

Equalizer24BandDialog::~Equalizer24BandDialog()
{
    delete signalMouseDoubleClick;
    delete signalUserLevelMapper;
    delete signalLevelMapper;

    sliderMap.clear();
    lbdbMap.clear();

    delete ui;
}

void Equalizer24BandDialog::mapLabelDb()
{
    lbdbMap[EQFrequency24Range::Frequency80Hz]    = ui->lbdb_1;
    lbdbMap[EQFrequency24Range::Frequency100Hz]   = ui->lbdb_2;
    lbdbMap[EQFrequency24Range::Frequency125Hz]   = ui->lbdb_3;
    lbdbMap[EQFrequency24Range::Frequency160Hz]   = ui->lbdb_4;
    lbdbMap[EQFrequency24Range::Frequency200Hz]   = ui->lbdb_5;
    lbdbMap[EQFrequency24Range::Frequency250Hz]   = ui->lbdb_6;
    lbdbMap[EQFrequency24Range::Frequency315Hz]   = ui->lbdb_7;
    lbdbMap[EQFrequency24Range::Frequency400Hz]   = ui->lbdb_8;
    lbdbMap[EQFrequency24Range::Frequency500Hz]   = ui->lbdb_9;
    lbdbMap[EQFrequency24Range::Frequency630Hz]   = ui->lbdb_10;
    lbdbMap[EQFrequency24Range::Frequency800Hz]   = ui->lbdb_11;
    lbdbMap[EQFrequency24Range::Frequency1000Hz]  = ui->lbdb_12;
    lbdbMap[EQFrequency24Range::Frequency1250Hz]  = ui->lbdb_13;
    lbdbMap[EQFrequency24Range::Frequency1600Hz]  = ui->lbdb_14;
    lbdbMap[EQFrequency24Range::Frequency2000Hz]  = ui->lbdb_15;
    lbdbMap[EQFrequency24Range::Frequency2500Hz]  = ui->lbdb_16;
    lbdbMap[EQFrequency24Range::Frequency3150Hz]  = ui->lbdb_17;
    lbdbMap[EQFrequency24Range::Frequency4000Hz]  = ui->lbdb_18;
    lbdbMap[EQFrequency24Range::Frequency5000Hz]  = ui->lbdb_19;
    lbdbMap[EQFrequency24Range::Frequency6300Hz]  = ui->lbdb_20;
    lbdbMap[EQFrequency24Range::Frequency8000Hz]  = ui->lbdb_21;
    lbdbMap[EQFrequency24Range::Frequency10000Hz] = ui->lbdb_22;
    lbdbMap[EQFrequency24Range::Frequency12500Hz] = ui->lbdb_23;
    lbdbMap[EQFrequency24Range::Frequency16000Hz] = ui->lbdb_24;
}

void Equalizer24BandDialog::mapSlider()
{
    sliderMap[EQFrequency24Range::Frequency80Hz]    = ui->slider_1;
    sliderMap[EQFrequency24Range::Frequency100Hz]   = ui->slider_2;
    sliderMap[EQFrequency24Range::Frequency125Hz]   = ui->slider_3;
    sliderMap[EQFrequency24Range::Frequency160Hz]   = ui->slider_4;
    sliderMap[EQFrequency24Range::Frequency200Hz]   = ui->slider_5;
    sliderMap[EQFrequency24Range::Frequency250Hz]   = ui->slider_6;
    sliderMap[EQFrequency24Range::Frequency315Hz]   = ui->slider_7;
    sliderMap[EQFrequency24Range::Frequency400Hz]   = ui->slider_8;
    sliderMap[EQFrequency24Range::Frequency500Hz]   = ui->slider_9;
    sliderMap[EQFrequency24Range::Frequency630Hz]   = ui->slider_10;
    sliderMap[EQFrequency24Range::Frequency800Hz]   = ui->slider_11;
    sliderMap[EQFrequency24Range::Frequency1000Hz]  = ui->slider_12;
    sliderMap[EQFrequency24Range::Frequency1250Hz]  = ui->slider_13;
    sliderMap[EQFrequency24Range::Frequency1600Hz]  = ui->slider_14;
    sliderMap[EQFrequency24Range::Frequency2000Hz]  = ui->slider_15;
    sliderMap[EQFrequency24Range::Frequency2500Hz]  = ui->slider_16;
    sliderMap[EQFrequency24Range::Frequency3150Hz]  = ui->slider_17;
    sliderMap[EQFrequency24Range::Frequency4000Hz]  = ui->slider_18;
    sliderMap[EQFrequency24Range::Frequency5000Hz]  = ui->slider_19;
    sliderMap[EQFrequency24Range::Frequency6300Hz]  = ui->slider_20;
    sliderMap[EQFrequency24Range::Frequency8000Hz]  = ui->slider_21;
    sliderMap[EQFrequency24Range::Frequency10000Hz] = ui->slider_22;
    sliderMap[EQFrequency24Range::Frequency12500Hz] = ui->slider_23;
    sliderMap[EQFrequency24Range::Frequency16000Hz] = ui->slider_24;


    int i=0;
    for (Slider *slider : sliderMap.values())
    {
        slider->setMaximumLevel(150);
        slider->setMinimumLevel(-150);
        slider->setLevel(0);
        slider->setChangeStep(10);
        slider->setTickCount(24);

        connect(slider, SIGNAL(levelChanged(int)), signalLevelMapper, SLOT(map()));
        signalLevelMapper->setMapping(slider, i);

        connect(slider, SIGNAL(userLevelChanged(int)), signalUserLevelMapper, SLOT(map()));
        signalUserLevelMapper->setMapping(slider, i);

        connect(slider, SIGNAL(mouseDoubleClicked()), signalMouseDoubleClick, SLOT(map()));
        signalMouseDoubleClick->setMapping(slider, i);

        i++;
    }

    connect(signalLevelMapper, SIGNAL(mapped(int)), this, SLOT(onSliderLevelChanged(int)));
    connect(signalUserLevelMapper, SIGNAL(mapped(int)), this, SLOT(onSliderUserLevelChanged(int)));
    connect(signalMouseDoubleClick, SIGNAL(mapped(int)), this, SLOT(onSliderMouseDoubleClicked(int)));
}

void Equalizer24BandDialog::onBtnSwitchUserSwitchChanged(bool switchOn)
{
    if (switchOn)
        eqfx->on();
    else
        eqfx->off();

    emit switchChanged(switchOn);
}

void Equalizer24BandDialog::onSliderLevelChanged(int sliderIndex)
{
    Slider *slider = sliderMap.values()[sliderIndex];
    QLabel *label = lbdbMap.values()[sliderIndex];

    label->setText(QString::number(slider->level() / 10.0f, 'f', 1));
}

void Equalizer24BandDialog::onSliderUserLevelChanged(int sliderIndex)
{
    Slider *slider = sliderMap.values()[sliderIndex];
    EQFrequency24Range freq = sliderMap.keys()[sliderIndex];

    if (eqfx == nullptr)
        return;

    eqfx->setGain(freq, slider->level() / 10.0f);
}

void Equalizer24BandDialog::onSliderMouseDoubleClicked(int sliderIndex)
{
    Slider *slider = sliderMap.values()[sliderIndex];

    if (slider->level() == 0)
        return;

    slider->setLevel(0);

    onSliderUserLevelChanged(sliderIndex);
}

void Equalizer24BandDialog::on_btnReset_clicked()
{
    for (int i=0; i<24; i++)
    {
        onSliderMouseDoubleClicked(i);
    }
}
