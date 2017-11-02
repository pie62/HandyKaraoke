#ifndef MAPSOUNDFONTDIALOG_H
#define MAPSOUNDFONTDIALOG_H

#include <QDialog>
#include <QTreeWidgetItem>

#include "ComboBoxItem.h"
#include "Midi/MidiPlayer.h"

namespace Ui {
class MapSoundfontDialog;
}

class MapSoundfontDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MapSoundfontDialog(QWidget *parent = 0, MidiPlayer *player = nullptr);
    ~MapSoundfontDialog();

private slots:
    void onComboBoxItemsActivated(int row, int index);
    void onComboBoxDrumActivated(int row, int index);

    void on_btnOk_clicked();

    void on_btnCancel_clicked();

private:
    Ui::MapSoundfontDialog *ui;

    QList<QTreeWidgetItem*> treeWidgetItems;
    QList<ComboBoxItem*> comboBoxItems;
    QList<ComboBoxItem*> comboBoxDrum;

    QList<int> instMap;
    QList<int> drumMap;

    MidiPlayer *player = nullptr;
};

#endif // MAPSOUNDFONTDIALOG_H
