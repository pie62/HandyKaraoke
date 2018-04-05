#ifndef CHMXCOMBOBOX_H
#define CHMXCOMBOBOX_H

#include <QComboBox>

class ChMxComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit ChMxComboBox(QWidget *parent = nullptr);

    virtual void showPopup();
    virtual void hidePopup();

    bool isPopupVisible() { return _popupVisible; }

private:
    bool _popupVisible = false;
};

#endif // CHMXCOMBOBOX_H
