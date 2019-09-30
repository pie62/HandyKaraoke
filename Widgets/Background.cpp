#include "Background.h"

#include <QPainter>

Background::Background(QWidget *parent) :
    QWidget(parent),
    _color(81, 81, 81)
{

}

void Background::setBackgroundType(Background::BackgroundType t)
{
    _bgType = t;
    update();
}

void Background::setBackgroundColor(const QColor &color)
{
    _color = color;
    update();
}

void Background::setBackgroundColor(const QString &colorName)
{
    setBackgroundColor(QColor(colorName));
}

void Background::setBackgroundImage(const QString &imgFile)
{
    if (imgFile.isEmpty())
        return;

    QImage img(imgFile);
    if (img.isNull())
        return;

    _image = img;
    _imageName = imgFile;
    update();
}

void Background::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    QPainter painter(this);

    switch (_bgType) {
    case Color:
        painter.fillRect(rect(), _color);
        break;
    case Image:
        painter.drawImage(rect(), _image);
        break;
    default:
        break;
    }
}
