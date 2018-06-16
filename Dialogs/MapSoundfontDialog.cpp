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

    QStringList instNames = MidiHelper::GMInstrumentNumberNames();
    for (int i=0; i<instNames.count(); i++)
    {
        int sfIndex = synth->getMapSoundfontIndex()[i];

        QTableWidgetItem *item1 = new QTableWidgetItem(instNames[i]);
        QTableWidgetItem *item2 = new QTableWidgetItem();

        if (sfIndex < sfNames.count())
            item2->setText(sfNames[sfIndex]);

        ui->tableInst->insertRow(i);
        ui->tableInst->setItem(i, 0, item1);
        ui->tableInst->setItem(i, 1, item2);
    }


    // drum
    for (int i=0; i<16; i++)
    {
        int sfIndex = synth->getDrumMapSfIndex()[i];
        QTableWidgetItem *item = new QTableWidgetItem();

        if (sfIndex < sfNames.count())
            item->setText(sfNames[sfIndex]);

        ui->tableDrum->setItem(i, 1, item);
    }

    ui->tableInst->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableDrum->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    connect(ui->tableInst, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(showInstSoundfontsMenu(QPoint)));
    connect(ui->tableDrum, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(showDrumSoundfontsMenu(QPoint)));
}

MapSoundfontDialog::~MapSoundfontDialog()
{
    delete ui;
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
    QList<int> instMap = synth->getMapSoundfontIndex();
    QList<int> drumMap = synth->getDrumMapSfIndex();

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

    synth->setMapSoundfontIndex(instMap, drumMap);

    QSettings settings(CONFIG_APP_FILE_PATH, QSettings::IniFormat);
    settings.setValue("SynthSoundfontsMap", QVariant::fromValue(synth->getMapSoundfontIndex()));
    settings.setValue("SynthSoundfontsDrumMap", QVariant::fromValue(synth->getDrumMapSfIndex()));
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
