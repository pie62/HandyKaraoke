#include "VSTDialog.h"

#include <QShowEvent>
#include <QCloseEvent>

VSTDialog::VSTDialog(QWidget *parent, DWORD fxHandle, const QString &instName) : QDialog(parent)
{
    this->fxHandle = fxHandle;

    BASS_VST_INFO info;
    if (BASS_VST_GetInfo(fxHandle, &info) && info.hasEditor)
    {
        QString name = info.effectName;
        name += " - ";
        name += info.vendorName;

        setWindowTitle(name + "  [" + instName + "]");
        setFixedSize(info.editorWidth, info.editorHeight);
        setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
        setWindowFlags(windowFlags() | Qt::WindowMinimizeButtonHint);

        canOpen = true;
    }
}

void VSTDialog::showEvent(QShowEvent *event)
{
    BASS_VST_EmbedEditor(fxHandle, (HWND)this->winId());
    event->accept();
}

void VSTDialog::closeEvent(QCloseEvent *event)
{
    BASS_VST_EmbedEditor(fxHandle, NULL);
    event->accept();
}
