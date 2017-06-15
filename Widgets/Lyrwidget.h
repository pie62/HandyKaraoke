#ifndef LYRWIDGET_H
#define LYRWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QFontMetrics>
#include <QDebug>

class LyrWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LyrWidget(QWidget *parent = 0) : QWidget(parent) {
        tColor = Qt::yellow;
        bColor = Qt::black;
    }

    QString text() { return lText; }
    void setText(const QString &t) {
        this->lText = t;
        QFontMetrics fm(font());
        int w = fm.width(lText);
        resize(w, height());
        update();
    }

    QColor textColor() { return tColor; }
    void setTextColor(const QColor &c) {
        tColor = c;
        update();
    }

    QColor textBorderColor() { return bColor; }
    void setTextBorderColor(const QColor &c) {
        bColor = c;
        update();
    }

    int textBorderWidth() { return bWidth; }
    void setTextBorderWidth(int bw) {
        bWidth = bw;
        update();
    }

    void setTextFont(const QFont &f) {
        this->setFont(f);
        ly = f.pointSize() * 2;
        resize(width(), f.pointSize() * 2.5);
        update();
    }

    void setX(int x) {
        move(x, pos().y());
    }

    void setY(int y) {
        move(pos().x(), y);
    }

    QList<int> charsWidth() {
        QList<int> cl;
        QFontMetrics fm(font());
        for (int i=0; i<lText.length(); i++) {
            int w = fm.width(lText.left(i+1));
            cl.append(w);
        }
        return cl;
    }

protected:
    void paintEvent(QPaintEvent *event) {

        QPainter painter(this);
        painter.setRenderHints(QPainter::Antialiasing);

        QPainterPath path;
        path.addText(lx, ly, font(), lText);

        painter.fillPath(path, tColor);

        if (bWidth > 0) {
            QPen pen;
            pen.setWidth(bWidth);
            pen.setColor(bColor);
            painter.setPen(pen);
        }

        painter.drawPath(path);
    }

private:
    int lx = 0;
    int ly = 50;
    int bWidth = 1;
    QColor tColor;
    QColor bColor;
    QString lText;
};

#endif // LYRWIDGET_H
