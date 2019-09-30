#ifndef SECONDMONITORDIALOG_H
#define SECONDMONITORDIALOG_H

#include "Widgets/Background.h"
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

    Background *backgroundWidget();

    LyricsWidget *lyrWidget();

protected:
    void resizeEvent(QResizeEvent *event);

private:
    Ui::SecondMonitorDialog *ui;
    Background *bgWidget = nullptr;
};

#endif // SECONDMONITORDIALOG_H
