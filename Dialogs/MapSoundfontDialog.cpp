#include "MapSoundfontDialog.h"
#include "ui_MapSoundfontDialog.h"

#include "Midi/MidiHelper.h"

#include <QSettings>


MapSoundfontDialog::MapSoundfontDialog(QWidget *parent, MidiPlayer *player) :
    QDialog(parent),
    ui(new Ui::MapSoundfontDialog)
{
    ui->setupUi(this);

    this->player = player;

    this->instMap = player->midiSynthesizer()->getMapSoundfontIndex();

    QStringList sfNames;
    for (const std::string &sn : player->midiSynthesizer()->soundfontFiles()) {
        QString n = QString::fromStdString(sn);
        n = n.replace(0, n.lastIndexOf("/")+1, "");
        sfNames.append(n);
    }

    QStringList instNames = MidiHelper::GMInstrumentNumberNames();
    instNames.append("--- - Percussion (Drum)");

    int index = 0;
    for (const QString &instName : instNames) {

        QTreeWidgetItem *item = new QTreeWidgetItem();
        item->setText(0, instName);
        treeWidgetItems.append(item);

        ComboBoxItem *cbItem = new ComboBoxItem(this, index);
        cbItem->addItems(sfNames);
        cbItem->setCurrentIndex(instMap.at(index));
        connect(cbItem, SIGNAL(userActivated(int,int)), this, SLOT(onComboBoxItemsActivated(int,int)));
        comboBoxItems.append(cbItem);

        ui->treeWidget->addTopLevelItem(item);
        ui->treeWidget->setItemWidget(item, 1, cbItem);

        index++;
    }

    for (int col = 0; col < 2; ++col)
        ui->treeWidget->resizeColumnToContents(0);
}

MapSoundfontDialog::~MapSoundfontDialog()
{
    instMap.clear();

    for (ComboBoxItem *cb : comboBoxItems) {
        delete cb;
    }
    comboBoxItems.clear();

    for (QTreeWidgetItem *item : treeWidgetItems) {
        delete item;
    }
    treeWidgetItems.clear();

    delete ui;
}

void MapSoundfontDialog::onComboBoxItemsActivated(int row, int index)
{
    instMap[row] = index;
}

void MapSoundfontDialog::on_btnOk_clicked()
{
    player->midiSynthesizer()->setMapSoundfontIndex(instMap);

    QSettings settings;

    settings.beginWriteArray("SynthSoundfontsMap");
    for (int i=0; i<129; i++) {
        settings.setArrayIndex(i);
        settings.setValue("mapTo", instMap.at(i));
    }
    settings.endArray();

    close();
}

void MapSoundfontDialog::on_btnCancel_clicked()
{
    close();
}
