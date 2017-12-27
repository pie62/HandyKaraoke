#include "SecondMonitorDialog.h"
#include "ui_SecondMonitorDialog.h"

SecondMonitorDialog::SecondMonitorDialog(QWidget *parent, LyricsWidget *lyr) :
    QDialog(parent),
    ui(new Ui::SecondMonitorDialog)
{
    ui->setupUi(this);

    ui->lyr->setTextFont(lyr->textFont());
    ui->lyr->setTextColor(lyr->textColor());
    ui->lyr->setTextBorderColor(lyr->textBorderColor());
    ui->lyr->setTextBorderOutColor(lyr->textBorderOutColor());
    ui->lyr->setTextBorderWidth(lyr->textBorderWidth());
    ui->lyr->setTextBorderOutWidth(lyr->textBorderOutWidth());

    ui->lyr->setCurColor(lyr->curColor());
    ui->lyr->setCurBorderColor(lyr->curBorderColor());
    ui->lyr->setCurBorderOutColor(lyr->curBorderOutColor());
    ui->lyr->setCurBorderWidth(lyr->curBorderWidth());
    ui->lyr->setCurBorderOutWidth(lyr->curBorderOutWidth());

    ui->lyr->setLine1Position(lyr->line1Position());
    ui->lyr->setLine2Position(lyr->line2Position());
    ui->lyr->setLine1Y(lyr->line1Y());
    ui->lyr->setLine2Y(lyr->line2Y());
    ui->lyr->setAnimationTime(lyr->animationTime());
    ui->lyr->setAutoFontSize(lyr->isAutoFontSize());

    ui->lyr->setLyrics(lyr->lyrData(), lyr->curData());

    ui->lyr->hide();
}

SecondMonitorDialog::~SecondMonitorDialog()
{
    delete ui;
}

void SecondMonitorDialog::setBackgroundColor(const QString &colorName)
{
    useBgImg = false;
    this->setStyleSheet("background-color: " + colorName + ";");
}

void SecondMonitorDialog::setBackgroundImage(const QString &img)
{
    useBgImg = true;
    bgImg = img;

    this->setStyleSheet("");
    QPixmap bg(img);
    bg = bg.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, bg); //set the pic to the background
    this->setPalette(palette); //show the background pic
}

LyricsWidget *SecondMonitorDialog::lyrWidget()
{
    return ui->lyr;
}

void SecondMonitorDialog::resizeEvent(QResizeEvent *event)
{
    if (useBgImg)
        setBackgroundImage(bgImg);

    if (!ui->lyr->isVisible()) {
        ui->lyr->resize(this->size());
    }
}
