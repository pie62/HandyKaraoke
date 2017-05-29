#include "LyricsWidget.h"

#include <QDebug>
#include <QPainter>
#include <QFile>
#include <QThread>

LyricsWidget::LyricsWidget(QWidget *parent) : QWidget(parent)
{
    //ani = new QVariantAnimation();
    //connect(ani, SIGNAL(valueChanged(QVariant)), this, SLOT(onAniValueChanged(QVariant)));
    //ani->setDuration(100);
    /*timer = new QTimer();
    timer->setInterval(5);
    connect(timer, SIGNAL(timeout()), this, SLOT(onTimerTimeout()));*/
    animation = new QPropertyAnimation();
    animation->setTargetObject(this);
    animation->setPropertyName("currentcur");
    animation->setDuration(200);

    text_color.setRgb(255, 255, 127);
    text_border_color = Qt::black;
    cur_color = Qt::red;
    cur_border_color = Qt::white;


    text_font.setPointSize(40);
    text_font.setBold(true);

    //this->
    this->setMaximumHeight(text_font.pointSize() * 5);
    this->setMinimumHeight(text_font.pointSize() * 5);

}

LyricsWidget::~LyricsWidget()
{
    cur.clear();
    lyrics.clear();

    delete animation;
}

void LyricsWidget::setLyrics(QString lyr, QString curPath, uint32_t resolution)
{
    if (animation->state() == QPropertyAnimation::Running) {
        animation->stop();
    }

    linesIndex = 0;
    lyrics.clear();
    lyrics = lyr.split("\r\n");

    cur.clear();
    QFile f(curPath);
    f.open(QIODevice::ReadOnly);
    QDataStream in(&f);
    while (!in.atEnd()) {
        quint8 b1 = 0;
        quint8 b2 = 0;
        in >> b1;
        in >> b2;
        long cs = (b1 + (b2 << 8)) * resolution / 24;
        cur.append(cs);
    }

    if (linesIndex < lyrics.count()) {
        line1 = lyrics.at(linesIndex);
        linesIndex++;
    }
    line2 = "";
    /*if (linesIndex < lyrics.count()) {
        line2 = lyrics.at(1);
    }*/

    calculateLinePosition();
    isLine1 = true;
    percent = 0;
    current_Cur = 0;
    char_index = -1;
    cur_index =0;
    at_end_line = false;

    this->update();
}

void LyricsWidget::setCurrentCur(int cc)
{
    if (cc <= current_Cur) {
        return;
    }

    current_Cur = cc;
    this->update();
}

void LyricsWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

    QPen penText;
    penText.setWidth(text_border_width);
    penText.setColor(text_border_color);

    // line 1
    QPainterPath path1;
    path1.addText(line1_x, line1_y, text_font, line1);
    painter.fillPath( path1, text_color);

    // line 2
    QPainterPath path2;
    path2.addText(line2_x, line2_y, text_font, line2);
    painter.fillPath( path2, text_color);

    if (text_border_width > 0) painter.setPen(penText);
    painter.drawPath(path1);
    painter.drawPath(path2);

    // Cursor
    QPen penCur;
    penCur.setWidth(cur_border_width);
    penCur.setColor(cur_border_color);

    if (cur_border_width > 0) painter.setPen(penCur);

    if (isLine1)
    {
        QRect rect = path1.boundingRect().toRect();
        QRect inRect(rect.x(), rect.y(), current_Cur, rect.height());
        QRegion r(rect);
        r = r.intersected(inRect);

        painter.setBrush(cur_color);
        painter.setClipRegion(r);
        painter.drawPath(path1);
    }
    else
    {
        QRect rect = path2.boundingRect().toRect();
        QRect inRect(rect.x(), rect.y(), current_Cur, rect.height());
        QRegion r(rect);
        r = r.intersected(inRect);

        painter.setBrush(cur_color);
        painter.setClipRegion(r);
        painter.drawPath(path2);
    }

    //painter.setPen(pen);
    //painter.drawPath(path1);
    //painter.drawPath(path2);

}

void LyricsWidget::resizeEvent(QResizeEvent *event)
{
    calculateLinePosition();
    this->update();
}

void LyricsWidget::setTextFont(QFont f)
{
    text_font = f;
    this->setMaximumHeight(text_font.pointSize() * 5);
    this->setMinimumHeight(text_font.pointSize() * 5);
    calculateLinePosition();
    this->update();
}

void LyricsWidget::setTextColor(QColor c)
{
    text_color = c;
    this->update();
}

void LyricsWidget::setTextBorderColor(QColor c)
{
    text_border_color = c;
    this->update();
}

void LyricsWidget::setTextBorderWidth(int w)
{
    text_border_width = w;
    this->update();
}

void LyricsWidget::setCurColor(QColor c)
{
    cur_color = c;
    this->update();
}

void LyricsWidget::setCurBorderColor(QColor c)
{
    cur_border_color = c;
    this->update();
}

void LyricsWidget::setCurBorderWidth(int w)
{
    cur_border_width = w;
    this->update();
}

void LyricsWidget::reset()
{
    if (animation->state() == QPropertyAnimation::Running) {
        animation->stop();
    }

    linesIndex = 0;
    if (linesIndex < lyrics.count()) {
        line1 = lyrics.at(linesIndex);
        linesIndex++;
    }
    line2 = "";
    calculateLinePosition();
    isLine1 = true;
    percent = 0;
    current_Cur = 0;
    char_index = -1;
    cur_index =0;
    at_end_line = false;

    this->update();
}

