#include "LEDVu.h"

#include <QResizeEvent>
#include <QPainter>


LEDVu::LEDVu(QWidget *parent) : QFrame(parent)
{
    _timer = new QTimer();
    connect(_timer, SIGNAL(timeout()), this, SLOT(onTimerTimeout()));

    _animation = new QPropertyAnimation(this, "level");
    connect(_animation, SIGNAL(finished()), this, SLOT(onAnimationFinised()));

    _pixLedOn = QPixmap(size());
    _pixLedOff = QPixmap(size());

    _bgColor = Qt::black;

    _ledColorOn1 = Qt::green;
    _ledColorOn2 = Qt::yellow;
    _ledColorOn3 = Qt::red;

    _ledColorOff1 = QColor("#003300");
    _ledColorOff2 = QColor("#333300");
    _ledColorOff3 = QColor("#330000");

    /*_bgColor = QColor("#27323A");

    _ledColorOff1 = QColor("#435055");
    _ledColorOff2 = QColor("#435055");
    _ledColorOff3 = QColor("#435055");

    _ledColorOn1 = Qt::green;
    _ledColorOn2 = Qt::yellow;
    _ledColorOn3 = Qt::red;*/

    //---------------------------------------
}

LEDVu::~LEDVu()
{
    delete _animation;
    delete _timer;
}

void LEDVu::setBackGroundColor(const QColor &c)
{
    _bgColor = c;
    createLedPixmap();
    update();
}

void LEDVu::setLedColorOn1(const QColor &c)
{
    _ledColorOn1 = c;
    createLedPixmap();
    update();
}

void LEDVu::setLedColorOn2(const QColor &c)
{
    _ledColorOn2 = c;
    createLedPixmap();
    update();
}

void LEDVu::setLedColorOn3(const QColor &c)
{
    _ledColorOn3 = c;
    createLedPixmap();
    update();
}

void LEDVu::setLedColorOff1(const QColor &c)
{
    _ledColorOff1 = c;
    createLedPixmap();
    update();
}

void LEDVu::setLedColorOff2(const QColor &c)
{
    _ledColorOff2 = c;
    createLedPixmap();
    update();
}

void LEDVu::setLedColorOff3(const QColor &c)
{
    _ledColorOff3 = c;
    createLedPixmap();
    update();
}

void LEDVu::setPeakHoldMs(int msTime)
{
    if (msTime > 10000) _peakHoldMs = 10000;
    else if (msTime < 50) _peakHoldMs = 50;
    else _peakHoldMs = msTime;
}

void LEDVu::setLevel(int v)
{
    if (_vl == v)
        return;

    if (v > _maxVl) _vl = _maxVl;
    else if (v < _minVl) _vl = _minVl;
    else _vl = v;

    if (_showPeakHold && (_vl > _peakLevel)) {
        _timer->stop();
        _peakLevel = _vl;
        _timer->start(_peakHoldMs);
    }

    update();
    emit levelChanged(_vl);
}

void LEDVu::setMaximumLevel(int mav)
{
    _maxVl = mav;
    _absVl = abs(_maxVl) + abs(_minVl);
    update();
}

void LEDVu::setMinimumLevel(int miv)
{
    _minVl = miv;
    _absVl = abs(_maxVl) + abs(_minVl);
    update();
}

void LEDVu::peak(int v)
{
    if (v == _vl)
        return;

    _animation->stop();
    _animation->setDuration(_peakUpTime);
    _animation->setStartValue(_vl);
    _animation->setEndValue(v);
    _animation->start();
}

void LEDVu::setShowPeakHold(bool s)
{
    _showPeakHold = s;
    if (!s) {
        _timer->stop();
        _peakLevel = 0;
    }
    update();
}

void LEDVu::resizeEvent(QResizeEvent *event)
{
    _ledSize.setWidth(event->size().width()-2);
    _ledSize.setHeight(2);

    calculateLedCount(event->size());

    _pixLedOn = QPixmap(event->size());
    _pixLedOff = QPixmap(event->size());

    _pixSize.clear();
    _pixSize = createLedPixmap();
}

void LEDVu::paintEvent(QPaintEvent *event)
{
    int ledVl = _ledCount * _vl / _absVl;
    ledVl = _ledCount - ledVl;

    QPainter p(this);

    p.drawPixmap(0, 0, _pixLedOff);
    p.save();
    p.setClipRect(0, _pixSize.at(ledVl), width(), height());
    p.drawPixmap(0, 0, _pixLedOn);
    p.restore();

    if (_showPeakHold && _peakLevel > 0) {
        int pl = _ledCount * _peakLevel / _absVl;
        pl = _ledCount - pl;

        QRect r(QPoint(1, _pixSize.at(pl)+1), _ledSize);
        if (pl > (_ledCount2 + _ledCount3))
            p.fillRect(r, _ledColorOn1);
        else if (pl > _ledCount3)
            p.fillRect(r, _ledColorOn2);
        else
            p.fillRect(r, _ledColorOn3);
    }

    p.end();
}

void LEDVu::onAnimationFinised()
{
    if (_vl == 0)
        return;

    _animation->setDuration(_peakDownTime);
    _animation->setStartValue(_vl);
    _animation->setEndValue(0);
    _animation->start();
}

void LEDVu::onTimerTimeout()
{
    _timer->stop();
    _peakLevel = _vl;
    update();
    if (_peakLevel > 0) {
        _timer->start(_peakHoldMs);
    }
}

void LEDVu::calculateLedCount(const QSize &s)
{
    _ledCount = s.height() / 3;
    _ledCount3 = _ledCount * _ledPercent3 / 100;
    _ledCount2 = _ledCount * _ledPercent2 / 100;
    _ledCount1 = _ledCount - (_ledCount2 + _ledCount3);
}

QList<int> LEDVu::createLedPixmap()
{
    QList<int> sl;

    _pixLedOn.fill(_bgColor);
    _pixLedOff.fill(_bgColor);

    QPainter p1(&_pixLedOn);
    QPainter p2(&_pixLedOff);

    sl.append(0);
    int y = 1;
    for (int i=0; i<_ledCount; i++) {

        QRect r(QPoint(1, y), _ledSize);

        if (i > (_ledCount2 + _ledCount3)) {
            p1.fillRect(r, _ledColorOn1);
            p2.fillRect(r, _ledColorOff1);
        }
        else if (i > _ledCount3) {
            p1.fillRect(r, _ledColorOn2);
            p2.fillRect(r, _ledColorOff2);
        }
        else {
            p1.fillRect(r, _ledColorOn3);
            p2.fillRect(r, _ledColorOff3);
        }

        y += 3;
        sl.append(y-1);
    }

    p1.end();
    p2.end();

    return sl;
}

/*
QPixmap LEDVu::createLedPixmap(int index)
{
    QPixmap pix(size());
    pix.fill(_bgColor);

    QPainter p(&pix);

    int y = 1;
    for (int i=0; i<_ledCount; i++) {

        QRect r(QPoint(1, y), ledSize);

        if (i >= index) { // On Led
            if (i > (_ledCount2 + _ledCount3))
                p.fillRect(r, _ledColorOn1);
            else if (i > _ledCount3)
                p.fillRect(r, _ledColorOn2);
            else
                p.fillRect(r, _ledColorOn3);
        }
        else { // Off led
            if (i > (_ledCount2 + _ledCount3))
                p.fillRect(r, _ledColorOff1);
            else if (i > _ledCount3)
                p.fillRect(r, _ledColorOff2);
            else
                p.fillRect(r, _ledColorOff3);
        }

        y += 3;
    }

    p.end();

    return pix;
}
*/
