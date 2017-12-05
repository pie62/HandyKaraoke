#include "BusDialog.h"
#include "ui_BusDialog.h"

#include <QMenu>
#include <QSettings>


BusDialog::BusDialog(QWidget *parent, QMap<InstrumentType, InstCh *> *chInstMap, MidiSynthesizer *synth) :
    QDialog(parent),
    busSignalMapper(this),
    ui(new Ui::BusDialog)
{
    ui->setupUi(this);

    this->chInstMap = chInstMap;
    this->synth = synth;

    // Tab 1 --------------------------------------------------------
    {
        int i = 0;
        for (InstrumentType t : chInstMap->keys()) {
            if (t >= InstrumentType::BusGroup1)
                break;

            QTableWidgetItem *nameItem = new QTableWidgetItem(chInstMap->value(t)->fullInstrumentName());
            QTableWidgetItem *busItem;

            QList<QString> names = busNames();
            int bus = synth->instrument(t).bus;
            if (i == -1) {
                busItem  = new QTableWidgetItem(names[0]);
            } else {
                busItem  = new QTableWidgetItem(names[bus + 1]);
            }

            ui->tableInst->insertRow(i);
            ui->tableInst->setItem(i, 0, nameItem);
            ui->tableInst->setItem(i, 1, busItem);
            i++;
        }

        ui->tableInst->setEditTriggers(QAbstractItemView::NoEditTriggers);
        QHeaderView *header = ui->tableInst->horizontalHeader();
        header->setSectionResizeMode(QHeaderView::Stretch);

        connect(&busSignalMapper, SIGNAL(mapped(int)), this, SLOT(setBus(int)));
        connect(ui->tableInst, SIGNAL(customContextMenuRequested(QPoint)),
                this, SLOT(showBusMenu(QPoint)));
    }
    // ----------------------------------------------------------------------------
    // Tab 2 -------------------------------------------------------------
    {
        int start = static_cast<int>(InstrumentType::BusGroup1);
        for (int i=0; i<16; i++) {
            InstrumentType type = static_cast<InstrumentType>(start + i);
            QString n = chInstMap->value(type)->instrumentName();

            QTableWidgetItem *item1 = new QTableWidgetItem(busNames()[i+1]);
            QTableWidgetItem *item2 = new QTableWidgetItem(n);
            ui->tableBusName->insertRow(i);
            ui->tableBusName->setItem(i, 0, item1);
            ui->tableBusName->setItem(i, 1, item2);
        }

        QHeaderView *header = ui->tableBusName->horizontalHeader();
        header->setSectionResizeMode(QHeaderView::Stretch);

        connect(ui->tableBusName, SIGNAL(cellChanged(int,int)),
                this, SLOT(setBusName(int,int)));
    }
    // ---------------------------------------------------------------
}

BusDialog::~BusDialog()
{
    delete ui;
}

void BusDialog::on_btnClose_clicked()
{
    this->close();
}

void BusDialog::showBusMenu(const QPoint &pos)
{
    QMenu menu(this);
    QMenu busMenu("Bus Group", this);

    int i = -1;
    for (const QString n : busNames()) {
        QAction *act = busMenu.addAction(n);
        connect(act, SIGNAL(triggered()), &busSignalMapper, SLOT(map()));
        busSignalMapper.setMapping(act, i);
        i++;
    }

    menu.addMenu(&busMenu);
    menu.exec(ui->tableInst->mapToGlobal(pos));
}

void BusDialog::setBus(int b)
{
    QModelIndexList indexList = ui->tableInst->selectionModel()->selectedRows();
    for (QModelIndex index : indexList) {
        QTableWidgetItem *item = ui->tableInst->item(index.row(), 1);
        item->setText(busNames()[b+1]);
        InstrumentType type = static_cast<InstrumentType>(index.row());
        synth->setBusGroup(type, b);
    }
}

void BusDialog::setBusName(int row, int col)
{
    int start = static_cast<int>(InstrumentType::BusGroup1);
    InstrumentType type = static_cast<InstrumentType>(start + row);
    QTableWidgetItem *item = ui->tableBusName->item(row, col);
    if (col == 0) {
        chInstMap->value(type)->setFullInstrumentName(item->text());
    } else {
        chInstMap->value(type)->setInstrumentName(item->text());
    }

}

QList<QString> BusDialog::busNames()
{
    QList<QString> names;
    names.append("Master (Default)");
    int start = static_cast<int>(InstrumentType::BusGroup1);
    for (int i=0; i<16; i++) {
        InstrumentType type = static_cast<InstrumentType>(start + i);
        names.append(chInstMap->value(type)->fullInstrumentName());
    }
    return names;
}
