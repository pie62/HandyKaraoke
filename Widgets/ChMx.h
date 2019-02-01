#ifndef CHMX_H
#define CHMX_H

#include <QWidget>
#include <QLabel>
#include "LEDVu.h"

class ChLabel : public QLabel
{
    Q_OBJECT
public:
    explicit ChLabel(QWidget *parent = 0) : QLabel(parent) {}
signals:
    void mouseHover();
protected:
    void enterEvent(QEvent *event) {
        setStyleSheet("background-color: rgb(117, 117, 117); color: rgb(30,144,255);");
        emit mouseHover();
        QLabel::enterEvent(event);
    }

    void leaveEvent(QEvent *event) {
        setStyleSheet("background-color: rgb(117, 117, 117); color: rgb(238, 238, 238);");
        QLabel::leaveEvent(event);
    }
};

//=================================================

namespace Ui {
class ChMx;
}

class ChMx : public QWidget
{
    Q_OBJECT

public:
    explicit ChMx(QWidget *parent = 0);
    ~ChMx();

    LEDVu* vuBar();
    void setMuteButton(bool m);

public slots:
    void setChannelNumber(int ch);
    void setSliderValue(int v);
    void peak(int v);

signals:
    void muteChanged(int ch, bool m);
    void soloChanged(int ch, bool s);
    void sliderValueChanged(int ch, int v);
    void mouseHover(int ch);
    void mouseRightClicked(int ch, const QPoint &p);

protected:
    void mouseReleaseEvent(QMouseEvent *event);

private slots:
    void onChLabelMouseHover();
    void onBtnMuteClicked();
    void onBtnSoloClicked();
    void onSliderValueChanged(int v);

private:
    Ui::ChMx *ui;

    int chNumber = 0;
    bool mute = false;
    bool solo = false;

    QString btnDefault;
    QString btnMuteStyle;
    QString btnSoloStyle;
};

#endif // CHMX_H
