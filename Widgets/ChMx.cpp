#include "ChMx.h"
#include "ui_ChMx.h"

#include <QToolTip>
#include <QCursor>
#include <QMouseEvent>


ChMx::ChMx(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChMx)
{
    ui->setupUi(this);

    ui->vuBar->setMaximumLevel(127);

    /*btnDefault      = "color: rgb(24, 24, 24);font: bold 10pt;"
                      "border-radius: 2px;"
                      "border: 1px solid rgb(158, 158, 158);"
                      "background-color: rgb(245, 245, 245);";*/

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


    setChannelNumber(0);
    ui->btnMute->setStyleSheet(btnDefault);
    ui->btnSolo->setStyleSheet(btnDefault);

    ui->slider->setMaximumLevel(127);
    ui->slider->setLevel(100);

    connect(ui->lbNumber, SIGNAL(mouseHover()), this, SLOT(onChLabelMouseHover()));
    connect(ui->btnMute, SIGNAL(clicked()), this, SLOT(onBtnMuteClicked()));
    connect(ui->btnSolo, SIGNAL(clicked()), this, SLOT(onBtnSoloClicked()));
    connect(ui->slider, SIGNAL(levelChanged(int)), this, SLOT(onSliderValueChanged(int)));
}

ChMx::~ChMx()
{
    delete ui;
}

LEDVu *ChMx::vuBar()
{
    return ui->vuBar;
}

void ChMx::setMuteButton(bool m)
{
    if (m == mute)
        return;

    mute = m;
    if (mute) {
        ui->btnMute->setStyleSheet(btnMuteStyle);
    }
    else {
        ui->btnMute->setStyleSheet(btnDefault);
    }
}

void ChMx::setChannelNumber(int ch)
{
    if (ch < 0 || ch > 15)
        return;

    chNumber = ch;
    ui->lbNumber->setText(QString::number(ch+1));
}

void ChMx::setSliderValue(int v)
{
    disconnect(ui->slider, SIGNAL(levelChanged(int)), this, SLOT(onSliderValueChanged(int)));
    ui->slider->setLevel(v);
    ui->slider->setToolTip(QString::number(v));
    connect(ui->slider, SIGNAL(levelChanged(int)), this, SLOT(onSliderValueChanged(int)));
}

void ChMx::peak(int v)
{
    if (v <= ui->vuBar->level() )
        return;

    ui->vuBar->peak(v);
}

void ChMx::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton) {
        emit mouseRightClicked(chNumber, event->pos());
        event->ignore();
    }
}

void ChMx::onChLabelMouseHover()
{
    emit mouseHover(chNumber);
}

void ChMx::onBtnMuteClicked()
{
    mute = !mute;

    if (mute) {
        ui->btnMute->setStyleSheet(btnMuteStyle);
    }
    else {
        ui->btnMute->setStyleSheet(btnDefault);
    }

    emit muteChanged(chNumber, mute);
}

void ChMx::onBtnSoloClicked()
{
    solo = !solo;

    if (solo) {
        ui->btnSolo->setStyleSheet(btnSoloStyle);
    }
    else {
        ui->btnSolo->setStyleSheet(btnDefault);
    }

    emit soloChanged(chNumber, solo);
}

void ChMx::onSliderValueChanged(int v)
{
    ui->slider->setToolTip(QString::number(v));
    QToolTip::showText(QCursor::pos(), QString::number(v));
    emit sliderValueChanged(chNumber, v);
}
