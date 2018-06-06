#ifndef VSTDIALOG_H
#define VSTDIALOG_H

#include <QDialog>

#include <bass.h>
#include <bass_vst.h>

class VSTDialog : public QDialog
{
    Q_OBJECT
public:
    explicit VSTDialog(QWidget *parent = nullptr, DWORD fxHandle = 0, const QString &instName = QString());

    DWORD getFxHandle() { return fxHandle; }
    bool isCanOpen() { return canOpen; }

public slots:

protected:
    void showEvent(QShowEvent *event);
    void closeEvent(QCloseEvent *event);

private:
    DWORD fxHandle;
    bool canOpen = false;
};

#endif // VSTDIALOG_H
