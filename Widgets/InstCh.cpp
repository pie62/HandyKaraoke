#include "InstCh.h"
#include "ui_InstCh.h"

#include "VSTLabel.h"
#include <QToolTip>
#include <QScrollBar>
#include <QCursor>
#include <QMouseEvent>


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

    QString fxListScroll = "QScrollBar:vertical {width: 8px;} ";

    QScrollBar *bar = ui->fxList->verticalScrollBar();
    bar->setStyleSheet(fxListScroll);


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

    connect(this, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(contextMenuRequested(QPoint)));
}

InstCh::~InstCh()
{
    delete ui;
}

LEDVu *InstCh::vuBar()
{
    return ui->vuBar;
}

QString InstCh::instrumentName()
{
    return ui->lbName->text();
}

QString InstCh::fullInstrumentName()
{
    return ui->lbName->toolTip();
}

void InstCh::addFXLabel(const QString &label, int fxIndex, bool bypass)
{
    QListWidgetItem *item = new QListWidgetItem(ui->fxList);

    VSTLabel *vstLabel = new VSTLabel(ui->fxList, label, fxIndex, bypass);

    connect(vstLabel, SIGNAL(byPassChanged(int,bool)),
            this, SLOT(onFxByPassChanged(int,bool)));
    connect(vstLabel, SIGNAL(doubleClicked(int)),
            this, SLOT(onFxDoubleClicked(int)));
    connect(vstLabel, SIGNAL(menuRequested(int,QPoint)),
            this, SLOT(onFxMenuRequested(int,QPoint)));

    ui->fxList->addItem(item);
    ui->fxList->setItemWidget(item, vstLabel);
}

void InstCh::removeVSTLabel(int fxIndex)
{
    QListWidgetItem *item = ui->fxList->takeItem(fxIndex);
    QWidget *widget = ui->fxList->itemWidget(item);
    delete widget;
    delete item;

    for (int i=fxIndex; i<ui->fxList->count(); i++) {
        item = ui->fxList->item(i);
        widget = ui->fxList->itemWidget(item);
        VSTLabel *label = dynamic_cast<VSTLabel*>(widget);
        label->setFxIndex(i);
    }
}

void InstCh::setInstrumentType(InstrumentType t)
{
    instType = t;
}

void InstCh::setInstrumentName(const QString &name)
{
    ui->lbName->setText(name);
}

void InstCh::setFullInstrumentName(const QString &name)
{
    ui->lbName->setToolTip(name);
    ui->lbImage->setToolTip(name);
}

void InstCh::setInstrumentNames(const QString &name, const QString &tooltip)
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

void InstCh::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && ui->lbImage->underMouse())
        emit imageDoubleClicked(instType);
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

void InstCh::contextMenuRequested(const QPoint &pos)
{
    emit menuRequested(instType, pos);
}

void InstCh::onFxMenuRequested(int fxIndex, const QPoint &pos)
{
    //QListWidgetItem *item = ui->fxList->item(fxIndex);
    //QWidget *widget = ui->fxList->itemWidget(item);
    //emit fxRemoveMenuRequested(instType, fxIndex, widget->mapToGlobal(pos));
    emit fxRemoveMenuRequested(instType, fxIndex, pos);
}

void InstCh::onFxByPassChanged(int fxIndex, bool bypass)
{
    emit fxByPassChanged(instType, fxIndex, bypass);
}

void InstCh::onFxDoubleClicked(int fxIndex)
{
    emit fxDoubleClicked(instType, fxIndex);
}
