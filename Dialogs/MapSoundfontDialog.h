#ifndef MAPSOUNDFONTDIALOG_H
#define MAPSOUNDFONTDIALOG_H

#include <QDialog>
#include <QTableWidget>
#include <QSignalMapper>

#include "Midi/MidiPlayer.h"

namespace Ui {
class MapSoundfontDialog;
}

class MapSoundfontDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MapSoundfontDialog(QWidget *parent = 0, MidiSynthesizer *synth = nullptr);
    ~MapSoundfontDialog();

private slots:
    void showInstSoundfontsMenu(const QPoint& pos);
    void showDrumSoundfontsMenu(const QPoint& pos);
    void setMapSoundfontsIndex(int sfIndex);

    void on_btnClose_clicked();

private:
    void createSoundfontsListMenu(QMenu *menu, QSignalMapper *signalMapper);

private:
    Ui::MapSoundfontDialog *ui;

    // true = instruments table, false = drum table
    bool selectedInstTable = true;

    MidiSynthesizer *synth = nullptr;
    QStringList sfNames;
};

#endif // MAPSOUNDFONTDIALOG_H
