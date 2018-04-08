#include "ChMxComboBox.h"

ChMxComboBox::ChMxComboBox(QWidget *parent) : QComboBox(parent)
{

}

void ChMxComboBox::showPopup()
{
    _popupVisible = true;
    QComboBox::showPopup();
}

void ChMxComboBox::hidePopup()
{
    QComboBox::hidePopup();
    _popupVisible = false;
}
