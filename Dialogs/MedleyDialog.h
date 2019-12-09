#ifndef MEDLEYDIALOG_H
#define MEDLEYDIALOG_H

#include <QDialog>

class MidiPlayer;

namespace Ui {
class MedleyDialog;
}

class MedleyDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MedleyDialog(QWidget *parent = nullptr, MidiPlayer *player = nullptr);
    ~MedleyDialog();

protected:
    void keyPressEvent(QKeyEvent *e);

private slots:
    void on_btnOk_clicked();
    void on_btnCancel_clicked();

private:
    Ui::MedleyDialog *ui;

    MidiPlayer *_player;
};

#endif // MEDLEYDIALOG_H
