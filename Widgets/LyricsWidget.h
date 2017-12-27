#ifndef LYRICSWIDGET_H
#define LYRICSWIDGET_H

#include <QWidget>
#include <QVariantAnimation>

enum class LinePosition {
    Center,
    Left,
    Right
};

class LyricsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LyricsWidget(QWidget *parent = 0);
    ~LyricsWidget();

    void reset();
    void setLyrics(const QString &lyr, const QList<long> &curs);
    void setPositionCursor(int tick);
    void setSeekPositionCursor(int tick);

    QString lyrData() { return lyrics.join("\r\n"); }
    QList<long> curData() { return cursors; }

    QFont   textFont()          { return font(); }
    QColor  textColor()         { return tColor; }
    QColor  textBorderColor()   { return tBorderColor; }
    QColor  textBorderOutColor(){ return tBorderOutColor; }
    int     textBorderWidth()   { return tBorderWidth; }
    int     textBorderOutWidth(){ return tBorderOutWidth; }

    QColor  curColor()          { return cColor; }
    QColor  curBorderColor()    { return cBorderColor; }
    QColor  curBorderOutColor() { return cBorderOutColor; }
    int     curBorderWidth()    { return cBorderWidth; }
    int     curBorderOutWidth() { return cBorderOutWidth; }

    void setTextFont(const QFont &f);
    void setTextColor(const QColor &c);
    void setTextBorderColor(const QColor &c);
    void setTextBorderOutColor(const QColor &c);
    void setTextBorderWidth(int w);
    void setTextBorderOutWidth(int w);

    void setCurColor(const QColor &c);
    void setCurBorderColor(const QColor &c);
    void setCurBorderOutColor(const QColor &c);
    void setCurBorderWidth(int w);
    void setCurBorderOutWidth(int w);

    bool isAutoFontSize() { return autoFontSize; }
    void setAutoFontSize(bool a) { autoFontSize = a; }

    LinePosition line1Position() { return line1_p; }
    LinePosition line2Position() { return line2_p; }
    void setLine1Position(LinePosition p);
    void setLine2Position(LinePosition p);

    int line1Y() { return line1_y; }
    int line2Y() { return line2_y; }
    void setLine1Y(int y);
    void setLine2Y(int y);

    QString textLine1() { return tLine1; }
    QString textLine2() { return tLine2; }
    void setTextLine1(const QString &text, bool andUpdate = true);
    void setTextLine2(const QString &text, bool andUpdate = true);

    int  animationTime()         { return animation->duration(); }
    void setAnimationTime(int t) { animation->setDuration(t); }
    void stopAnimation()         { animation->stop(); }

public slots:

signals:

protected:
    void resizeEvent(QResizeEvent *event);
    void paintEvent(QPaintEvent *event);

private slots:
    void onAnimationValueChanged(const QVariant &v);

private:
    QVariantAnimation *animation;

    QString tLine1, tLine2;
    QPixmap pixLine1, pixLine2;
    QPixmap pixCurLine1, pixCurLine2;

    LinePosition line1_p = LinePosition::Center;
    LinePosition line2_p = LinePosition::Center;

    int line1_y = 220;  // from bottom
    int line2_y = 100;  // from bottom

    QStringList lyrics;
    QList<long> cursors;
    bool isLine1 = true;
    bool autoFontSize = true;
    int linesIndex = 0;

    QList<int> chars_width; // In current line
    int char_index = 0; // In current line
    int cursor_index = 0;
    int cursor_width = 0;  // current cursor position
    int cursor_toEnd = 0;
    bool at_end_line = false;

    int tBorderWidth = 2, tBorderOutWidth = 1;
    int cBorderWidth = 3, cBorderOutWidth = 1;

    QColor tColor, tBorderColor, tBorderOutColor;
    QColor cColor, cBorderColor, cBorderOutColor;

    QRect updateArea;

    QList<int> getCharsWidth();
    QList<int> getCharsWidth(const QString &text);
    QRect calculateUpdateArea();
    QSize calculateLineSize(const QString &text);

    int getAllBorderSize();
    QFont getPerfectFont(const QString &text);
    QPoint getLine1Point();
    QPoint getLine2Point();

    void drawTextToPixmap(QPixmap *pix, const QString &text);
    void drawCursorTextToPixmap(QPixmap *pix, const QString &text);
};

#endif // LYRICSWIDGET_H
