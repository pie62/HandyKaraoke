#include "SongDetail.h"
#include "ui_SongDetail.h"

SongDetail::SongDetail(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SongDetail)
{
    ui->setupUi(this);

    #ifdef __linux__
    {  // UI Font in linux
        QString fontName = "TH Sarabun New Bold";
        QFont f = ui->label->font();
        f.setBold(true);
        f.setFamily(fontName);
        f.setPointSize(14);

        ui->label->setFont(f);
        ui->label_2->setFont(f);
        ui->label_3->setFont(f);
        ui->label_4->setFont(f);
        ui->label_5->setFont(f);
        ui->label_6->setFont(f);

        ui->lbArtist->setFont(f);
        ui->lbBpm->setFont(f);
        ui->lbId->setFont(f);
        ui->lbKey->setFont(f);
        ui->lbName->setFont(f);
        ui->lbType->setFont(f);
    }
    #endif
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

    QString speedText;
    if (song->bpmSpeed() != 0) {
        QString s;
        if (song->bpmSpeed() > 0)
            s = "+" + QString::number(song->bpmSpeed());
        else
            s = QString::number(song->bpmSpeed());

        speedText =  + "(" + s + ")";
    }

    ui->lbBpm->setText(QString::number(song->tempo() + song->bpmSpeed()) + " " + speedText + " BPM");
    ui->lbType->setText(song->songType());
}
