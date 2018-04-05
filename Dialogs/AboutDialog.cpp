#include "AboutDialog.h"
#include "ui_AboutDialog.h"

#include "version.h"

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);

    ui->lbVersion->setText(VER_FILEVERSION_STR);

    QString version = QT_VERSION_STR;
    ui->lbQtVer->setText("Using Qt version " + version);
}

AboutDialog::~AboutDialog()
{
    delete ui;
}
