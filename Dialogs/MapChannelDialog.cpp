#include "MapChannelDialog.h"
#include "ui_MapChannelDialog.h"

#include "Midi/MidiPlayer.h"
#include <QMenu>
#include <QSettings>


MapChannelDialog::MapChannelDialog(QWidget *parent, MidiPlayer *player) :
    QDialog(parent),
    ui(new Ui::MapChannelDialog)
{
    ui->setupUi(this);

    this->player = player;
    this->devices = MidiPlayer::midiDevices();
    ui->tableWidget->setRowCount(16);

    for (int i=0; i<16; i++)
    {
        QTableWidgetItem *itemCh = new QTableWidgetItem(QString::number(i+1));
        itemCh->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->tableWidget->setItem(i, 0, itemCh);

        QTableWidgetItem *itemPort = new QTableWidgetItem();
        int port = player->midiChannel()[i].port();
        if (port == -1) {
            itemPort->setText(synthStr);
        } else {
            itemPort->setText(QString::fromStdString(devices[port]));
        }
        ui->tableWidget->setItem(i, 1, itemPort);
    }
}

MapChannelDialog::~MapChannelDialog()
{
    if (signalMapper != nullptr)
        delete signalMapper;

    delete ui;
}

void MapChannelDialog::on_tableWidget_customContextMenuRequested(const QPoint &pos)
{
    QMenu menu;

    if (signalMapper != nullptr) {
        delete signalMapper;
        signalMapper = nullptr;
    }
    signalMapper = new QSignalMapper(this);
    connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(setMapMidiOut(int)));

    for (int i=0; i<devices.size(); i++) {
        QString portName = QString::fromStdString(devices[i]);
        QAction *action = menu.addAction(portName);
        connect(action, SIGNAL(triggered()), signalMapper, SLOT(map()));
        signalMapper->setMapping(action, i);
    }

    QAction *action = menu.addAction(synthStr);
    connect(action, SIGNAL(triggered()), signalMapper, SLOT(map()));
    signalMapper->setMapping(action, -1);

    menu.exec(ui->tableWidget->mapToGlobal(pos));
}

void MapChannelDialog::setMapMidiOut(int port)
{
    QModelIndexList indexList = ui->tableWidget->selectionModel()->selectedRows();
    for (QModelIndex index : indexList) {
        player->setMapChannelOutput(index.row(), port);
        QTableWidgetItem *item = ui->tableWidget->item(index.row(), 1);
        if (port == -1) {
            item->setText(synthStr);
        } else {
            item->setText(QString::fromStdString(devices[port]));
        }
    }

    QList<int> ports;
    for (int i=0; i<16; i++) {
        ports.append(player->midiChannel()[i].port());
    }

    QSettings st;
    st.setValue("MidiChannelMapper", QVariant::fromValue(ports));
}

void MapChannelDialog::on_btnClose_clicked()
{
    this->close();
}
