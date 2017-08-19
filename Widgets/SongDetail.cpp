#include "SongDetail.h"
#include "ui_SongDetail.h"

SongDetail::SongDetail(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SongDetail)
{
    ui->setupUi(this);
}

SongDetail::~SongDetail()
{
    delete ui;
}

void SongDetail::setDetail(Song *song)
{
    ui->lbId->setText(song->id());
    ui->lbName->setText(song->name());
    ui->lbArtist->setText(song->artist());
    ui->lbKey->setText(song->key());
//    ui->lbBpm->setText(QString::number(song->tempo()) + " BPM");
    ui->lbBpm->setText(QString::number(_bpm) + " BPM");
    ui->lbType->setText(song->songType());
}

void SongDetail::setBPM(int bpm)
{
    _bpm = bpm;
}
