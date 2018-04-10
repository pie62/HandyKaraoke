#include "EQ31BandDialog.h"
#include "ui_EQ31BandDialog.h"

EQ31BandDialog::EQ31BandDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EQ31BandDialog)
{
    ui->setupUi(this);
}

EQ31BandDialog::~EQ31BandDialog()
{
    delete ui;
}
