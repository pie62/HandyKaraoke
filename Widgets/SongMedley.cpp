#include "SongMedley.h"
#include "ui_SongMedley.h"

SongMedley::SongMedley(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SongMedley)
{
    ui->setupUi(this);
}

SongMedley::~SongMedley()
{
    delete ui;
}

int SongMedley::cutStartBar()
{
    return ui->spinStart->value();
}

int SongMedley::cutEndBar()
{
    return ui->spinEnd->value();
}

void SongMedley::setup(int cutStartBar, int cutEndBar)
{
    ui->spinStart->setValue(cutStartBar);
    ui->spinEnd->setValue(cutEndBar);

    ui->spinStart->setFocus();
}
