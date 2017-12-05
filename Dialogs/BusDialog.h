#ifndef BUSDIALOG_H
#define BUSDIALOG_H

#include "Midi/MidiSynthesizer.h"
#include "Widgets/InstCh.h"

#include <QDialog>
#include <QSignalMapper>

namespace Ui {
class BusDialog;
}

class BusDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BusDialog(QWidget *parent = 0,
                       QMap<InstrumentType, InstCh*> *chInstMap = nullptr,
                       MidiSynthesizer *synth = nullptr);
    ~BusDialog();

private slots:
    void on_btnClose_clicked();
    void showBusMenu(const QPoint &pos);
    void setBus(int b);
    void setBusName(int row, int col);

private:
    Ui::BusDialog *ui;
    QMap<InstrumentType, InstCh*> *chInstMap;
    MidiSynthesizer *synth;
    QSignalMapper busSignalMapper;

    QList<QString> busNames();
};

#endif // BUSDIALOG_H
