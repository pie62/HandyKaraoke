#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <QWidget>

enum class BackgroundType {
    Color,
    Image,
    Video
};

class Background : public QWidget
{
    Q_OBJECT

public:
    explicit Background(QWidget *parent = nullptr);
    ~Background();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    void scalePixmap(QSize parentZize);

private:
    BackgroundType  _type = BackgroundType::Color;

    QColor          _color = Qt::black;

    QString         _imgFile;
    QPixmap         *_imgPixmap = nullptr;

};

#endif // BACKGROUND_H
