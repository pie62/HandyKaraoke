#include "VSTLabel.h"
#include "ui_VSTLabel.h"

#include <QMouseEvent>
#include <QDebug>

VSTLabel::VSTLabel(QWidget *parent, const QString &label) :
    QWidget(parent),
    ui(new Ui::VSTLabel)
{
    ui->setupUi(this);

    ui->label->setText(label);

    ui->btn->setStyleSheet("border: 1px solid rgb(0, 170, 255);");
    ui->btn->setStyleSheet("background: rgb(0, 170, 255); border: none;");
}

VSTLabel::~VSTLabel()
{
    delete ui;
}

void VSTLabel::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton)
        return;

    qDebug() << "Double clicked";
}

void VSTLabel::on_btn_clicked()
{
    fxOn = !fxOn;

    if (fxOn)
        ui->btn->setStyleSheet("border: 1px solid rgb(0, 170, 255);");
    else
        ui->btn->setStyleSheet("background: rgb(0, 170, 255); border: none;");
}
