#include "DialogHelper.h"

DialogHelper::DialogHelper(QObject *parent) : QObject(parent)
{
}

QString DialogHelper::getSpeakerName(SpeakerType sp)
{
    switch (sp)
    {
    case SpeakerType::Default:
        return "Default Speaker";

    case SpeakerType::FrontStereo:
        return "Stereo Front (FL+FR)";
    case SpeakerType::FrontLeft:
        return "Mono Front Left (FL)";
    case SpeakerType::FrontRight:
        return "Mono Front Right (FR)";

    case SpeakerType::RearStereo:
        return "Stereo Rear (RL+RR)";
    case SpeakerType::RearLeft:
        return "Mono Rear Left (RL)";
    case SpeakerType::RearRight:
        return "Mono Rear Right (RR)";

    case SpeakerType::CenterStereo:
        return "Stereo Center/Sub (FC+SW)";
    case SpeakerType::CenterMono:
        return "Mono Center (FC)";
    case SpeakerType::SubwooferMono:
        return "Mono Subwoofer (SW)";

    case SpeakerType::SideStereo:
        return "Stereo Side (SL+SR)";
    case SpeakerType::SideLeft:
        return "Mono Side Left (SL)";
    case SpeakerType::SideRight:
        return "Mono Side Right (SR)";
    }
}

QString DialogHelper::getSpeakerIconName(SpeakerType sp)
{
    switch (sp) {

    case SpeakerType::Default:
        return ":/Icons/SoundCard/ports/port-front.png";

    case SpeakerType::FrontStereo:
        return ":/Icons/SoundCard/ports/port-front.png";
    case SpeakerType::FrontLeft:
        return ":/Icons/SoundCard/ports/port-front-left.png";
    case SpeakerType::FrontRight:
        return ":/Icons/SoundCard/ports/port-front-right.png";

    case SpeakerType::RearStereo:
        return ":/Icons/SoundCard/ports/port-rear.png";
    case SpeakerType::RearLeft:
        return ":/Icons/SoundCard/ports/port-rear-left.png";
    case SpeakerType::RearRight:
        return ":/Icons/SoundCard/ports/port-rear-right.png";

    case SpeakerType::CenterStereo:
        return ":/Icons/SoundCard/ports/port-center.png";
    case SpeakerType::CenterMono:
        return ":/Icons/SoundCard/ports/port-center-left.png";
    case SpeakerType::SubwooferMono:
        return ":/Icons/SoundCard/ports/port-center-right.png";

    case SpeakerType::SideStereo:
        return ":/Icons/SoundCard/ports/port-side.png";
    case SpeakerType::SideLeft:
        return ":/Icons/SoundCard/ports/port-side-left.png";
    case SpeakerType::SideRight:
        return ":/Icons/SoundCard/ports/port-side-right.png";
    }
}

QString DialogHelper::getSpeakerSmallIconName(SpeakerType sp)
{
    switch (sp) {

    case SpeakerType::Default:
        return ":/Icons/SoundCard/ports-small/port-sm-front.png";

    case SpeakerType::FrontStereo:
        return ":/Icons/SoundCard/ports-small/port-sm-front.png";
    case SpeakerType::FrontLeft:
        return ":/Icons/SoundCard/ports-small/port-sm-front-left.png";
    case SpeakerType::FrontRight:
        return ":/Icons/SoundCard/ports-small/port-sm-front-right.png";

    case SpeakerType::RearStereo:
        return ":/Icons/SoundCard/ports-small/port-sm-rear.png";
    case SpeakerType::RearLeft:
        return ":/Icons/SoundCard/ports-small/port-sm-rear-left.png";
    case SpeakerType::RearRight:
        return ":/Icons/SoundCard/ports-small/port-sm-rear-right.png";

    case SpeakerType::CenterStereo:
        return ":/Icons/SoundCard/ports-small/port-sm-center.png";
    case SpeakerType::CenterMono:
        return ":/Icons/SoundCard/ports-small/port-sm-center-left.png";
    case SpeakerType::SubwooferMono:
        return ":/Icons/SoundCard/ports-small/port-sm-center-right.png";

    case SpeakerType::SideStereo:
        return ":/Icons/SoundCard/ports-small/port-sm-side.png";
    case SpeakerType::SideLeft:
        return ":/Icons/SoundCard/ports-small/port-sm-side-left.png";
    case SpeakerType::SideRight:
        return ":/Icons/SoundCard/ports-small/port-sm-side-right.png";
    }
}

