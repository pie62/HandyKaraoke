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
    QString fullInstrumentName();

    void addVSTLabel(const QString &label, int fxIndex, bool bypass = false);
    void removeVSTLabel(int fxIndex);

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

    void menuRequested(InstrumentType type, const QPoint &pos);
    void fxRemoveMenuRequested(InstrumentType typr, int fxIndex, const QPoint &pos);
    void fxByPassChanged(InstrumentType type, int fxIndex, bool bypass);
    void fxDoubleClicked(InstrumentType type, int fxIndex);

private slots:
    void onBtnMuteClicked();
    void onBtnSoloClicked();
    void onSliderUserLevelChanged(int v);
    void onSliderDoubleClicked();
    void contextMenuRequested(const QPoint &pos);

    void onFxMenuRequested(int fxIndex, const QPoint &pos);
    void onFxByPassChanged(int fxIndex, bool bypass);
    void onFxDoubleClicked(int fxIndex);

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
