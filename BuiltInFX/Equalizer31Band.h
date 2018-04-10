#ifndef EQUALIZER31BAND_H
#define EQUALIZER31BAND_H


#include "BASSFX/FX.h"
#include "Widgets/Slider.h"

#include <QDialog>
#include <QLabel>
#include <QMap>
#include <QSignalMapper>

enum class EQFreq31Range
{
    Frequency20Hz,      Frequency25Hz,      Frequency31d5Hz,
    Frequency40Hz,      Frequency50Hz,      Frequency63Hz,
    Frequency80Hz,      Frequency100Hz,     Frequency125Hz,
    Frequency160Hz,     Frequency200Hz,     Frequency250Hz,
    Frequency315Hz,     Frequency400Hz,     Frequency500Hz,
    Frequency630Hz,     Frequency800Hz,     Frequency1000Hz,
    Frequency1250Hz,    Frequency1600Hz,    Frequency2000Hz,
    Frequency2500Hz,    Frequency3150Hz,    Frequency4000Hz,
    Frequency5000Hz,    Frequency6300Hz,    Frequency8000Hz,
    Frequency10000Hz,   Frequency12500Hz,   Frequency16000Hz,
    Frequency20000Hz
};

namespace Ui {
class Equalizer31Band;
}

class Equalizer31Band : public QDialog
{
    Q_OBJECT

public:
    explicit Equalizer31Band(QWidget *parent = 0, QList<HFX> fxs = QList<HFX>());
    ~Equalizer31Band();

private slots:
    void setGian(int sliderIndex);
    void resetGian(int sliderIndex);

    void on_btnReset_clicked();

private:
    Ui::Equalizer31Band *ui;

    QList<HFX> fxs;

    QSignalMapper *signalLevelMapper;
    QSignalMapper *signalMouseDoubleClick;

    QMap<EQFreq31Range, QLabel*> lbdbMap;
    QMap<EQFreq31Range, Slider*> sliderMap;

    void mapLabels();
    void mapSliders();
};

#endif // EQUALIZER31BAND_H
