#ifndef LYRMANAGER_H
#define LYRMANAGER_H

#include "Lyrwidget.h"

#include <QObject>
#include <QFile>
#include <QPropertyAnimation>

class LyrManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int cursorWidth READ cursorWidth WRITE setCursorWidth)

public:
    explicit LyrManager(QObject *parent = 0,
                        LyrWidget *line1 = 0, LyrWidget *line2 = 0, LyrWidget *curLine = 0);
    ~LyrManager();

    void setLyrics(const QString &lyr, QFile *curFile, uint32_t resolution);
    void setPositionCursor(int tick);
    void setSeekPositionCursor(int tick);
    void reset();

    QFont   textFont()          { return line1->font(); }
    QColor  textColor()         { return line1->textColor(); }
    QColor  textBorderColor()   { return line1->textBorderColor(); }
    int     textBorderWidth()   { return line1->textBorderWidth(); }

    QColor  curColor()          { return curLine->textColor(); }
    QColor  curBorderColor()    { return curLine->textBorderColor(); }
    int     curBorderWidth()    { return curLine->textBorderWidth(); }

    void setTextFont(const QFont &f);
    void setTextColor(const QColor &c);
    void setTextBorderColor(const QColor &c);
    void setTextBorderWidth(int w);

    void setCurColor(const QColor &c);
    void setCurBorderColor(const QColor &c);
    void setCurBorderWidth(int w);

    int cursorWidth() { return cursor_width; }
    void setCursorWidth(int w);

    int line1Y() { return line1_y; }
    int line2Y() { return line2_y; }

public slots:
    void show();
    void hide();
    void setLine1Y(int y);
    void setLine2Y(int y);
    void onMainWindowResized(const QSize &s);

signals:

private slots:
    void calculateLinesPosition();

private:
    LyrWidget *line1;
    LyrWidget *line2;
    LyrWidget *curLine;

    int line1_y = 220;  // from bottom
    int line2_y = 100;  // from bottom
    QSize mainWinSize;

    QStringList lyrics;
    QList<long> cursors;
    bool isLine1 = true;
    int linesIndex = 0;

    QList<int> chars_width; // In current line
    int char_index = 0; // In current line
    int cursor_index = 0;
    int cursor_width = 0;  // current_Cur
    int cursor_toEnd = 0;
    bool at_end_line = false;

    QPropertyAnimation *animation;
};

#endif // LYRMANAGER_H
