#ifndef SPEAKERDIALOG_H
#define SPEAKERDIALOG_H

#include <QDialog>
#include <QSignalMapper>

#include "MainWindow.h"
#include "Widgets/InstCh.h"

namespace Ui {
class SpeakerDialog;
}

class SpeakerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SpeakerDialog(QWidget *parent = 0, QMap<InstrumentType, InstCh*> *chInstMap = 0, MainWindow *mainWindow = 0);
    ~SpeakerDialog();

private slots:
    void on_btnClose_clicked();
    void showSpeakerMenu(const QPoint &pos);
    void setSpeaker(int number);

private:
    Ui::SpeakerDialog *ui;

    QMap<InstrumentType, InstCh*> *chInstMap;
    MainWindow *mainWindow;
    MidiPlayer *player;
    MidiSynthesizer *synth;

    QStringList deviceNames;
};

#endif // SPEAKERDIALOG_H
