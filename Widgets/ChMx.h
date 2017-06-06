#ifndef CHMX_H
#define CHMX_H

#include <QWidget>

namespace Ui {
class ChMx;
}

class ChMx : public QWidget
{
    Q_OBJECT

public:
    explicit ChMx(QWidget *parent = 0);
    ~ChMx();

private:
    Ui::ChMx *ui;
};

#endif // CHMX_H
