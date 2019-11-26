#include "MapSoundfontDialog.h"
#include "ui_MapSoundfontDialog.h"

#include "Midi/MidiHelper.h"
#include "Config.h"

#include <QMenu>
#include <QSettings>


MapSoundfontDialog::MapSoundfontDialog(QWidget *parent, MidiSynthesizer *synth) :
    QDialog(parent),
    ui(new Ui::MapSoundfontDialog)
{
    ui->setupUi(this);

    this->synth = synth;

    for (const QString& sn : synth->soundfontFiles())
        sfNames.append(QFileInfo(sn).fileName());

    this->showSoundfontPreset(0);

    ui->tableInst->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableDrum->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    connect(ui->cbPresets, SIGNAL(activated(int)),
            this, SLOT(showSoundfontPreset(int)));
    connect(ui->tableInst, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(showInstSoundfontsMenu(QPoint)));
    connect(ui->tableDrum, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(showDrumSoundfontsMenu(QPoint)));
}

MapSoundfontDialog::~MapSoundfontDialog()
{
    delete ui;
}

void MapSoundfontDialog::showSoundfontPreset(int preset)
{
    QStringList instNames = MidiHelper::GMInstrumentNumberNames();
    for (int i=0; i<instNames.count(); i++)
    {
        int sfIndex = synth->getMapSoundfontIndex(preset)[i];

        QTableWidgetItem *item1 = new QTableWidgetItem(instNames[i]);
        QTableWidgetItem *item2 = new QTableWidgetItem();

        if (sfIndex < sfNames.count())
            item2->setText(sfNames[sfIndex]);

        if (ui->tableInst->rowCount() <= i) {
            ui->tableInst->insertRow(i);
            ui->tableInst->setItem(i, 0, item1);
        }
        ui->tableInst->setItem(i, 1, item2);
    }


    // drum
    for (int i=0; i<16; i++)
    {
        int sfIndex = synth->getDrumMapSfIndex(preset)[i];
        QTableWidgetItem *item = new QTableWidgetItem();

        if (sfIndex < sfNames.count())
            item->setText(sfNames[sfIndex]);

        ui->tableDrum->setItem(i, 1, item);
    }
}

void MapSoundfontDialog::showInstSoundfontsMenu(const QPoint &pos)
{
    if (sfNames.count() == 0)
        return;

    this->selectedInstTable = true;

    QMenu menu(this);
    QSignalMapper signalMapper;
    createSoundfontsListMenu(&menu, &signalMapper);
    connect(&signalMapper, SIGNAL(mapped(int)), this, SLOT(setMapSoundfontsIndex(int)));

    menu.exec(ui->tableInst->mapToGlobal(pos));
}

void MapSoundfontDialog::showDrumSoundfontsMenu(const QPoint &pos)
{
    if (sfNames.count() == 0)
        return;

    this->selectedInstTable = false;

    QMenu menu(this);
    QSignalMapper signalMapper;
    createSoundfontsListMenu(&menu, &signalMapper);
    connect(&signalMapper, SIGNAL(mapped(int)), this, SLOT(setMapSoundfontsIndex(int)));

    menu.exec(ui->tableDrum->mapToGlobal(pos));
}

void MapSoundfontDialog::setMapSoundfontsIndex(int sfIndex)
{
    int presetsIndex = ui->cbPresets->currentIndex();
    QList<int> instMap = synth->getMapSoundfontIndex(presetsIndex);
    QList<int> drumMap = synth->getDrumMapSfIndex(presetsIndex);

    QModelIndexList selection;
    if (selectedInstTable)
        selection = ui->tableInst->selectionModel()->selectedRows();
    else
        selection = ui->tableDrum->selectionModel()->selectedRows();

    for (int i=0; i<selection.count(); i++)
    {
        int row = selection.at(i).row();
        if (selectedInstTable)
        {
            instMap[row] = sfIndex;
            ui->tableInst->item(row, 1)->setText(sfNames[sfIndex]);
        }
        else
        {
            drumMap[row] = sfIndex;
            ui->tableDrum->item(row, 1)->setText(sfNames[sfIndex]);
        }
    }

    synth->setMapSoundfontIndex(presetsIndex, instMap, drumMap);

    QString sfKey = "SynthSoundfontsMap";
    QString drKey = "SynthSoundfontsDrumMap";
    if (presetsIndex > 0) {
        sfKey = sfKey + QString::number(presetsIndex);
        drKey = drKey + QString::number(presetsIndex);
    }
    QSettings settings(Config::CONFIG_APP_FILE_PATH, QSettings::IniFormat);
    settings.setValue(sfKey, QVariant::fromValue(synth->getMapSoundfontIndex(presetsIndex)));
    settings.setValue(drKey, QVariant::fromValue(synth->getDrumMapSfIndex(presetsIndex)));
}

void MapSoundfontDialog::createSoundfontsListMenu(QMenu *menu, QSignalMapper *signalMapper)
{
    for (int i=0; i<sfNames.count(); i++)
    {
        QAction *act = menu->addAction(sfNames[i]);
        connect(act, SIGNAL(triggered()), signalMapper, SLOT(map()));
        signalMapper->setMapping(act, i);
    }
}

void MapSoundfontDialog::on_btnClose_clicked()
{
    this->close();
}
