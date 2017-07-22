#ifndef SETTINGVUDIALOG_H
#define SETTINGVUDIALOG_H

#include <QDialog>

#include <Widgets/LEDVu.h>

namespace Ui {
class SettingVuDialog;
}

class SettingVuDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingVuDialog(QWidget *parent = 0, QList<LEDVu*> ledVus = QList<LEDVu*>());
    ~SettingVuDialog();

private slots:
    void on_btnDefault_clicked();
    void on_btnClose_clicked();

    void on_btnOn1_clicked();
    void on_btnOn2_clicked();
    void on_btnOn3_clicked();
    void on_btnOff1_clicked();
    void on_btnOff2_clicked();
    void on_btnOff3_clicked();
    void on_btnBg_clicked();

    void onChbPhToggled(bool checked);
    void onSpbPhValueChanged(int value);

private:
    Ui::SettingVuDialog *ui;

    QList<LEDVu*> vus;

    void setFrameColor(QFrame *f, const QColor &c);
    void setFrameColor();
    QColor getColor(const QColor &defaultColor);
};

#endif // SETTINGVUDIALOG_H
