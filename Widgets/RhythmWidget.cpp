#include "RhythmWidget.h"
#include "ui_RhythmWidget.h"

#include <QPainter>
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
    beats.append(ui->b6);

    beats[4]->hide();
    beats[5]->hide();
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
    beats[_tempIndex]->off();

    _currentBar = -1;
    _currentBeat = -1;
    _currentBeatInBar = _signatureBeats.count() > 0 ? _signatureBeats[0].nBeatInBar : 4;
    _tempIndex = 0;


    ui->lbBeat->setText("0:" + QString::number(_barCount+1));
    displayBeats(_currentBeatInBar);
}

void RhythmWidget::setBeat(const QList<SignatureBeat> &signatureBeats, int beatCount)
{
    _signatureBeats.clear();
    _signatureBeats = signatureBeats;
    _beatCount = beatCount;

    { // Count bar
       _barCount= 0;
        int lastBeat = 0, lastBeatInBar = 0;
        for (SignatureBeat sigBeat : signatureBeats) {
            lastBeat = sigBeat.nBeat;
            if (lastBeat > 0) {
                _barCount += lastBeat / lastBeatInBar;
            }
            lastBeatInBar = sigBeat.nBeatInBar;
        }
        _barCount += (beatCount - lastBeat) / lastBeatInBar;
    }

    reset();
}

void RhythmWidget::setCurrentBeat(int b)
{
    if (b == _currentBeat || b > _beatCount)
        return;

    _currentBeat = b;

    int lastBeat = 0;
    int lastBar = 0;
    int beatInbar = 4;
    for (SignatureBeat sigBeat : _signatureBeats) {
        if (sigBeat.nBeat > _currentBeat) {
            break;
        }
        lastBar += (sigBeat.nBeat - lastBeat) / beatInbar;
        lastBeat = sigBeat.nBeat;
        beatInbar = sigBeat.nBeatInBar;
    }

    if (beatInbar != _currentBeatInBar) {
        _currentBeatInBar = beatInbar;
        displayBeats(_currentBeatInBar);
    }

    int cBar = ((_currentBeat - lastBeat) / beatInbar) + lastBar;
    if (_currentBar != cBar) {
        _currentBar = cBar;
        ui->lbBeat->setText(QString::number(_currentBar+1)  + ":" + QString::number(_barCount+1));
    }

    int beatIndex = (_currentBeat - lastBeat) % _currentBeatInBar;
    beats[_tempIndex]->off();
    beats[beatIndex]->on();
    _tempIndex = beatIndex;
}

void RhythmWidget::offAllBeats()
{
    for (RhythmRectangle *rr : beats) {
        rr->off();
    }
}

void RhythmWidget::displayBeats(int n)
{
    for (int i=0; i<6; i++) {
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
