#include "CustomFXList.h"

#include <QScrollBar>

CustomFXList::CustomFXList(QWidget *parent) : QListWidget(parent)
{

}

void CustomFXList::enterEvent(QEvent *event)
{
    QListWidget::enterEvent(event);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
}

void CustomFXList::leaveEvent(QEvent *event)
{
    QListWidget::leaveEvent(event);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}
