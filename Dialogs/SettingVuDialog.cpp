#include "SettingVuDialog.h"
#include "ui_SettingVuDialog.h"

#include <QColorDialog>

SettingVuDialog::SettingVuDialog(QWidget *parent,  QList<LEDVu*> ledVus) :
    QDialog(parent),
    ui(new Ui::SettingVuDialog)
{
    ui->setupUi(this);

    vus = ledVus;
    setFrameColor();

    if (vus.count() > 0) {
        bool chc = vus[0]->isShowPeakHold();
        ui->chbPh->setChecked(chc);
        ui->spbPh->setValue(vus[0]->peakHoldMs());
        if (!chc) {
            ui->spbPh->setEnabled(false);
            ui->lbPh->setEnabled(false);
        }
    }

    connect(ui->chbPh, SIGNAL(toggled(bool)), this, SLOT(onChbPhToggled(bool)));
    connect(ui->spbPh, SIGNAL(valueChanged(int)), this, SLOT(onSpbPhValueChanged(int)));
}

SettingVuDialog::~SettingVuDialog()
{
    delete ui;
}

void SettingVuDialog::setFrameColor(QFrame *f, const QColor &c)
{
    f->setStyleSheet("background: " + c.name());
}

void SettingVuDialog::setFrameColor()
{
    if (vus.count() > 0) {
        setFrameColor(ui->fOn1, vus[0]->ledColorOn1());
        setFrameColor(ui->fOn2, vus[0]->ledColorOn2());
        setFrameColor(ui->fOn3, vus[0]->ledColorOn3());

        setFrameColor(ui->fOff1, vus[0]->ledColorOff1());
        setFrameColor(ui->fOff2, vus[0]->ledColorOff2());
        setFrameColor(ui->fOff3, vus[0]->ledColorOff3());

        setFrameColor(ui->fBg, vus[0]->backgroundColor());
    }
}

QColor SettingVuDialog::getColor(const QColor &defaultColor)
{
    QColor color = QColorDialog::getColor(defaultColor, this, "Select Color",
                                          QColorDialog::DontUseNativeDialog);
    return color;
}

void SettingVuDialog::on_btnDefault_clicked()
{
    for (LEDVu *vu : vus) {
        vu->setBackGroundColor(Qt::black);
        vu->setLedColorOn1(Qt::green);
        vu->setLedColorOn2(Qt::yellow);
        vu->setLedColorOn3(Qt::red);
        vu->setLedColorOff1(QColor("#003300"));
        vu->setLedColorOff2(QColor("#333300"));
        vu->setLedColorOff3(QColor("#330000"));
    }
    setFrameColor();

    ui->chbPh->setChecked(true);
    ui->spbPh->setValue(500);
}

void SettingVuDialog::on_btnClose_clicked()
{
    close();
}

void SettingVuDialog::on_btnOn1_clicked()
{
    QColor c = getColor(vus[0]->ledColorOn1());
    if (!c.isValid())
        return;

    for (LEDVu *vu : vus) {
        vu->setLedColorOn1(c);
    }

    setFrameColor(ui->fOn1, c);
}

void SettingVuDialog::on_btnOn2_clicked()
{
    QColor c = getColor(vus[0]->ledColorOn2());
    if (!c.isValid())
        return;

    for (LEDVu *vu : vus) {
        vu->setLedColorOn2(c);
    }

    setFrameColor(ui->fOn2, c);
}

void SettingVuDialog::on_btnOn3_clicked()
{
    QColor c = getColor(vus[0]->ledColorOn3());
    if (!c.isValid())
        return;

    for (LEDVu *vu : vus) {
        vu->setLedColorOn3(c);
    }

    setFrameColor(ui->fOn3, c);
}

void SettingVuDialog::on_btnOff1_clicked()
{
    QColor c = getColor(vus[0]->ledColorOff1());
    if (!c.isValid())
        return;

    for (LEDVu *vu : vus) {
        vu->setLedColorOff1(c);
    }

    setFrameColor(ui->fOff1, c);
}

void SettingVuDialog::on_btnOff2_clicked()
{
    QColor c = getColor(vus[0]->ledColorOff2());
    if (!c.isValid())
        return;

    for (LEDVu *vu : vus) {
        vu->setLedColorOff2(c);
    }

    setFrameColor(ui->fOff2, c);
}

void SettingVuDialog::on_btnOff3_clicked()
{
    QColor c = getColor(vus[0]->ledColorOff3());
    if (!c.isValid())
        return;

    for (LEDVu *vu : vus) {
        vu->setLedColorOff3(c);
    }

    setFrameColor(ui->fOff3, c);
}

void SettingVuDialog::on_btnBg_clicked()
{
    QColor c = getColor(vus[0]->backgroundColor());
    if (!c.isValid())
        return;

    for (LEDVu *vu : vus) {
        vu->setBackGroundColor(c);
    }

    setFrameColor(ui->fBg, c);
}

void SettingVuDialog::onChbPhToggled(bool checked)
{
    if (checked) {
        ui->spbPh->setEnabled(true);
        ui->lbPh->setEnabled(true);
    }
    else {
        ui->spbPh->setEnabled(false);
        ui->lbPh->setEnabled(false);
    }

    for (LEDVu *vu : vus) {
        vu->setShowPeakHold(checked);
    }
}

void SettingVuDialog::onSpbPhValueChanged(int value)
{
    for (LEDVu *vu : vus) {
        vu->setPeakHoldMs(value);
    }
}
