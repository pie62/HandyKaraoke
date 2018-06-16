#ifndef EQUALIZER31BANDDIALOG_H
#define EQUALIZER31BANDDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QMap>
#include <QSignalMapper>

#include "Widgets/Slider.h"

#include "BASSFX/Equalizer31BandFX.h"

namespace Ui {
class Equalizer31BandDialog;
}

class Equalizer31BandDialog : public QDialog
{
    Q_OBJECT

public:
    explicit Equalizer31BandDialog(QWidget *parent = 0, QList<Equalizer31BandFX *> eqfxs = QList<Equalizer31BandFX *>());
    ~Equalizer31BandDialog();

    static bool isOpenned();

signals:
    void switchChanged(bool s);

protected:
    void showEvent(QShowEvent *);
    void closeEvent(QCloseEvent *);

private slots:
    void mapLabelDb();
    void mapSlider();

    void onBtnSwitchUserSwitchChanged(bool switchOn);
    void onSliderLevelChanged(int sliderIndex);
    void onSliderUserLevelChanged(int sliderIndex);
    void onSliderMouseDoubleClicked(int sliderIndex);

    void on_btnReset_clicked();

private:
    Ui::Equalizer31BandDialog *ui;

    QList<Equalizer31BandFX *> eqfxs;

    QMap<EQFrequency31Range, QLabel*> lbdbMap;
    QMap<EQFrequency31Range, Slider*> sliderMap;

    QSignalMapper *signalLevelMapper;
    QSignalMapper *signalUserLevelMapper;
    QSignalMapper *signalMouseDoubleClick;

    static bool openned;
};

#endif // EQUALIZER31BANDDIALOG_H
