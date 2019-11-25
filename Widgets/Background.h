#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <QWidget>

class Background : public QWidget
{
    Q_OBJECT
public:
    enum BackgroundType {
        Color = 0,
        Image,
        Video
    };

    explicit Background(QWidget *parent = nullptr);

    BackgroundType backgroundType() { return _bgType; }
    void setBackgroundType(BackgroundType t);

    QColor backgroundColor() { return _color; }
    void setBackgroundColor(const QColor &color);
    void setBackgroundColor(const QString &colorName);

    QString backgroundImage() { return _imageName; }
    void setBackgroundImage(const QString &imgFile);

protected:
    void paintEvent(QPaintEvent *event);

private:
    BackgroundType _bgType = Color;
    QColor _color;
    QImage _image;
    QString _imageName;
};

#endif // BACKGROUND_H
