#include "VSTDialog.h"

#include <QCloseEvent>

VSTDialog::VSTDialog(QWidget *parent, uint32_t fxHandle) : QDialog(parent)
{
    this->fxHandle = fxHandle;
}

void VSTDialog::closeEvent(QCloseEvent *event)
{
    emit closing(fxHandle);
    event->accept();
}
