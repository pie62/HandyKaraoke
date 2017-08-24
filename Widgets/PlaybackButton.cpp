#include "PlaybackButton.h"

PlaybackButton::PlaybackButton(QWidget *parent) : QToolButton(parent)
{

}

void PlaybackButton::setIconFiles(const QString &icoOne, const QString &icoTwo)
{
    icons[0] = icoOne;
    icons[1] = icoTwo;

    setIcon(QIcon(icoOne));
}

void PlaybackButton::enterEvent(QEvent *)
{
    setIcon(QIcon(icons[1]));
}

void PlaybackButton::leaveEvent(QEvent *)
{
    setIcon(QIcon(icons[0]));
}
