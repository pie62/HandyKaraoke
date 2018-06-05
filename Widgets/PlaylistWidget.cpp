#include "PlaylistWidget.h"
#include "ui_PlaylistWidget.h"

PlaylistWidget::PlaylistWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PlaylistWidget)
{
    ui->setupUi(this);

    #ifdef __linux__
    { // UI Font in linux
        QString fontName = "TH Sarabun New Bold";
        QFont f = this->font();
        f.setBold(true);
        f.setFamily(fontName);
        f.setPointSize(f.pointSize() - 10);

        ui->label->setFont(f);

        f.setPointSize(f.pointSize() - 2);
        ui->tableWidget->setFont(f);
    }
    #endif

    QHeaderView *header = ui->tableWidget->horizontalHeader();
    header->setMinimumSectionSize(40);
    header->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    header->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    header->setSectionResizeMode(2, QHeaderView::Stretch);
    header->setSectionResizeMode(3, QHeaderView::ResizeToContents);
}

PlaylistWidget::~PlaylistWidget()
{
    delete ui;
}

void PlaylistWidget::setPlaylist(const QList<Song *> &songlist)
{
    for (int row=ui->tableWidget->rowCount()-1; row>=0; row--)
        ui->tableWidget->removeRow(row);

    for (Song* s : songlist)
        addSong(s);

    setCurrentRow(0);
}

void PlaylistWidget::addSong(Song *song)
{
    int rowCount = ui->tableWidget->rowCount();

    ui->tableWidget->insertRow(rowCount);
    ui->tableWidget->setItem(rowCount, 0, new QTableWidgetItem(QString::number(rowCount+1)));
    ui->tableWidget->setItem(rowCount, 1, new QTableWidgetItem(song->id()));
    ui->tableWidget->setItem(rowCount, 2, new QTableWidgetItem(song->detailWithoutIDType()));
    ui->tableWidget->setItem(rowCount, 3, new QTableWidgetItem(song->songType()));

    ui->tableWidget->item(rowCount, 0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
}

void PlaylistWidget::updateDetail(int row, Song *song)
{
    ui->tableWidget->item(row, 2)->setText(song->detailWithoutIDType());
}

int PlaylistWidget::rowCount()
{
    return ui->tableWidget->rowCount();
}

int PlaylistWidget::currentRow()
{
    if (ui->tableWidget->rowCount() > 0)
        return ui->tableWidget->currentRow();
    else
        return -1;
}

void PlaylistWidget::setCurrentRow(int row)
{
    if (row >= ui->tableWidget->rowCount())
        return;

    ui->tableWidget->selectRow(row);
}

void PlaylistWidget::removeRow(int row)
{
    if (row >= ui->tableWidget->rowCount())
        return;

    ui->tableWidget->removeRow(row);

    for (int i=row; i<ui->tableWidget->rowCount(); i++)
    {
        ui->tableWidget->item(i, 0)->setText(QString::number(i+1));
    }

    if (row == 0)
        ui->tableWidget->selectRow(0);
    else if (row == ui->tableWidget->rowCount())
        ui->tableWidget->selectRow(row-1);
    else
        ui->tableWidget->selectRow(row);
}

void PlaylistWidget::swapRow(int row, int toRow)
{
    if (row >= ui->tableWidget->rowCount())
        return;

    if (toRow >= ui->tableWidget->rowCount())
        return;

    // row item
    QTableWidgetItem *item1 = ui->tableWidget->takeItem(row, 1);
    QTableWidgetItem *item2 = ui->tableWidget->takeItem(row, 2);
    QTableWidgetItem *item3 = ui->tableWidget->takeItem(row, 3);

    // set new row data
    ui->tableWidget->setItem(row, 1, ui->tableWidget->takeItem(toRow, 1));
    ui->tableWidget->setItem(row, 2, ui->tableWidget->takeItem(toRow, 2));
    ui->tableWidget->setItem(row, 3, ui->tableWidget->takeItem(toRow, 3));

    // set toRow item data
    ui->tableWidget->setItem(toRow, 1, item1);
    ui->tableWidget->setItem(toRow, 2, item2);
    ui->tableWidget->setItem(toRow, 3, item3);

    ui->tableWidget->selectRow(row);
}
