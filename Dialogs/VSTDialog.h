#ifndef VSTDIALOG_H
#define VSTDIALOG_H

#include <cstdint>
#include <QDialog>

class VSTDialog : public QDialog
{
    Q_OBJECT
public:
    explicit VSTDialog(QWidget *parent = nullptr, uint32_t fxHandle = 0);

    uint32_t getFxHandle() { return fxHandle; }

signals:
    void closing(uint32_t fx);

public slots:

protected:
    void closeEvent(QCloseEvent *event);

private:
    uint32_t fxHandle;
};

#endif // VSTDIALOG_H
