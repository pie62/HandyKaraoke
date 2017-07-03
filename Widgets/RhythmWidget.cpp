#include "RhythmWidget.h"
#include "ui_RhythmWidget.h"

#include <QDebug>

RhythmWidget::RhythmWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RhythmWidget)
{
    ui->setupUi(this);

    beats.append(ui->b1);
    beats.append(ui->b2);
    beats.append(ui->b3);
    beats.append(ui->b4);
    beats.append(ui->b5);

    beats[4]->hide();

    defaultStyle = "background-color: rgb(255, 255, 255);"
                   "border: 2px solid  rgb(0, 0, 0);";
    beatStyle = "background-color: #00B8A9;"
                "border: 2px solid  rgb(0, 0, 0);";
}

RhythmWidget::~RhythmWidget()
{
    delete ui;
}

void RhythmWidget::setBpm(int bpm)
{
    ui->lbBpm->setText( QString::number(bpm) );
}

void RhythmWidget::setBeatCount(int bc)
{
    _beatCount = bc;
    ui->lbBeat->setText( QString::number(_currentBar) + ":" + QString::number(_beatCount) );
}

void RhythmWidget::setBeatInBar(int bb)
{
    _beatInBar = bb;

    int i=0;
    for (QFrame *bt : beats) {
        if (_beatInBar > i)
            bt->show();
        else
            bt->hide();
        i++;
    }
}

void RhythmWidget::setCurrentBeat(int b)
{
    if (b == _currentBeat)
        return;

    _currentBeat = b;

    int cb = ( (_currentBeat - 1 ) / _beatInBar) + 1;
    if (_currentBar != cb) {
        _currentBar = cb;
        ui->lbBeat->setText( QString::number(_currentBar) + ":" + QString::number(_beatCount) );
    }

    //ui->lbBeat->setText( QString::number(_currentBar) + ":" + QString::number(_beatCount) );

    /*int i = _currentBeat % _beatInBar;
    if (i == 1) {
        beats[_beatInBar - 1]->setStyleSheet(defaultStyle);
        beats[i-1]->setStyleSheet(beatStyle);
        ui->lbBeat->setText( QString::number(_currentBar) + ":" + QString::number(_beatCount) );
    }
    else if (i == 0) {
        beats[_beatInBar - 2]->setStyleSheet(defaultStyle);
        beats[_beatInBar - 1]->setStyleSheet(beatStyle);
    }
    else {
        beats[i - 2]->setStyleSheet(defaultStyle);
        beats[i-1]->setStyleSheet(beatStyle);
    }*/

    int i = _currentBeat % _beatInBar;

    switch (i) {
    case 0:
        beats[_beatInBar - 2]->setStyleSheet(defaultStyle);
        beats[_beatInBar - 1]->setStyleSheet(beatStyle);
        break;
    case 1:
        beats[_beatInBar-1]->setStyleSheet(defaultStyle);
        beats[0]->setStyleSheet(beatStyle);
        break;
    default:
        beats[i-2]->setStyleSheet(defaultStyle);
        beats[i-1]->setStyleSheet(beatStyle);
        break;
    }

    qDebug() << "beat in bar " << i;
}
