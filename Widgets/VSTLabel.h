#ifndef VSTLABEL_H
#define VSTLABEL_H

#include <QWidget>

namespace Ui {
class VSTLabel;
}

class VSTLabel : public QWidget
{
    Q_OBJECT

public:
    explicit VSTLabel(QWidget *parent = 0, const QString &label = QString());
    ~VSTLabel();

protected:
    void mouseDoubleClickEvent(QMouseEvent *event);

private slots:
    void on_btn_clicked();

private:
    Ui::VSTLabel *ui;
    bool fxOn = false;
};

#endif // VSTLABEL_H