void LyricsWidget::setCursorPosition(int tick)
{
    //int ee = cur[0];
    if (cur_index >= cur.count())
        return;


    if (tick < cur[cur_index])
        return;

    bool use_animation = true;

    if (at_end_line) {
        at_end_line = false;
        char_index = 0;
        percent = 0;
        current_Cur = 0;
        isLine1 = !isLine1;
    }

    if (char_width.count() == 0 && linesIndex < lyrics.count()) {
        if (isLine1) line2 = lyrics.at(linesIndex);
        else line1 = lyrics.at(linesIndex);
        linesIndex++;
        isLine1 = !isLine1;
        cur_index++;
        calculateLinePosition();
        this->update();
        return;
    }

    if (char_index == char_width.count()) {
        if (use_animation) {
            animation->stop();
            setCurrentCur(percent);
        }
        //animation->stop();
        at_end_line = true;
        //setCurrentCur(percent);
        cur_index++;
        return;
    }

    if (char_index == 0) {
        if (linesIndex < lyrics.count()) {
            if (isLine1) line2 = lyrics.at(linesIndex);
            else line1 = lyrics.at(linesIndex);
            linesIndex++;
        } else {
            if (isLine1) line2 = "";
            else line1 = "";
        }
        calculateLinePosition();
    }

    if (char_index >= 0) {
        percent = char_width[char_index];
    }
    char_index++;


    if (cur_index == cur.count() - 1) {
        percent = char_width.last();
    }

    cur_index++;

    if (use_animation) {
        if (animation->state() == QPropertyAnimation::Running) {
            animation->stop();
        }
        animation->setStartValue(current_Cur);
        animation->setEndValue(percent);
        animation->start();
    } else {
        setCurrentCur(percent);
    }

}

void LyricsWidget::setSeekPosition(int tick)
{
    if (animation->state() == QPropertyAnimation::Running) {
        animation->stop();
    }

    if (cur.count() == 0) return;
    linesIndex = 0;
    if (linesIndex < lyrics.count()) {
        line1 = lyrics.at(linesIndex);
        linesIndex++;
    }
    line2 = "";

    calculateLinePosition();
    isLine1 = true;
    percent = 0;
    current_Cur = 0;
    cur_index = 0;
    char_index = -1;
    char_width.clear();
    at_end_line = false;

    int round = 0;
    while (cur[round] < tick) {
        round++;
        if (round == cur.count()) break;
    }
    for (int i=0; i<=round; i++) {

        if (isLine1) {

            if (line1.count() == 0 && linesIndex < lyrics.count()) {
                line2 = lyrics.at(linesIndex);
                linesIndex++;
                isLine1 = false;
                cur_index++;
                calculateLinePosition();
                continue;
            }

            if (char_index == line1.count()) {
                isLine1 = false;
                cur_index++;
                char_index = 0;
                percent = 0;
                current_Cur = 0;
                continue;
            }

            if (char_index == 0) {
                if (linesIndex < lyrics.count()) {
                    line2 = lyrics.at(linesIndex);
                    linesIndex++;
                } else {
                    line2 = "";
                }
                calculateLinePosition();
            }

        } else {

            if (line2.count() == 0 && linesIndex < lyrics.count()) {
                line1 = lyrics.at(linesIndex);
                linesIndex++;
                isLine1 = true;
                cur_index++;
                calculateLinePosition();
                continue;
            }

            if (char_index == line2.count()) {
                isLine1 = true;
                cur_index++;
                char_index = 0;
                percent = 0;
                current_Cur = 0;
                continue;
            }

            if (char_index == 0) {
                if (linesIndex < lyrics.count()) {
                    line1 = lyrics.at(linesIndex);
                    linesIndex++;
                } else {
                    line1 = "";
                }
                calculateLinePosition();
            }

        }

        if (char_index >= 0)
            percent = char_width[char_index];
        char_index++;

        if (cur_index == cur.count() - 1) {
            percent = char_width.last();
        }

        cur_index++;
        current_Cur = percent;
    }
    at_end_line = (char_index == char_width.count()) ? true : false;
    calculateLinePosition();
    this->update();
}

void LyricsWidget::calculateLinePosition()
{
    QFontMetrics fm(text_font);

    line1_x = (this->width() - fm.width(line1)) / 2;
    line1_y = text_font.pointSize() * 1.8;

    line2_x = (this->width() - fm.width(line2)) / 2;
    line2_y = text_font.pointSize() * 4.5;

    char_width.clear();
    if (isLine1) {
        for (int i=0; i<line1.length(); i++) {
            int w = fm.width(line1.left(i+1));
            char_width.append(w);
        }
    } else {
        for (int i=0; i<line2.length(); i++) {
            int w = fm.width(line2.left(i+1));
            char_width.append(w);
        }
    }

    if (char_width.count() > 0) {
        char_width.last() += 2;
    }
    //qDebug() << "current asddsd" << ani->currentValue();
}

void LyricsWidget::onTimerTimeout()
{
    /*if (currentCur < percent) {
        currentCur++;
        this->update();
    } else {
        //timer->stop();
    }
    qDebug() << currentCur;*/
}
