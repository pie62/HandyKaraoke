#include "LyrManager.h"

LyrManager::LyrManager(QObject *parent, LyrWidget *line1, LyrWidget *line2, LyrWidget *curLine)
    : QObject(parent)
{
    this->line1 = line1;
    this->line2 = line2;
    this->curLine = curLine;

    animation = new QPropertyAnimation(curLine, "size");
    animation->setDuration(400);

    QFont f;
    f.setBold(true);
    f.setPointSize(40);

    setTextFont(f);
    setTextColor(QColor("#f3f378"));
    setTextBorderColor(QColor("#000000"));
    setTextBorderWidth(1);

    setCurColor(QColor("#ff0000"));
    setCurBorderColor(QColor("#ffffff"));
    setCurBorderWidth(2);

    mainWinSize.setWidth(1000);
    mainWinSize.setHeight(670);

    setLine1Y(220);
    setLine2Y(100);
}

LyrManager::~LyrManager()
{
    cursors.clear();
    lyrics.clear();

    delete animation;
}

void LyrManager::setLyrics(const QString &lyr, QFile *curFile, uint32_t resolution)
{
    if (animation->state() == QPropertyAnimation::Running) {
        animation->stop();
    }

    cursors.clear();
    lyrics.clear();

    lyrics = lyr.split("\r\n");

    if (!curFile->isOpen()) {
        curFile->open(QIODevice::ReadOnly);
    }
    QDataStream in(curFile);
    while (!in.atEnd()) {
        quint8 b1 = 0;
        quint8 b2 = 0;
        in >> b1;
        in >> b2;
        long cs = (b1 + (b2 << 8)) * resolution / 24;
        cursors.append(cs);
    }

    reset();
}

void LyrManager::setPositionCursor(int tick)
{
    if (cursor_index >= cursors.count())
        return;

    if (tick < cursors[cursor_index])
        return;

    if (at_end_line) {
        at_end_line = false;
        char_index = 0;
        cursor_width = 0;
        cursor_toEnd = 0;
        isLine1 = !isLine1;
    }

    if (chars_width.count() == 0 && linesIndex < lyrics.count()) {
        if (isLine1) line2->setText(lyrics.at(linesIndex));
        else line1->setText(lyrics.at(linesIndex));
        linesIndex++;
        isLine1 = !isLine1;
        cursor_index++;
        calculateLinesPosition();
        return;
    }

    if (char_index == chars_width.count()) {
        animation->stop();
        curLine->resize(cursor_toEnd, curLine->height());
        at_end_line = true;
        cursor_index++;
        return;
    }

    if (char_index == 0) {
        if (linesIndex < lyrics.count()) {
            if (isLine1) line2->setText(lyrics.at(linesIndex));
            else line1->setText(lyrics.at(linesIndex));
            linesIndex++;
        } else {
            if (isLine1) line2->setText("");
            else line1->setText("");
        }
        calculateLinesPosition();
    }

    if (char_index >= 0 && chars_width.count() != 0) {
        cursor_toEnd = chars_width.at(char_index);
    }

    char_index++;

    if (cursor_index == cursors.count() - 1) {
        cursor_toEnd = chars_width.count() == 0 ? 0 : chars_width.last();
    }

    cursor_index++;

    animation->stop();
    animation->setStartValue(curLine->size());
    animation->setEndValue(QSize(cursor_toEnd, curLine->height()));
    animation->start();
}

void LyrManager::setSeekPositionCursor(int tick)
{
    if (animation->state() == QPropertyAnimation::Running) {
        animation->stop();
    }

    if (tick == 0) {
        reset();
        return;
    }

    if (cursors.count() == 0) {
        return;
    }

    reset();

    int round = 0;
    while (cursors[round] < tick) {
        round++;
        if (round == cursors.count())
            break;
    }

    for (int i=0; i<round; i++) {

        if (at_end_line) {
            at_end_line = false;
            char_index = 0;
            cursor_width = 0;
            cursor_toEnd = 0;
            isLine1 = !isLine1;
        }

        if (chars_width.count() == 0 && linesIndex < lyrics.count()) {
            if (isLine1) line2->setText(lyrics.at(linesIndex));
            else line1->setText(lyrics.at(linesIndex));
            linesIndex++;
            isLine1 = !isLine1;
            cursor_index++;
            calculateLinesPosition();
            continue;
        }

        if (char_index == chars_width.count()) {
            at_end_line = true;
            cursor_index++;
            continue;
        }

        if (char_index == 0) {
            if (linesIndex < lyrics.count()) {
                if (isLine1) line2->setText(lyrics.at(linesIndex));
                else line1->setText(lyrics.at(linesIndex));
                linesIndex++;
            } else {
                if (isLine1) line2->setText("");
                else line1->setText("");
            }
            calculateLinesPosition();
        }

        if (char_index >= 0 && chars_width.count() != 0) {
            cursor_toEnd = chars_width.at(char_index);
        }

        char_index++;

        if (cursor_index == cursors.count() - 1) {
            cursor_toEnd = chars_width.count() == 0 ? 0 : chars_width.last();
        }

        cursor_index++;

    } // end for loop

    at_end_line = (char_index == chars_width.count()) ? true : false;
    calculateLinesPosition();
    curLine->resize(cursor_toEnd, curLine->height());
}

