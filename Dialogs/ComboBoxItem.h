#ifndef COMBOBOXITEM_H
#define COMBOBOXITEM_H

#include <QComboBox>

class ComboBoxItem : public QComboBox
{
    Q_OBJECT
public:
    explicit ComboBoxItem(QWidget *parent = 0, int row = 0) : QComboBox(parent) {
        this->iRow = row;
        connect(this, SIGNAL(activated(int)), this, SLOT(onActivated(int)));
    }

signals:
    void userActivated(int row, int index);

private slots:
    void onActivated(int index) {
        emit userActivated(iRow, index);
    }

private:
    int iRow = 0;
};

#endif // COMBOBOXITEM_H
