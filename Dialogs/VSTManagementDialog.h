#ifndef VSTMANAGEMENTDIALOG_H
#define VSTMANAGEMENTDIALOG_H

#include "Midi/MidiSynthesizer.h"

#include <QDialog>

namespace Ui {
class VSTManagementDialog;
}

class VSTManagementDialog : public QDialog
{
    Q_OBJECT

public:
    explicit VSTManagementDialog(QWidget *parent = 0, MidiSynthesizer *synth = nullptr,
                                 int *selectedIndex = nullptr);
    ~VSTManagementDialog();

private slots:
    void on_btnAdd_clicked();

    void on_btnCancel_clicked();

private:
    Ui::VSTManagementDialog *ui;
    MidiSynthesizer *synth;
    int *selectedIndex;
};

#endif // VSTMANAGEMENTDIALOG_H
