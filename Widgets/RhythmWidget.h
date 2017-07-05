#ifndef RHYTHMWIDGET_H
#define RHYTHMWIDGET_H

#include <QWidget>
#include <QMap>

class RhythmRectangle : public QWidget
{
    Q_OBJECT

public:
    explicit RhythmRectangle(QWidget *parent = 0);

    bool isOn() { return _isOn; }
    void on();
    void off();
    void setOnColor(const QColor &c);
    void setOffColor(const QColor &c);

protected:
    void paintEvent(QPaintEvent *event);

private:
    QColor _offColor, _onColor;
    bool _isOn = false;
};


namespace Ui {
class RhythmWidget;
}

class RhythmWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RhythmWidget(QWidget *parent = 0);
    ~RhythmWidget();

    int beatCount() { return _beatCount; }
    int currentBeat() { return _currentBeat; }
    int currentBar() { return _barIndex+1; }

public slots:
    void setBpm(int bpm);
    void reset();
    void setBeat(const QMap<int,int> &beatInBar, int beatCount);
    void setCurrentBeat(int b);
    void setSeekBeat(int b);

private:
    Ui::RhythmWidget *ui;
    QList<RhythmRectangle*> beats;

    int _beatCount = 0;
    int _currentBeat = 0;
    int _barCount = 0;
    int _barIndex = 0;

    int lastBeat = 0;
    int timeSingIndex = 0;
    int beatBarIndex = 0;
    int nBeatInBar = 4;
    bool changeBeatInBar = false;
    QMap<int,int> _beatInBar;

    QString defaultStyle;
    QString beatStyle;

    void offAllBeats();
    void displayBeats(int n);
};

#endif // RHYTHMWIDGET_H
