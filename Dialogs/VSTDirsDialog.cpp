#include "VSTDirsDialog.h"
#include "ui_VSTDirsDialog.h"

#include <QSettings>
#include <QDirIterator>
#include <QFileDialog>


VSTDirsDialog::VSTDirsDialog(QWidget *parent, SynthMixerDialog *mixDlg,
                             MidiSynthesizer *synth) :
    QDialog(parent),
    ui(new Ui::VSTDirsDialog)
{
    ui->setupUi(this);

    this->mixDlg = mixDlg;
    this->synth = synth;

    QSettings st;
    QStringList dirs = st.value("VSTDirs", QStringList()).toStringList();

    QStringList vstDirs;
    vstDirs << QDir::currentPath() + "/VST";
    vstDirs += dirs;

    ui->list->addItems(vstDirs);
    ui->list->setCurrentRow(0);
    if (vstDirs.count() == 1)
        ui->btnRemove->setEnabled(false);
}

VSTDirsDialog::~VSTDirsDialog()
{
    QStringList vstDirs;
    for (int i=1; i<ui->list->count(); i++) {
        QListWidgetItem *item = ui->list->item(i);
        vstDirs << item->text();
    }

    QSettings st;
    st.setValue("VSTDirs", vstDirs);

    delete ui;
}

void VSTDirsDialog::on_btnClose_clicked()
{
    this->close();
}

void VSTDirsDialog::on_btnAdd_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(
                this, "เลือกโฟลเดอร์ VST", QDir::homePath(),
                QFileDialog::ShowDirsOnly
                |QFileDialog::DontResolveSymlinks);

    if (dir == "")
        return;

    ui->list->addItem(dir);
    ui->btnRemove->setEnabled(true);
}

void VSTDirsDialog::on_btnRemove_clicked()
{
    if (ui->list->count() == 1)
        return;

    QListWidgetItem *item = ui->list->takeItem(ui->list->currentRow());
    delete item;

    if (ui->list->count() == 1)
        ui->btnRemove->setEnabled(false);
}

void VSTDirsDialog::on_btnUpdate_clicked()
{
    QStringList filesPath;
    QStringList filesName;

    for (int i=0; i<ui->list->count(); i++) {
        QListWidgetItem *item = ui->list->item(i);
        QDirIterator it(item->text(), QStringList() << "*.DLL" << "*.dll",
                        QDir::Files|QDir::NoSymLinks, QDirIterator::Subdirectories);
        while (it.hasNext()) {
            filesPath << it.next();
            filesName << it.fileName();
        }
    }

    if (filesPath.count() == 0) {
        ui->progressBar->setMaximum(1);
        ui->progressBar->setValue(1);
        synth->setVSTList(QMap<uint, VSTNamePath>());
        mixDlg->setVSTVendorMenu();
        return;
    }

    ui->progressBar->setValue(0);
    ui->progressBar->setMaximum(filesPath.count());

    QMap<uint, VSTNamePath> vstList;

    for (int i=0; i<filesPath.count(); i++) {

        ui->lbVSTName->setText(filesName[i]);

        BASS_VST_INFO info;
        if (!MidiSynthesizer::isVSTFile(filesPath[i], &info)) {
            ui->progressBar->setValue(i+1);
            continue;
        }

        VSTNamePath v;
        v.uniqueID = info.uniqueID;
        v.vstName = info.effectName;
        v.vstvendor = info.vendorName;
        v.vstPath = filesPath[i];

        vstList[info.uniqueID] = v;

        ui->progressBar->setValue(i+1);
    }

    ui->lbVSTName->setText("");
    synth->setVSTList(vstList);
    mixDlg->setVSTVendorMenu();
}
