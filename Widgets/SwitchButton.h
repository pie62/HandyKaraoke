#ifndef SWITCHBUTTON_H
#define SWITCHBUTTON_H

#include <QWidget>
#include <QPropertyAnimation>

class SwitchButton : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int offset READ offset WRITE setOffset)

public:
    explicit SwitchButton(QWidget *parent = 0);
    ~SwitchButton();

    bool isOn() const {return _on; }
    QString onText() { return _onText; }
    QString offText() { return _offText; }

    int offset() const { return _x; }
    void setOffset(int ost);

public slots:
    void on();
    void off();
    void setOnText(const QString &ot);
    void setOffText(const QString &ot);

signals:
    void switchChanged(bool switchOn);
    void userSwitchChanged(bool switchOn);

protected:
    void enterEvent(QEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);

private:
    QPropertyAnimation *animation;
    int _x = 2;

    bool _on = false;
    QString _onText = "ON";
    QString _offText = "OFF";
};

#endif // SWITCHBUTTON_H