void LyrManager::reset()
{
    if (animation->state() == QPropertyAnimation::Running) {
        animation->stop();
    }

    linesIndex = 0;
    if (lyrics.count() > 0) {
        line1->setText(lyrics.at(0));
        linesIndex = 1;
    }
    if (lyrics.count() > 1) {
        line2->setText(lyrics.at(1));
    }

    calculateLinesPosition();

    curLine->resize(0, curLine->height());
    isLine1 = true;
    cursor_toEnd = 0;
    cursor_width = 0;
    cursor_index = 0;
    char_index = -1;
    at_end_line = false;
}

void LyrManager::setTextFont(const QFont &f)
{
    line1->setTextFont(f);
    line2->setTextFont(f);
    curLine->setTextFont(f);

    calculateLinesPosition();

    /*if (char_index != -1 && char_index != chars_width.count())
        curLine->resize(chars_width[char_index], curLine->height());
    */

}

void LyrManager::setTextColor(const QColor &c)
{
    line1->setTextColor(c);
    line2->setTextColor(c);
}

void LyrManager::setTextBorderColor(const QColor &c)
{
    line1->setTextBorderColor(c);
    line2->setTextBorderColor(c);
}

void LyrManager::setTextBorderWidth(int w)
{
    line1->setTextBorderWidth(w);
    line2->setTextBorderWidth(w);
}

void LyrManager::setCurColor(const QColor &c)
{
    curLine->setTextColor(c);
}

void LyrManager::setCurBorderColor(const QColor &c)
{
    curLine->setTextBorderColor(c);
}

void LyrManager::setCurBorderWidth(int w)
{
    curLine->setTextBorderWidth(w);
}

void LyrManager::setCursorWidth(int w)
{
    if (w == cursor_width)
        return;
    cursor_width = w;
    curLine->resize(w, curLine->height());
}

void LyrManager::show()
{
    line1->show();
    line2->show();
    curLine->show();
}

void LyrManager::hide()
{
    line1->hide();
    line2->hide();
    curLine->hide();
}

void LyrManager::setLine1Y(int y)
{
    line1->setY(mainWinSize.height() - y);
    if (isLine1) {
        curLine->setY(mainWinSize.height() - y);
    }
    line1_y = y;
}

void LyrManager::setLine2Y(int y)
{
    line2->setY(mainWinSize.height() - y);
    if (!isLine1) {
        curLine->setY(mainWinSize.height() - y);
    }
    line2_y = y;
}

void LyrManager::onMainWindowResized(const QSize &s)
{
    mainWinSize = s;
    line1->move((s.width() - line1->width()) / 2, s.height() - line1_y);
    line2->move((s.width() - line2->width()) / 2, s.height() - line2_y);

    if (isLine1) curLine->move(line1->pos());
    else curLine->move(line2->pos());
    //curLine->setX((s.width() - curLine->width()) / 2);
}

void LyrManager::calculateLinesPosition()
{
    line1->setX((mainWinSize.width() - line1->width()) / 2);
    line2->setX((mainWinSize.width() - line2->width()) / 2);

    chars_width.clear();
    if (isLine1) {
        curLine->hide();
        curLine->setText(line1->text());
        curLine->move(line1->pos().x(), line1->pos().y());
        curLine->resize(0, curLine->height());
        curLine->show();
        chars_width = line1->charsWidth();
    } else {
        curLine->hide();
        curLine->setText(line2->text());
        curLine->move(line2->pos().x(), line2->pos().y());
        curLine->resize(0, curLine->height());
        curLine->show();
        chars_width = line2->charsWidth();
    }
}
