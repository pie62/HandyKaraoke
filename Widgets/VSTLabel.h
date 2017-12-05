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
    explicit VSTLabel(QWidget *parent = 0, const QString &label = QString(),
                      int fxIndex = 0, bool bypass = false);
    ~VSTLabel();

    int getFxIndex() { return fxIndex; }
    void setFxIndex(int i) { fxIndex = i; }

signals:
    void byPassChanged(int index, bool bypass);
    void doubleClicked(int index);
    void menuRequested(int index, const QPoint &pos);

protected:
    void mouseDoubleClickEvent(QMouseEvent *event);

private slots:
    void on_btn_clicked();
    void contextMenuRequested(const QPoint &pos);

private:
    Ui::VSTLabel *ui;
    bool fxBypass = false;
    int fxIndex = 0;
};

#endif // VSTLABEL_H
