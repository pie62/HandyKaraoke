#include "VSTManagementDialog.h"
#include "ui_VSTManagementDialog.h"


VSTManagementDialog::VSTManagementDialog(QWidget *parent, MidiSynthesizer *synth,
                                         int *selectedIndex) :
    QDialog(parent),
    ui(new Ui::VSTManagementDialog)
{
    ui->setupUi(this);

    this->synth = synth;
    this->selectedIndex = selectedIndex;

    for (const VSTNamePath &v : synth->VSTList().values()) {
        ui->vstListWidget->addItem(v.vstvendor + " - " + v.vstName);
    }

    ui->vstListWidget->setCurrentRow(0);
}

VSTManagementDialog::~VSTManagementDialog()
{
    delete ui;
}

void VSTManagementDialog::on_btnAdd_clicked()
{
    *selectedIndex = ui->vstListWidget->currentRow();
    this->close();
}

void VSTManagementDialog::on_btnCancel_clicked()
{
    *selectedIndex = -1;
    this->close();
}
