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
    this->drumMap = player->midiSynthesizer()->getDrumMapSfIndex();

    QStringList sfNames;
    for (QString sn : player->midiSynthesizer()->soundfontFiles()) {
        sn = sn.replace(0, sn.lastIndexOf("/")+1, "");
        sfNames.append(sn);
    }

    QStringList instNames = MidiHelper::GMInstrumentNumberNames();

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

    // drum
    for (int i=0; i<16; i++) {
        ComboBoxItem *cbItem = new ComboBoxItem(this, i);
        cbItem->addItems(sfNames);
        cbItem->setCurrentIndex(drumMap.at(i));
        connect(cbItem, SIGNAL(userActivated(int,int)), this, SLOT(onComboBoxDrumActivated(int,int)));
        comboBoxDrum.append(cbItem);

        QTreeWidgetItem *item = ui->treeDrum->topLevelItem(i);
        ui->treeDrum->setItemWidget(item, 1, cbItem);
    }

    for (int col = 0; col < 2; ++col)
        ui->treeWidget->resizeColumnToContents(0);

    for (int col = 0; col < 2; ++col)
        ui->treeDrum->resizeColumnToContents(0);
}

MapSoundfontDialog::~MapSoundfontDialog()
{
    instMap.clear();
    drumMap.clear();

    for (ComboBoxItem *cb : comboBoxDrum) {
        delete cb;
    }
    comboBoxDrum.clear();

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

void MapSoundfontDialog::onComboBoxDrumActivated(int row, int index)
{
    drumMap[row] = index;
}

void MapSoundfontDialog::on_btnOk_clicked()
{
    player->midiSynthesizer()->setMapSoundfontIndex(instMap, drumMap);

    QSettings settings;

    settings.beginWriteArray("SynthSoundfontsMap");
    for (int i=0; i<128; i++) {
        settings.setArrayIndex(i);
        settings.setValue("mapTo", instMap.at(i));
    }
    settings.endArray();

    settings.beginWriteArray("SynthSoundfontsDrumMap");
    for (int i=0; i<16; i++) {
        settings.setArrayIndex(i);
        settings.setValue("mapTo", drumMap.at(i));
    }
    settings.endArray();

    close();
}

void MapSoundfontDialog::on_btnCancel_clicked()
{
    close();
}
