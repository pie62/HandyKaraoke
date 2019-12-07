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
    bool isLock() { return lock; }

    void setLock(bool lock);
    void setPlayer(MidiPlayer *p);
    void peak(int ch, int value);

    bool isMuteVoice();
    void setMuteVoice(bool mute);

public slots:
    void showDeTail(int ch);
    void onPlayerLoaded();
    void onPlayerPlayingEvent(MidiEvent e);

signals:
    void lockChanged(bool lock);
    void mouseLeaved();

protected:
    void leaveEvent(QEvent *event);

private slots:
    void onChSliderValueChanged(int ch, int v);
    void onChMuteChanged(int ch, bool m);
    void onChSoloChanged(int ch, bool s);
    void onChMouseRightClicked(int ch, const QPoint &p);
    void onCbIntsActivated(int index);
    void onDialPanValueChanged(int value);
    void onDialReverbValueChanged(int value);
    void onDialChorusValueChanged(int value);

    void setPanToolTip(int value);

    void onChbMuteVoiceToggled(bool checked);
    void onChbLockToggled(bool checked);

    void onBtnSettingVuClicked();

private:
    Ui::ChannelMixer *ui;

    MidiPlayer *player;
    QList<ChMx*> chs;

    bool lock = false;
};

#endif // CHANNELMIXER_H
