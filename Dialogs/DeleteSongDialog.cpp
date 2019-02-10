#include "DeleteSongDialog.h"
#include "ui_DeleteSongDialog.h"

DeleteSongDialog::DeleteSongDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DeleteSongDialog)
{
    ui->setupUi(this);

    ui->cbRmDatabase->setEnabled(false);
}

DeleteSongDialog::~DeleteSongDialog()
{
    delete ui;
}

void DeleteSongDialog::on_cbRmStorage_toggled(bool checked)
{
    this->rmStorage = checked;
}

void DeleteSongDialog::on_buttonBox_accepted()
{
    this->deleted = true;
    this->close();
}

void DeleteSongDialog::on_buttonBox_rejected()
{
    this->deleted = false;
    this->close();
}
