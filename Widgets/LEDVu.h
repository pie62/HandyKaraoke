#ifndef LEDVU_H
#define LEDVU_H

#include <QFrame>
#include <QTimer>
#include <QPropertyAnimation>

class LEDVu : public QFrame
{
    Q_OBJECT
    Q_PROPERTY(int level READ level WRITE setLevel NOTIFY levelChanged)

public:
    explicit LEDVu(QWidget *parent = 0);
    ~LEDVu();

    QColor backgroundColor()    { return _bgColor; }
    QColor ledColorOn1()        { return _ledColorOn1; }
    QColor ledColorOn2()        { return _ledColorOn2; }
    QColor ledColorOn3()        { return _ledColorOn3; }
    QColor ledColorOff1()       { return _ledColorOff1; }
    QColor ledColorOff2()       { return _ledColorOff2; }
    QColor ledColorOff3()       { return _ledColorOff3; }

    void setBackGroundColor(const QColor &c);

    void setLedColorOn1(const QColor &c);
    void setLedColorOn2(const QColor &c);
    void setLedColorOn3(const QColor &c);

    void setLedColorOff1(const QColor &c);
    void setLedColorOff2(const QColor &c);
    void setLedColorOff3(const QColor &c);

    bool isShowPeakHold() { return _showPeakHold; }
    int  peakHoldMs() { return _peakHoldMs; }
    void setPeakHoldMs(int msTime);

    int level() { return _vl; }

public slots:
    void setLevel(int v);
    void setMaximumLevel(int mav);
    void setMinimumLevel(int miv);
    void peak(int v);
    void setShowPeakHold(bool s);

signals:
    void levelChanged(int v);

protected:
    void resizeEvent(QResizeEvent *event);
    void paintEvent(QPaintEvent *event);

private slots:
    void onAnimationFinised();
    void onTimerTimeout();

private:
    QPixmap _pixLedOn, _pixLedOff;
    QList<int> _pixSize;

    QSize _ledSize;
    QColor _bgColor;
    QColor _ledColorOn1, _ledColorOn2, _ledColorOn3;
    QColor _ledColorOff1, _ledColorOff2, _ledColorOff3;

    QTimer *_timer;
    QPropertyAnimation *_animation;
    int _peakUpTime = 80, _peakDownTime = 2500;
    int _peakHoldMs = 500, _peakLevel = 0;
    bool _showPeakHold = true;

    int _ledCount;
    int _ledCount1;
    int _ledCount2;
    int _ledCount3;
    int _ledPercent1 = 70;
    int _ledPercent2 = 15;
    int _ledPercent3 = 15;

    int _absVl = 100;
    int _maxVl = 100;
    int _minVl = 0;
    int _vl = 0;

    void calculateLedCount(const QSize &s);
    QList<int> createLedPixmap();
    //QPixmap createLedPixmap(int index);
};

#endif // LEDVU_H
