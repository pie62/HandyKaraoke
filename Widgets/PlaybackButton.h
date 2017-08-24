#ifndef PLAYBACKBUTTON_H
#define PLAYBACKBUTTON_H

#include <QToolButton>

class PlaybackButton : public QToolButton
{
    Q_OBJECT
public:
    explicit PlaybackButton(QWidget *parent = nullptr);

    void setIconFiles(const QString &icoOne, const QString &icoTwo);

signals:

protected:
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);

private:
    QString icons[2];
};

#endif // PLAYBACKBUTTON_H
