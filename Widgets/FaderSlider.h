#ifndef FADERSLIDER_H
#define FADERSLIDER_H

#include <QFrame>
#include <QLabel>

class FaderSlider : public QFrame
{
    Q_OBJECT
public:
    explicit FaderSlider(QWidget *parent = nullptr);
    ~FaderSlider();

    int level() { return sLv; }
    int minimumLevel() { return sMinLv; }
    int maximumLevel() { return sMaxLv; }
    int changeStep() { return sChangStep; }
    int tickCount() { return sTickCount; }
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
    QLabel *sHandle;

    qreal sTickCount = 20;

    bool sMousePress = false;
    int sChangStep = 5;
    int sMinLv = 0;
    int sMaxLv = 100;
    int sLv = 0;
};

#endif // FADERSLIDER_H
