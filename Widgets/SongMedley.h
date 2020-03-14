#ifndef SONGMEDLEY_H
#define SONGMEDLEY_H

#include <QWidget>

namespace Ui {
class SongMedley;
}

class SongMedley : public QWidget
{
    Q_OBJECT

public:
    explicit SongMedley(QWidget *parent = nullptr);
    ~SongMedley();

    int cutStartBar();
    int cutEndBar();

    void setup(int cutStartBar, int cutEndBar);

private:
    Ui::SongMedley *ui;
};

#endif // SONGMEDLEY_H
