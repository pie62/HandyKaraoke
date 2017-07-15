#include "ReverbDialog.h"
#include "ui_ReverbDialog.h"

ReverbDialog::ReverbDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReverbDialog)
{
    ui->setupUi(this);
}

ReverbDialog::~ReverbDialog()
{
    delete ui;
}
