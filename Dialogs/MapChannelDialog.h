#ifndef MAPCHANNELDIALOG_H
#define MAPCHANNELDIALOG_H

#include <QDialog>
#include <QSignalMapper>

class MidiPlayer;

namespace Ui {
class MapChannelDialog;
}

class MapChannelDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MapChannelDialog(QWidget *parent = 0, MidiPlayer *player = nullptr);
    ~MapChannelDialog();

private slots:
    void on_tableWidget_customContextMenuRequested(const QPoint &pos);
    void setMapMidiOut(int port);

    void on_btnClose_clicked();

private:
    Ui::MapChannelDialog *ui;

    MidiPlayer *player;
    QSignalMapper *signalMapper = nullptr;
    QString synthStr = "Midi Synthesizer (SoundFont)";
    std::vector<std::string> devices;
};

#endif // MAPCHANNELDIALOG_H
