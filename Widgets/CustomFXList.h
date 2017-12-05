#ifndef CUSTOMFXLIST_H
#define CUSTOMFXLIST_H

#include <QListWidget>

class CustomFXList : public QListWidget
{
    Q_OBJECT
public:
    explicit CustomFXList(QWidget *parent = nullptr);

signals:

public slots:

protected:
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
};

#endif // CUSTOMFXLIST_H
