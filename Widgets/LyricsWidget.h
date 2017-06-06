#ifndef LYRICSWIDGET_H
#define LYRICSWIDGET_H

#include <QWidget>
#include <QPropertyAnimation>
#include <QTimer>

class LyricsWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int currentcur READ currentCur WRITE setCurrentCur)
public:
    explicit LyricsWidget(QWidget *parent = 0);
    ~LyricsWidget();

    QFont textFont() { return text_font; }
    QColor textColor() { return text_color; }
    QColor textBorderColor() { return text_border_color; }
    int textBorderWidth() { return text_border_width; }

    QColor curColor() { return cur_color; }
    QColor curBorderColor() { return cur_border_color; }
    int curBorderWidth() { return cur_border_width; }

    void setLyrics(QString lyr, QString curPath, uint32_t resolution);

    int currentCur() const { return current_Cur; }
    void setCurrentCur(int cc);

protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);

signals:

public slots:
    void setTextFont(QFont f);
    void setTextColor(QColor c);
    void setTextBorderColor(QColor c);
    void setTextBorderWidth(int w);

    void setCurColor(QColor c);
    void setCurBorderColor(QColor c);
    void setCurBorderWidth(int w);

    void reset();
    void setCursorPosition(int tick);
    void setSeekPosition(int tick);

private:
    QTimer *timer;
    QPropertyAnimation *animation;

    QFont text_font;
    QColor text_color; //(Qt::red);
    QColor text_border_color;
    int text_border_width = 1;

    QColor cur_color; //(Qt::blue);
    QColor cur_border_color; //(Qt::white);
    int cur_border_width = 2;

    int line1_x;
    int line1_y;
    int line2_x;
    int line2_y;

    QStringList lyrics;
    QList<long> cur;
    bool isLine1 = true;
    QString line1 = "";
    QString line2 = "";
    int linesIndex = 0;

    QList<int> char_width; // In current line
    int char_index = 0; // In current line
    int cur_index = 0;
    int percent = 0;
    int current_Cur = 0;
    bool at_end_line = false;

private slots:
    void calculateLinePosition();
    void onTimerTimeout();
};

#endif // LYRICSWIDGET_H