void DialogHelper::createSpeakerMenu(QMenu *menu, QSignalMapper *sigMapper, int deviceNumber)
{
    if (deviceNumber < 0)
        return;

    menu->clear();

    QAction *act;
    QIcon icon;

    // front
    icon.addFile(getSpeakerSmallIconName(SpeakerType::FrontStereo));
    act = menu->addAction(icon, getSpeakerName(SpeakerType::FrontStereo));
    connect(act, SIGNAL(triggered()), sigMapper, SLOT(map()));
    sigMapper->setMapping(act, (deviceNumber | 1 << 16));

    icon.addFile(getSpeakerSmallIconName(SpeakerType::FrontLeft));
    act = menu->addAction(icon, getSpeakerName(SpeakerType::FrontLeft));
    connect(act, SIGNAL(triggered()), sigMapper, SLOT(map()));
    sigMapper->setMapping(act, (deviceNumber | 2 << 16));

    icon.addFile(getSpeakerSmallIconName(SpeakerType::FrontRight));
    act = menu->addAction(icon, getSpeakerName(SpeakerType::FrontRight));
    connect(act, SIGNAL(triggered()), sigMapper, SLOT(map()));
    sigMapper->setMapping(act, (deviceNumber | 3 << 16));
    // ------------------------------------------------------------------------

    menu->addSeparator();

    // rear
    icon.addFile(getSpeakerSmallIconName(SpeakerType::RearStereo));
    act = menu->addAction(icon, getSpeakerName(SpeakerType::RearStereo));
    connect(act, SIGNAL(triggered()), sigMapper, SLOT(map()));
    sigMapper->setMapping(act, (deviceNumber | 4 << 16));

    icon.addFile(getSpeakerSmallIconName(SpeakerType::RearLeft));
    act = menu->addAction(icon, getSpeakerName(SpeakerType::RearLeft));
    connect(act, SIGNAL(triggered()), sigMapper, SLOT(map()));
    sigMapper->setMapping(act, (deviceNumber | 5 << 16));

    icon.addFile(getSpeakerSmallIconName(SpeakerType::RearRight));
    act = menu->addAction(icon, getSpeakerName(SpeakerType::RearRight));
    connect(act, SIGNAL(triggered()), sigMapper, SLOT(map()));
    sigMapper->setMapping(act, (deviceNumber | 6 << 16));
    // ------------------------------------------------------------------------

    menu->addSeparator();

    // center
    icon.addFile(getSpeakerSmallIconName(SpeakerType::CenterStereo));
    act = menu->addAction(icon, getSpeakerName(SpeakerType::CenterStereo));
    connect(act, SIGNAL(triggered()), sigMapper, SLOT(map()));
    sigMapper->setMapping(act, (deviceNumber | 7 << 16));

    icon.addFile(getSpeakerSmallIconName(SpeakerType::CenterMono));
    act = menu->addAction(icon, getSpeakerName(SpeakerType::CenterMono));
    connect(act, SIGNAL(triggered()), sigMapper, SLOT(map()));
    sigMapper->setMapping(act, (deviceNumber | 8 << 16));

    icon.addFile(getSpeakerSmallIconName(SpeakerType::SubwooferMono));
    act = menu->addAction(icon, getSpeakerName(SpeakerType::SubwooferMono));
    connect(act, SIGNAL(triggered()), sigMapper, SLOT(map()));
    sigMapper->setMapping(act, (deviceNumber | 9 << 16));
    // ------------------------------------------------------------------------

    menu->addSeparator();

    // side
    icon.addFile(getSpeakerSmallIconName(SpeakerType::SideStereo));
    act = menu->addAction(icon, getSpeakerName(SpeakerType::SideStereo));
    connect(act, SIGNAL(triggered()), sigMapper, SLOT(map()));
    sigMapper->setMapping(act, (deviceNumber | 10 << 16));

    icon.addFile(getSpeakerSmallIconName(SpeakerType::SideLeft));
    act = menu->addAction(icon, getSpeakerName(SpeakerType::SideLeft));
    connect(act, SIGNAL(triggered()), sigMapper, SLOT(map()));
    sigMapper->setMapping(act, (deviceNumber | 11 << 16));

    icon.addFile(getSpeakerSmallIconName(SpeakerType::SideRight));
    act = menu->addAction(icon, getSpeakerName(SpeakerType::SideRight));
    connect(act, SIGNAL(triggered()), sigMapper, SLOT(map()));
    sigMapper->setMapping(act, (deviceNumber | 12 << 16));
}
