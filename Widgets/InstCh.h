#ifndef INSTCH_H
#define INSTCH_H

#include <QWidget>

#include "Widgets/LEDVu.h"
#include "Midi/MidiSynthesizer.h"


namespace Ui {
class InstCh;
}

class InstCh : public QWidget
{
    Q_OBJECT

public:
    explicit InstCh(QWidget *parent = 0);
    ~InstCh();

    LEDVu* vuBar();

public slots:
    void setInstrumentType(InstrumentType t);
    void setInstrumentName(const QString &name, const QString &tooltip = QString());
    void setInstrumentImage(const QImage &img);
    void setMuteButton(bool m);
    void setSoloButton(bool s);
    void setSliderLevel(int v);
    void peak(int v);

signals:
    void muteChanged(InstrumentType type, bool m);
    void soloChanged(InstrumentType type, bool s);
    void sliderLevelChanged(InstrumentType type, int v);
    void sliderDoubleClicked(InstrumentType type);

private slots:
    void onBtnMuteClicked();
    void onBtnSoloClicked();
    void onSliderUserLevelChanged(int v);
    void onSliderDoubleClicked();

private:
    Ui::InstCh *ui;

    InstrumentType instType;
    bool mute = false;
    bool solo = false;

    QString btnDefault;
    QString btnMuteStyle;
    QString btnSoloStyle;
};

#endif // INSTCH_H
