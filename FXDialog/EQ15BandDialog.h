#ifndef EQ15BANDDIALOG_H
#define EQ15BANDDIALOG_H

#include "BASSFX/Equalizer15BandFX.h"
#include "Widgets/Slider.h"

#include <QDialog>
#include <QLabel>
#include <QMap>
#include <QSignalMapper>

namespace Ui {
class EQ15BandDialog;
}

class EQ15BandDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EQ15BandDialog(QWidget *parent = 0, Equalizer15BandFX *fx = nullptr);
    ~EQ15BandDialog();

public slots:
    void setGian(int sliderIndex);
    void resetGian(int sliderIndex);

private slots:
    void on_btnReset_clicked();

private:
    Ui::EQ15BandDialog *ui;

    Equalizer15BandFX *fx;

    QSignalMapper *signalLevelMapper;
    QSignalMapper *signalMouseDoubleClick;

    QMap<EQFrequency15Range, QLabel*> lbdbMap;
    QMap<EQFrequency15Range, Slider*> sliderMap;

    void mapLabels();
    void mapSliders();
};

#endif // EQ15BANDDIALOG_H
