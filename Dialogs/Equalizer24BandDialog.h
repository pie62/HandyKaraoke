#ifndef EQUALIZER24BANDDIALOG_H
#define EQUALIZER24BANDDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QMap>
#include <QSignalMapper>

#include "Widgets/Slider.h"

#include "BASSFX/Equalizer24BandFX.h"

namespace Ui {
class Equalizer24BandDialog;
}

class Equalizer24BandDialog : public QDialog
{
    Q_OBJECT

public:
    explicit Equalizer24BandDialog(QWidget *parent = 0, Equalizer24BandFX *eqfx = nullptr);
    ~Equalizer24BandDialog();

signals:
    void switchChanged(bool s);

private slots:
    void mapLabelDb();
    void mapSlider();

    void onBtnSwitchUserSwitchChanged(bool switchOn);
    void onSliderLevelChanged(int sliderIndex);
    void onSliderUserLevelChanged(int sliderIndex);
    void onSliderMouseDoubleClicked(int sliderIndex);

    void on_btnReset_clicked();

private:
    Ui::Equalizer24BandDialog *ui;

    Equalizer24BandFX *eqfx;

    QMap<EQFrequency24Range, QLabel*> lbdbMap;
    QMap<EQFrequency24Range, Slider*> sliderMap;

    QSignalMapper *signalLevelMapper;
    QSignalMapper *signalUserLevelMapper;
    QSignalMapper *signalMouseDoubleClick;
};

#endif // EQUALIZER24BANDDIALOG_H
