#include "Slider.h"

#include <QResizeEvent>
#include <QPainter>


SliderHandle::SliderHandle(QWidget *parent) : QFrame(parent)
{

}

void SliderHandle::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    QPen pen(palette().color(QPalette::Text), 1);
    p.setPen(pen);
    p.setBrush(palette().color(QPalette::Highlight));

    p.drawRoundedRect(0, 0, width(), height(), 3, 3);

    p.end();
}


// =====================================================================

Slider::Slider(QWidget *parent) : QFrame(parent)
{
    sHandle = new SliderHandle(this);
}

Slider::~Slider()
{
    delete sHandle;
}

void Slider::setLevel(int level)
{
    if (level == sLv)
        return;

    if (level > sMaxLv)
        sLv = sMaxLv;
    else if (level < sMinLv)
        sLv = sMinLv;
    else
        sLv = level;

    moveHandle();

    emit levelChanged(sLv);
    emit levelChanged(QString::number(sLv));
}

void Slider::setMinimumLevel(int minLevel)
{
    if (minLevel == sMinLv || minLevel >= sMaxLv)
        return;

    sMinLv = minLevel;
    if (sLv < sMinLv) {
        sLv = sMinLv;
        emit levelChanged(sLv);
        emit levelChanged(QString::number(sLv));
    }

    moveHandle();
}

void Slider::setMaximumLevel(int maxLevel)
{
    if (maxLevel == sMaxLv || maxLevel <= sMinLv)
        return;

    sMaxLv = maxLevel;
    if (sLv > sMaxLv) {
        sLv = sMaxLv;
        emit levelChanged(sLv);
        emit levelChanged(QString::number(sLv));
    }

    moveHandle();
}

void Slider::setChangeStep(int cstp)
{
    if (cstp < 1)
        return;

    sChangStep = cstp;
}

void Slider::setTickCount(int tc)
{
    if (tc == sTickCount || tc < 0)
        return;

    sTickCount = tc;
    update();
}

void Slider::setHandleHeight(int h)
{
    if (h < 2)
        return;

    sHandleHeight = h;
    sHandle->resize(width(), h);

    moveHandle();
}

void Slider::setEnableMousePress(bool mp)
{
    sMousePress = mp;
}

void Slider::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton)
        return;

    emit mouseDoubleClicked();
}

void Slider::wheelEvent(QWheelEvent *event)
{
    int lv = sLv;
    if (event->delta() > 0) { // up
        lv += sChangStep;
    }
    else {
        lv -= sChangStep;
    }

    if (lv > sMaxLv)
        lv = sMaxLv;
    else if (lv < sMinLv)
        lv = sMinLv;

    if (lv == sLv)
        return;

    setLevel(lv);

    emit userLevelChanged(level());
    emit userLevelChanged(QString::number(level()));

    event->accept();
}

void Slider::mousePressEvent(QMouseEvent *event)
{
    if (!sMousePress)
        return;

    if (event->button() != Qt::LeftButton)
        return;

    int abslv = abs(sMaxLv-sMinLv);

    int lv = abslv - (abslv * event->y() / height());
    lv = lv + sMinLv;

    if (lv > sMaxLv)
        lv = sMaxLv;
    else if (lv < sMinLv)
        lv = sMinLv;

    if (lv == sLv)
        return;

    setLevel(lv);

    emit userLevelChanged(level());
    emit userLevelChanged(QString::number(level()));
}

void Slider::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() != Qt::LeftButton)
        return;

    int abslv = abs(sMaxLv-sMinLv);

    int lv = abslv - (abslv * event->y() / height());
    lv = lv + sMinLv;

    if (lv > sMaxLv)
        lv = sMaxLv;
    else if (lv < sMinLv)
        lv = sMinLv;

    if (lv == sLv)
        return;

    setLevel(lv);

    emit userLevelChanged(level());
    emit userLevelChanged(QString::number(level()));
}

void Slider::resizeEvent(QResizeEvent *event)
{
    sHandle->resize(event->size().width(), sHandleHeight);
    moveHandle();
}

void Slider::paintEvent(QPaintEvent *event)
{
    QPainter p(this);

    p.setPen(QPen(palette().color(QPalette::Mid), 1));

    qreal distance = (height() - 1) / (sTickCount - 1);

    qreal xL1 = 2.0;
    qreal xL2 = (width() / 2.0) - 5.0;

    qreal xR1 = width() - xL2;
    qreal xR2 = width() - xL1;

    for (int i=0; i<sTickCount; i++) {

        qreal y = i*distance;
        QPointF pointL1(xL1, y);
        QPointF pointL2(xL2, y);

        p.drawLine(pointL1, pointL2);

        QPointF pointR1(xR1, y);
        QPointF pointR2(xR2, y);

        p.drawLine(pointR1, pointR2);
    }

    p.setPen(QPen(palette().color(QPalette::Mid), 3));

    qreal x = xL2 + 5;
    p.drawLine(QPointF(x, 0), QPointF(x, height()));

    p.end();
}

void Slider::moveHandle()
{
    int abslv = abs(sMaxLv-sMinLv);
    int maxHeight = height() - sHandleHeight;

    int absCurrentLv =  abs(sMinLv-sLv);

    sHandle->move(0, maxHeight * (abslv - absCurrentLv) / abslv);
}

