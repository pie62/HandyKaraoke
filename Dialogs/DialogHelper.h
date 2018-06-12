#ifndef DIALOGHELPER_H
#define DIALOGHELPER_H

#include <QObject>
#include <QMenu>
#include <QSignalMapper>

#include "Midi/MidiHelper.h"

class DialogHelper : public QObject
{
    Q_OBJECT
public:
    explicit DialogHelper(QObject *parent);

    static QString getSpeakerName(SpeakerType sp);

    static QString getSpeakerIconName(SpeakerType sp);
    static QString getSpeakerSmallIconName(SpeakerType sp);

    static void createSpeakerMenu(QMenu *menu, QSignalMapper *sigMapper, int deviceNumber);
};

#endif // DIALOGHELPER_H
