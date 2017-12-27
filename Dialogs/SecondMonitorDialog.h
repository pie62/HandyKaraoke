#ifndef SECONDMONITORDIALOG_H
#define SECONDMONITORDIALOG_H

#include "Widgets/LyricsWidget.h"

#include <QDialog>

namespace Ui {
class SecondMonitorDialog;
}

class SecondMonitorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SecondMonitorDialog(QWidget *parent = 0, LyricsWidget *lyr = nullptr);
    ~SecondMonitorDialog();

    void setBackgroundColor(const QString &colorName);
    void setBackgroundImage(const QString &img);

    LyricsWidget *lyrWidget();

protected:
    void resizeEvent(QResizeEvent *event);

private:
    Ui::SecondMonitorDialog *ui;
    bool useBgImg = false;
    QString bgImg = "";
};

#endif // SECONDMONITORDIALOG_H
