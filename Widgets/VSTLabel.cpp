#include "VSTLabel.h"
#include "ui_VSTLabel.h"

#include <QMouseEvent>


VSTLabel::VSTLabel(QWidget *parent, const QString &label, int fxIndex, bool bypass) :
    QWidget(parent),
    ui(new Ui::VSTLabel)
{
    ui->setupUi(this);

    this->fxIndex = fxIndex;
    this->fxBypass = bypass;

    ui->label->setText(label);
    ui->label->setToolTip(label);

    if (fxBypass)
        ui->btn->setStyleSheet("border: 1px solid rgb(0, 170, 255);");
    else
        ui->btn->setStyleSheet("background: rgb(0, 170, 255); border: none;");

    connect(this, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(contextMenuRequested(QPoint)));
}

VSTLabel::~VSTLabel()
{
    delete ui;
}

void VSTLabel::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton)
        return;

    emit doubleClicked(fxIndex);
}

void VSTLabel::on_btn_clicked()
{
    fxBypass = !fxBypass;

    if (fxBypass)
        ui->btn->setStyleSheet("border: 1px solid rgb(0, 170, 255);");
    else
        ui->btn->setStyleSheet("background: rgb(0, 170, 255); border: none;");

    emit byPassChanged(fxIndex, fxBypass);
}

void VSTLabel::contextMenuRequested(const QPoint &pos)
{
    emit menuRequested(fxIndex, pos);
}
