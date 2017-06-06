#include "ChMx.h"
#include "ui_ChMx.h"

ChMx::ChMx(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChMx)
{
    ui->setupUi(this);
}

ChMx::~ChMx()
{
    delete ui;
}
