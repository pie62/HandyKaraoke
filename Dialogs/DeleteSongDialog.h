#ifndef DELETESONGDIALOG_H
#define DELETESONGDIALOG_H

#include <QDialog>

namespace Ui {
class DeleteSongDialog;
}

class DeleteSongDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DeleteSongDialog(QWidget *parent = 0);
    ~DeleteSongDialog();

    bool removeFromStorage() { return rmStorage; }
    bool removeConfirmed() { return deleted; }

private slots:
    void on_cbRmStorage_toggled(bool checked);

    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::DeleteSongDialog *ui;

    bool rmStorage = true;
    bool deleted = false;
};

#endif // DELETESONGDIALOG_H
