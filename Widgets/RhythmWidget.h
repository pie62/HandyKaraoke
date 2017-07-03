#ifndef RHYTHMWIDGET_H
#define RHYTHMWIDGET_H

#include <QWidget>
#include <QFrame>

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
    int beatInBar() { return _beatInBar; }
    int currentBar() { return _currentBar; }
    int currentBeat() { return _currentBeat; }

public slots:
    void setBpm(int bpm);
    void setBeatCount(int bc);
    void setBeatInBar(int bb);
    void setCurrentBeat(int b);

private:
    Ui::RhythmWidget *ui;
    QList<QFrame*> beats;

    int _beatCount = 0;
    int _beatInBar = 4;
    int _currentBar = 0;
    int _currentBeat = 0;

    QString defaultStyle;
    QString beatStyle;
};

#endif // RHYTHMWIDGET_H
