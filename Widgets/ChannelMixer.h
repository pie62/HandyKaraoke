#ifndef CHANNELMIXER_H
#define CHANNELMIXER_H

#include <QWidget>

#include "Midi/MidiPlayer.h"
#include "ChMx.h"

class BackgroundChMx : public QFrame
{
    Q_OBJECT
public:
    explicit BackgroundChMx(QWidget *parent = 0) : QFrame(parent) {}

protected:
    void paintEvent(QPaintEvent *event);
};

namespace Ui {
class ChannelMixer;
}

class ChannelMixer : public QWidget
{
    Q_OBJECT

public:
    explicit ChannelMixer(QWidget *parent = 0);
    ~ChannelMixer();

    LEDVu* vuBar(int ch) { return chs[ch]->vuBar(); }

    void setPlayer(MidiPlayer *p);
    void peak(int ch, int value);

public slots:
    void showDeTail(int ch);
    void onPlayerLoaded();
    void onPlayerPlayingEvent(MidiEvent *e);

signals:
    void buttonCloseClicked();

private slots:
    void onChSliderValueChanged(int ch, int v);
    void onChMuteChanged(int ch, bool m);
    void onChSoloChanged(int ch, bool s);
    void onCbIntsActivated(int index);
    void onDialPanValueChanged(int value);
    void onDialReverbValueChanged(int value);
    void onDialChorusValueChanged(int value);

    void setPanToolTip(int value);

    void onChbMuteVoiceToggled(bool checked);

    void onBtnSettingVuClicked();
    void onBtnCloseClicked();

private:
    Ui::ChannelMixer *ui;

    MidiPlayer *player;
    QList<ChMx*> chs;
};

#endif // CHANNELMIXER_H
