#ifndef RHYTHMWIDGET_H
#define RHYTHMWIDGET_H

#include "Midi/MidiHelper.h"

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

    int barCount() { return _barCount; }
    int beatCount() { return _beatCount; }
    int currentBar() { return _currentBar; }
    int currentBeat() { return _currentBeat; }

public slots:
    void setBpm(int bpm);
    void reset();
    void setBeat(const QList<SignatureBeat> &signatureBeats, int beatCount);
    void setCurrentBeat(int b);

private:
    Ui::RhythmWidget *ui;
    QList<RhythmRectangle*> beats;

    int _barCount = 0;
    int _currentBar = -1;

    int _beatCount = 0;
    int _currentBeat = -1;
    int _currentBeatInBar = 4;
    int _tempIndex = 0;

    QList<SignatureBeat> _signatureBeats;

    void offAllBeats();
    void displayBeats(int n);
};

#endif // RHYTHMWIDGET_H
