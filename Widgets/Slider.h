#ifndef SLIDER_H
#define SLIDER_H

#include <QFrame>


class SliderHandle : public QFrame
{
    Q_OBJECT
public:
    explicit SliderHandle(QWidget *parent = 0);

protected:
    void paintEvent(QPaintEvent *event);
};

// ========================================================================

class Slider : public QFrame
{
    Q_OBJECT
public:
    explicit Slider(QWidget *parent = 0);
    ~Slider();

    int level() { return sLv; }
    int minimumLevel() { return sMinLv; }
    int maximumLevel() { return sMaxLv; }
    int changeStep() { return sChangStep; }
    int tickCount() { return sTickCount; }
    int handleHeight() { return sHandleHeight; }
    bool isEnableMousePress() { return sMousePress; }

signals:
    void levelChanged(int level);
    void levelChanged(QString level);
    void userLevelChanged(int level);
    void userLevelChanged(QString level);
    void mouseDoubleClicked();

public slots:
    void setLevel(int level);
    void setMinimumLevel(int minLevel);
    void setMaximumLevel(int maxLevel);
    void setChangeStep(int cstp);
    void setTickCount(int tc);
    void setHandleHeight(int h);
    void setEnableMousePress(bool mp);

protected:
    void mouseDoubleClickEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void resizeEvent(QResizeEvent *event);
    void paintEvent(QPaintEvent *event);

private slots:
    void moveHandle();

private:
    SliderHandle *sHandle;

    qreal sTickCount = 20;
    int sHandleHeight = 12;

    bool sMousePress = false;
    int sChangStep = 5;
    int sMinLv = 0;
    int sMaxLv = 100;
    int sLv = 0;
};

#endif // SLIDER_H
