#ifndef VSTDIRSDIALOG_H
#define VSTDIRSDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QLabel>
#include <QSignalMapper>

#include "MainWindow.h"
#include "Midi/MidiSynthesizer.h"

namespace Ui {
class VSTDirsDialog;
}

class VSTDirsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit VSTDirsDialog(QWidget *parent = 0, MainWindow *mainWindow = 0);
    ~VSTDirsDialog();

private slots:
    void on_btnClose_clicked();

    // VST Dirs tab
    void on_btnAdd_clicked();
    void on_btnRemove_clicked();
    void on_btnUpdate_clicked();


    // VSTi tab
    void initVSTiDetail(int vstiIndex, QLineEdit *le, QLabel *lbName, QLabel *lbVendor, QPushButton *btn, QPushButton *btn2);
    bool setVSTiFile(int vstiIndex, QLineEdit *le, QLabel *lbName, QLabel *lbVendor, QPushButton *btn, QPushButton *btn2);
    void showVSTiDialog(DWORD vsti, const QString &instName = QString());
    void removeVSTiFile(int vstiIndex, QLineEdit *le, QLabel *lbName, QLabel *lbVendor, QPushButton *btn, QPushButton *btn2);

    void on_btnVSTi1_clicked();
    void on_btnVSTi2_clicked();
    void on_btnVSTi3_clicked();
    void on_btnVSTi4_clicked();
    void on_btnVSTiRemove1_clicked();
    void on_btnVSTiRemove2_clicked();
    void on_btnVSTiRemove3_clicked();
    void on_btnVSTiRemove4_clicked();


    // VSTi used tab
    void showVSTiSelectMenu(const QPoint &pos);
    void setVSTiUsed(int selected);

private:
    Ui::VSTDirsDialog *ui;
    MainWindow * mainWindow;
    SynthMixerDialog *mixDlg;
    MidiSynthesizer *synth;

    QSignalMapper *vstiSignalMapper = nullptr;
};

#endif // VSTDIRSDIALOG_H
