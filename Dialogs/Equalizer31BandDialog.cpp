#include "Equalizer31BandDialog.h"
#include "ui_Equalizer31BandDialog.h"


Equalizer31BandDialog::Equalizer31BandDialog(QWidget *parent, QList<Equalizer31BandFX *> eqfxs) :
    QDialog(parent),
    ui(new Ui::Equalizer31BandDialog)
{
    ui->setupUi(this);

    this->eqfxs = eqfxs;

    signalLevelMapper = new QSignalMapper(this);
    signalUserLevelMapper = new QSignalMapper(this);
    signalMouseDoubleClick = new QSignalMapper(this);

    mapLabelDb();
    mapSlider();

    auto eqfx = eqfxs[0];
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

Equalizer31BandDialog::~Equalizer31BandDialog()
{
    delete signalMouseDoubleClick;
    delete signalUserLevelMapper;
    delete signalLevelMapper;

    sliderMap.clear();
    lbdbMap.clear();

    delete ui;
}

bool Equalizer31BandDialog::openned = false;

bool Equalizer31BandDialog::isOpenned()
{
    return openned;
}

void Equalizer31BandDialog::showEvent(QShowEvent *)
{
    openned = true;
}

void Equalizer31BandDialog::closeEvent(QCloseEvent *)
{
    openned = false;
}

void Equalizer31BandDialog::mapLabelDb()
{
    lbdbMap[EQFrequency31Range::Frequency20Hz]    = ui->lbdb;
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

void Equalizer31BandDialog::mapSlider()
{
    sliderMap[EQFrequency31Range::Frequency20Hz]    = ui->slider;
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


    int i=0;
    for (Slider *slider : sliderMap.values())
    {
        slider->setMaximumLevel(150);
        slider->setMinimumLevel(-150);
        slider->setLevel(0);
        slider->setChangeStep(10);
        slider->setTickCount(31);

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

void Equalizer31BandDialog::onBtnSwitchUserSwitchChanged(bool switchOn)
{
    for (auto eqfx : eqfxs)
    {
        if (switchOn)
            eqfx->on();
        else
            eqfx->off();
    }

    emit switchChanged(switchOn);
}

void Equalizer31BandDialog::onSliderLevelChanged(int sliderIndex)
{
    Slider *slider = sliderMap.values()[sliderIndex];
    QLabel *label = lbdbMap.values()[sliderIndex];

    label->setText(QString::number(slider->level() / 10.0f, 'f', 1));
}

void Equalizer31BandDialog::onSliderUserLevelChanged(int sliderIndex)
{
    Slider *slider = sliderMap.values()[sliderIndex];
    EQFrequency31Range freq = sliderMap.keys()[sliderIndex];

    for (auto eqfx : eqfxs)
        eqfx->setGain(freq, slider->level() / 10.0f);
}

void Equalizer31BandDialog::onSliderMouseDoubleClicked(int sliderIndex)
{
    Slider *slider = sliderMap.values()[sliderIndex];

    if (slider->level() == 0)
        return;

    slider->setLevel(0);

    onSliderUserLevelChanged(sliderIndex);
}

void Equalizer31BandDialog::on_btnReset_clicked()
{
    for (int i=0; i<31; i++)
    {
        onSliderMouseDoubleClicked(i);
    }
}
