#include "MedleyDialog.h"
#include "ui_MedleyDialog.h"

#include "Midi/MidiPlayer.h"

#include <QKeyEvent>


MedleyDialog::MedleyDialog(QWidget *parent, MidiPlayer *player) :
    QDialog(parent),
    ui(new Ui::MedleyDialog)
{
    ui->setupUi(this);

    _player = player;

    ui->spinBpm->setFocus();

    setWindowFlag(Qt::WindowContextHelpButtonHint, false);
}

MedleyDialog::~MedleyDialog()
{
    delete ui;
}

void MedleyDialog::keyPressEvent(QKeyEvent *e)
{
    QWidget::keyPressEvent(e);

    switch (e->key()) {
    case Qt::Key_Enter:
    case Qt::Key_Return:
        if (focusWidget() == ui->spinBpm)
            on_btnOk_clicked();
        break;
    case Qt::Key_Escape:
        close();
        break;
    default:
        break;
    }
}

void MedleyDialog::on_btnOk_clicked()
{
    _player->setMedleyBPM(ui->spinBpm->value());
    _player->setUseMedley(true);
    close();
}

void MedleyDialog::on_btnCancel_clicked()
{
    close();
}
