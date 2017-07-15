#include "SwitchButton.h"

#include <QPainter>
#include <QMouseEvent>


SwitchButton::SwitchButton(QWidget *parent) : QWidget(parent)
{
    animation = new QPropertyAnimation(this, "offset");
    animation->setDuration(120);

    QFont f = font();
    f.setBold(true);

    setFont(f);
}

SwitchButton::~SwitchButton()
{
    delete animation;
}

void SwitchButton::setOffset(int ost)
{
    _x = ost;
    update();
}

void SwitchButton::on()
{
    if (_on)
        return;

    _on = true;
    animation->setStartValue(_x);
    animation->setEndValue(width() - height()+2);
    animation->start();

    emit switchChanged(true);
}

void SwitchButton::off()
{
    if (!_on)
        return;

    _on = false;
    animation->setStartValue(_x);
    animation->setEndValue(2);
    animation->start();

    emit switchChanged(false);
}

void SwitchButton::setOnText(const QString &ot)
{
    _onText = ot;
    update();
}

void SwitchButton::setOffText(const QString &ot)
{
    _offText = ot;
    update();
}

void SwitchButton::enterEvent(QEvent *event)
{
    setCursor(Qt::PointingHandCursor);
    QWidget::enterEvent(event);
}

void SwitchButton::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton)
        return;

    if (_on)
        off();
    else
        on();

    emit userSwitchChanged(_on);
    QWidget::mouseReleaseEvent(event);
}

void SwitchButton::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    QPen pen(palette().color(QPalette::Shadow), 1);
    p.setPen(pen);

    if (_on)
    {
        p.setBrush(palette().color(QPalette::Highlight));
        p.drawRoundedRect(rect(), height()/2.0, height()/2.0);

        qreal tx = (width() - height() + 4 - fontMetrics().width(_onText)) / 2.0;
        qreal ty = (height() + font().pointSize()) / 2.0;

        p.setPen(QPen(palette().color(QPalette::Light), 1));
        p.drawText(tx, ty, _onText);
    }
    else
    {
        p.setBrush(palette().color(QPalette::Midlight));
        p.drawRoundedRect(rect(), height()/2.0, height()/2.0);

        //qreal tx = (width() - height() - 2 + fontMetrics().width(_offText)) / 2.0;
        qreal tx = (width() + height() - 4 - fontMetrics().width(_offText)) / 2.0;
        qreal ty = (height() + font().pointSize()) / 2.0;

        p.setPen(QPen(palette().color(QPalette::Mid), 1));
        p.drawText(tx, ty, _offText);
    }

    p.setPen(pen);
    p.setBrush(palette().color(QPalette::Button));
    p.drawEllipse(_x, 2, height()-4, height()-4);

    p.end();
}
