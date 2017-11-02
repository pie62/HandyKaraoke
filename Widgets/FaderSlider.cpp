#include "FaderSlider.h"

#include <QResizeEvent>
#include <QPainter>

FaderSlider::FaderSlider(QWidget *parent) : QFrame(parent)
{
    sHandle = new QLabel(this);
    sHandle->setPixmap(QPixmap(":/Icons/09B (narrow).png"));
    sHandle->setScaledContents(true);
    sHandle->resize(19, 28);
}

FaderSlider::~FaderSlider()
{
    delete sHandle;
}

void FaderSlider::setLevel(int level)
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

void FaderSlider::setMinimumLevel(int minLevel)
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

void FaderSlider::setMaximumLevel(int maxLevel)
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

void FaderSlider::setChangeStep(int cstp)
{
    if (cstp < 1)
        return;

    sChangStep = cstp;
}

void FaderSlider::setTickCount(int tc)
{
    if (tc == sTickCount || tc < 0)
        return;

    sTickCount = tc;
    update();
}

void FaderSlider::setEnableMousePress(bool mp)
{
    sMousePress = mp;
}

void FaderSlider::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton)
        return;

    emit mouseDoubleClicked();
}

void FaderSlider::wheelEvent(QWheelEvent *event)
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

void FaderSlider::mousePressEvent(QMouseEvent *event)
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

void FaderSlider::mouseMoveEvent(QMouseEvent *event)
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

void FaderSlider::resizeEvent(QResizeEvent *event)
{
    moveHandle();
}

void FaderSlider::paintEvent(QPaintEvent *event)
{
    QPainter p(this);

    #ifdef _WIN32
    p.setPen(QPen(palette().color(QPalette::Shadow), 3));
    #else
    p.setPen(QPen(palette().color(QPalette::Mid), 3));
    #endif

    int x = width() / 2;
    p.drawLine(QPointF(x, 0), QPointF(x, height()));

    p.end();
}

void FaderSlider::moveHandle()
{
    int abslv = abs(sMaxLv-sMinLv);
    int maxHeight = height() - sHandle->height();

    int absCurrentLv =  abs(sMinLv-sLv);

    sHandle->move((this->width() - sHandle->width()) / 2,
                  maxHeight * (abslv - absCurrentLv) / abslv);
}
