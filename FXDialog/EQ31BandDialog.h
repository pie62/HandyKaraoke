#ifndef EQ31BANDDIALOG_H
#define EQ31BANDDIALOG_H

#include <QDialog>

namespace Ui {
class EQ31BandDialog;
}

class EQ31BandDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EQ31BandDialog(QWidget *parent = 0);
    ~EQ31BandDialog();

private:
    Ui::EQ31BandDialog *ui;
};

#endif // EQ31BANDDIALOG_H
