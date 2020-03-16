#include "Background.h"

#include <QResizeEvent>
#include <QPainter>

Background::Background(QWidget *parent) : QWidget(parent)
{
    _imgFile = "D:/Wallpapers/1/boat.jpg";
    _imgPixmap = new QPixmap(_imgFile);
    _type = BackgroundType::Image;
}

Background::~Background()
{
    if (_imgPixmap != nullptr)
        delete _imgPixmap;
}

void Background::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter painter(this);
    painter.setPen(Qt::NoPen);

    switch (_type) {
    case BackgroundType::Color:
        painter.setBrush(_color);
        painter.drawRect(rect());
        break;
    case BackgroundType::Image:
        painter.drawPixmap(rect(), *_imgPixmap);
        break;
    case BackgroundType::Video:

        break;
    }
}
