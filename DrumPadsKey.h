#ifndef DRUMPADSKEY_H
#define DRUMPADSKEY_H

#include <QKeyEvent>

class DrumPadsKey
{
public:
    DrumPadsKey() {}

    static const int bassDrum = Qt::Key_M;
    static const int snare = Qt::Key_K;
    static const int hihatClose = Qt::Key_J;
    static const int hihatOpen = Qt::Key_H;
    static const int cowbell = Qt::Key_L;
    static const int tambourine = Qt::Key_U; // 54
    static const int tom1 = Qt::Key_I;  // 50
    static const int tom2 = Qt::Key_O; // 48
    static const int crashCymbal = Qt::Key_P;

    /*
    int _fxGuitarFretNoise = Qt::Key_W;  // 64 .. all
    int _fxBreathNoise = Qt::Key_E;
    int _fxSeashore = Qt::Key_R;
    int _fxBirdTweet = Qt::Key_T;
    int _fxTelephoneRing = Qt::Key_A;
    int _fxHelicopter = Qt::Key_S;
    int _fxApplause = Qt::Key_D;
    int _fxGunshot = Qt::Key_F;
    */
};

#endif // DRUMPADSKEY_H
