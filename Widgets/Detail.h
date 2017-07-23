#ifndef DETAIL_H
#define DETAIL_H

#include <QWidget>

namespace Ui {
class Detail;
}

class Detail : public QWidget
{
    Q_OBJECT

public:
    explicit Detail(QWidget *parent = 0);
    ~Detail();

public slots:
    void setDetail(const QString &text, const QString &value);
    void setText(const QString &text);
    void setValue(const QString &value);

private:
    Ui::Detail *ui;
};

#endif // DETAIL_H
