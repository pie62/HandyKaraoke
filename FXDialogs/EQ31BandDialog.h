#ifndef EQ31BANDDIALOG_H
#define EQ31BANDDIALOG_H

#include "BASSFX/Equalizer31BandFX.h"
#include "Widgets/Slider.h"

#include <QDialog>
#include <QLabel>
#include <QMap>
#include <QSignalMapper>

namespace Ui {
class EQ31BandDialog;
}

class EQ31BandDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EQ31BandDialog(QWidget *parent = 0, Equalizer31BandFX *fx = nullptr);
    ~EQ31BandDialog();

public slots:
    void setGian(int sliderIndex);
    void resetGian(int sliderIndex);

private slots:
    void on_btnReset_clicked();

private:
    Ui::EQ31BandDialog *ui;

    Equalizer31BandFX *fx;

    QSignalMapper *signalLevelMapper;
    QSignalMapper *signalMouseDoubleClick;

    QMap<EQFrequency31Range, QLabel*> lbdbMap;
    QMap<EQFrequency31Range, Slider*> sliderMap;

    void mapLabels();
    void mapSliders();
};

#endif // EQ31BANDDIALOG_H
