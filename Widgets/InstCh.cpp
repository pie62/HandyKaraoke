#include "InstCh.h"
#include "ui_InstCh.h"

#include <QToolTip>
#include <QCursor>


InstCh::InstCh(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InstCh)
{
    ui->setupUi(this);

    btnDefault      = "font: bold 10pt; border-radius: 2px;"
                      "border: 1px solid rgb(158, 158, 158);";

    btnMuteStyle    = "color: rgb(255, 255, 255);font: bold 10pt;"
                      "border-radius: 2px;"
                      "border: 1px solid rgb(158, 158, 158);"
                      "background-color: rgb(229, 57, 53);";

    btnSoloStyle    = "color: rgb(255, 255, 255);font: bold 10pt;"
                      "border-radius: 2px;"
                      "border: 1px solid rgb(158, 158, 158);"
                      "background-color: rgb(46, 184, 114);";


    ui->slider->setMaximumLevel(100);
    ui->slider->setLevel(50);
    ui->slider->setToolTip("50");

    ui->vuBar->setMaximumLevel(127);

    ui->btnMute->setStyleSheet(btnDefault);
    ui->btnSolo->setStyleSheet(btnDefault);

    setInstrumentType(InstrumentType::Piano);

    connect(ui->btnMute, SIGNAL(clicked()), this, SLOT(onBtnMuteClicked()));
    connect(ui->btnSolo, SIGNAL(clicked()), this, SLOT(onBtnSoloClicked()));
    connect(ui->slider, SIGNAL(userLevelChanged(int)),
            this, SLOT(onSliderUserLevelChanged(int)));
    connect(ui->slider, SIGNAL(mouseDoubleClicked()),
            this, SLOT(onSliderDoubleClicked()));
}

InstCh::~InstCh()
{
    delete ui;
}

LEDVu *InstCh::vuBar()
{
    return ui->vuBar;
}

void InstCh::setInstrumentType(InstrumentType t)
{
    instType = t;
}

void InstCh::setInstrumentName(const QString &name, const QString &tooltip)
{
    ui->lbName->setText(name);
    ui->lbName->setToolTip(tooltip);
    ui->lbImage->setToolTip(tooltip);
}

void InstCh::setInstrumentImage(const QImage &img)
{
    ui->lbImage->setPixmap(QPixmap::fromImage(img));
}

void InstCh::setMuteButton(bool m)
{
    if (m == mute)
        return;

    mute = m;
    if (mute)
        ui->btnMute->setStyleSheet(btnMuteStyle);
    else
        ui->btnMute->setStyleSheet(btnDefault);
}

void InstCh::setSoloButton(bool s)
{
    if (s == solo)
        return;

    solo = s;
    if (solo)
        ui->btnSolo->setStyleSheet(btnSoloStyle);
    else
        ui->btnSolo->setStyleSheet(btnDefault);
}

void InstCh::setSliderLevel(int v)
{
    ui->slider->setLevel(v);
    ui->slider->setToolTip(QString::number(v));
}

void InstCh::peak(int v)
{
    if (v <= ui->vuBar->level() )
        return;

    ui->vuBar->peak(v);
}

void InstCh::onBtnMuteClicked()
{
    mute = !mute;

    if (mute) {
        ui->btnMute->setStyleSheet(btnMuteStyle);
    }
    else {
        ui->btnMute->setStyleSheet(btnDefault);
    }

    emit muteChanged(instType, mute);
}

void InstCh::onBtnSoloClicked()
{
    solo = !solo;

    if (solo) {
        ui->btnSolo->setStyleSheet(btnSoloStyle);
    }
    else {
        ui->btnSolo->setStyleSheet(btnDefault);
    }

    emit soloChanged(instType, solo);
}

void InstCh::onSliderUserLevelChanged(int v)
{
    ui->slider->setToolTip(QString::number(v));
    QToolTip::showText(QCursor::pos(), QString::number(v));
    emit sliderLevelChanged(instType, v);
}

void InstCh::onSliderDoubleClicked()
{
    emit sliderDoubleClicked(instType);
}
