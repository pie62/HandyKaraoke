#include "RhythmWidget.h"
#include "ui_RhythmWidget.h"

#include <QPainter>

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
}

RhythmWidget::~RhythmWidget()
{
    delete ui;
}

void RhythmWidget::setBpm(int bpm)
{
    ui->lbBpm->setText( QString::number(bpm) );
}

void RhythmWidget::reset()
{
    changeBeatInBar = false;
    _currentBeat = -1;
    _barIndex = 0;

    beatBarIndex = 0;
    timeSingIndex = 0;

    nBeatInBar = 4;
    lastBeat = 0;
    if (_beatInBar.count() > 0) {
        nBeatInBar = _beatInBar.keys().at(0);
        lastBeat = _beatInBar.values().at(0) * nBeatInBar;
    }

    _barCount = 0;
    for (int nb : _beatInBar.values()) {
        _barCount += nb;
    }

    ui->lbBeat->setText("0:" + QString::number(_barCount+1));
    displayBeats(nBeatInBar);
}

void RhythmWidget::setBeat(const QMap<int, int> &beatInBar, int beatCount)
{
    _beatInBar.clear();
    _beatInBar = beatInBar;
    _beatCount = beatCount;

    reset();
}

void RhythmWidget::setCurrentBeat(int b)
{
    if (b == _currentBeat || b > _beatCount)
        return;

    _currentBeat = b;

    if (beatBarIndex == 0) {
        if (changeBeatInBar && timeSingIndex < _beatInBar.count()) {
            nBeatInBar = _beatInBar.keys().at(timeSingIndex);
            lastBeat += _beatInBar.values().at(timeSingIndex) * nBeatInBar;
            changeBeatInBar = false;
            displayBeats(nBeatInBar);
        }
        ui->lbBeat->setText(QString::number(_barIndex+1)  + ":" + QString::number(_barCount+1));
        beats[nBeatInBar-1]->off();
        beats[0]->on();
    }
    else {
        beats[beatBarIndex-1]->off();
        beats[beatBarIndex]->on();
    }
    beatBarIndex++;


    if (beatBarIndex == nBeatInBar) {
        beatBarIndex = 0;
        _barIndex++;
    }

    if (_currentBeat == (lastBeat-1)) {
        timeSingIndex++;
        changeBeatInBar = true;
    }
}

void RhythmWidget::setSeekBeat(int b)
{
    if (b == _currentBeat || b < 0 || b > _beatCount)
        return;

    reset();

    for (int i=0; i<b; i++) {
        _currentBeat = i;

        if (beatBarIndex == 0) {
            if (changeBeatInBar && timeSingIndex < _beatInBar.count()) {
                nBeatInBar = _beatInBar.keys().at(timeSingIndex);
                lastBeat += _beatInBar.values().at(timeSingIndex) * nBeatInBar;
                changeBeatInBar = false;
            }
        }
        beatBarIndex++;


        if (beatBarIndex == nBeatInBar) {
            beatBarIndex = 0;
            _barIndex++;
        }


        if (_currentBeat == (lastBeat-1)) {
            timeSingIndex++;
            changeBeatInBar = true;
        }
    } // end for loop

    displayBeats(nBeatInBar);
    beats[beatBarIndex]->on();
    ui->lbBeat->setText(QString::number(_barIndex+1)  + ":" + QString::number(_barCount+1));
}

void RhythmWidget::offAllBeats()
{
    for (RhythmRectangle *rr : beats) {
        rr->off();
    }
}

void RhythmWidget::displayBeats(int n)
{
    for (int i=0; i<5; i++) {
        if (i < n) {
           beats[i]->show();
        }
        else {
            beats[i]->hide();
        }
        beats[i]->off();
    }
}

RhythmRectangle::RhythmRectangle(QWidget *parent) : QWidget(parent)
{
    _offColor = Qt::white;
    _onColor = QColor(30,144,255);
}

void RhythmRectangle::on()
{
    if (_isOn)
        return;

    _isOn = true;
    update();
}

void RhythmRectangle::off()
{
    if (!_isOn)
        return;

    _isOn = false;
    update();
}

void RhythmRectangle::setOnColor(const QColor &c)
{
    _onColor = c;
    update();
}

void RhythmRectangle::setOffColor(const QColor &c)
{
    _offColor = c;
    update();
}

void RhythmRectangle::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
    p.setRenderHints(QPainter::Antialiasing);

    p.setPen(QPen(Qt::black, 2));

    if (_isOn) {
        p.setBrush(_onColor);
        p.drawRect(0, 0, width(), height());
    }
    else {
        p.setBrush(_offColor);
        p.drawRect(0, 0, width(), height());
    }

    p.end();
}
