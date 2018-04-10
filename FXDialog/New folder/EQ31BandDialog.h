#ifndef EQ31BANDDIALOG_H
#define EQ31BANDDIALOG_H


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
class EQ31BandDialog;
}

class EQ31BandDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EQ31BandDialog(QWidget *parent = 0, QList<HFX> fxs = QList<HFX>());
    ~EQ31BandDialog();

private slots:
    void setGian(int sliderIndex);
    void resetGian(int sliderIndex);

    void on_btnReset_clicked();

private:
    Ui::EQ31BandDialog *ui;

    QList<HFX> fxs;

    QSignalMapper *signalLevelMapper;
    QSignalMapper *signalMouseDoubleClick;

    QMap<EQFreq31Range, QLabel*> lbdbMap;
    QMap<EQFreq31Range, Slider*> sliderMap;

    void mapLabels();
    void mapSliders();
};

#endif // EQUALIZER31BAND_H
