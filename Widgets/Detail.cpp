#include "Detail.h"
#include "ui_Detail.h"

Detail::Detail(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Detail)
{
    ui->setupUi(this);
}

Detail::~Detail()
{
    delete ui;
}

void Detail::setDetail(const QString &text, const QString &value)
{
    ui->lbText->setText(text);
    ui->lbValue->setText(value);
}

void Detail::setText(const QString &text)
{
    ui->lbText->setText(text);
}

void Detail::setValue(const QString &value)
{
    ui->lbValue->setText(value);
}
