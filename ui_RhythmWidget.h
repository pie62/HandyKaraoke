/********************************************************************************
** Form generated from reading UI file 'RhythmWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RHYTHMWIDGET_H
#define UI_RHYTHMWIDGET_H

#include <QtCore/QLocale>
#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "RhythmWidget.h"

QT_BEGIN_NAMESPACE

class Ui_RhythmWidget
{
public:
    QHBoxLayout *horizontalLayout;
    QFrame *frame;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QLabel *lbBpm;
    QSpacerItem *horizontalSpacer;
    QLabel *lbBeat;
    QFrame *frame_3;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout_3;
    RhythmRectangle *b1;
    RhythmRectangle *b2;
    RhythmRectangle *b3;
    RhythmRectangle *b4;
    RhythmRectangle *b5;

    void setupUi(QWidget *RhythmWidget)
    {
        if (RhythmWidget->objectName().isEmpty())
            RhythmWidget->setObjectName(QStringLiteral("RhythmWidget"));
        RhythmWidget->resize(170, 60);
        RhythmWidget->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));
        horizontalLayout = new QHBoxLayout(RhythmWidget);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        frame = new QFrame(RhythmWidget);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setStyleSheet(QLatin1String("#frame {\n"
"background-color: rgb(62, 62, 62, 200);\n"
"border:1px solid rgb(255, 255, 255);\n"
"}"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        verticalLayout = new QVBoxLayout(frame);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(4, 0, 4, 4);
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        lbBpm = new QLabel(frame);
        lbBpm->setObjectName(QStringLiteral("lbBpm"));
        QFont font;
        font.setPointSize(16);
        font.setBold(true);
        font.setWeight(75);
        lbBpm->setFont(font);
        lbBpm->setStyleSheet(QStringLiteral("color: #F6416C;"));

        horizontalLayout_2->addWidget(lbBpm);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        lbBeat = new QLabel(frame);
        lbBeat->setObjectName(QStringLiteral("lbBeat"));
        lbBeat->setFont(font);
        lbBeat->setStyleSheet(QStringLiteral("color: #00B8A9;"));

        horizontalLayout_2->addWidget(lbBeat);


        verticalLayout->addLayout(horizontalLayout_2);

        frame_3 = new QFrame(frame);
        frame_3->setObjectName(QStringLiteral("frame_3"));
        frame_3->setMinimumSize(QSize(0, 3));
        frame_3->setMaximumSize(QSize(16777215, 3));
        frame_3->setStyleSheet(QLatin1String("background-color: #FFDE7D;\n"
"margin: 0 1px 0 1px;"));
        frame_3->setFrameShape(QFrame::StyledPanel);
        frame_3->setFrameShadow(QFrame::Raised);

        verticalLayout->addWidget(frame_3);

        verticalSpacer = new QSpacerItem(20, 4, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout->addItem(verticalSpacer);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        b1 = new RhythmRectangle(frame);
        b1->setObjectName(QStringLiteral("b1"));
        b1->setMinimumSize(QSize(0, 12));
        b1->setMaximumSize(QSize(16777215, 12));

        horizontalLayout_3->addWidget(b1);

        b2 = new RhythmRectangle(frame);
        b2->setObjectName(QStringLiteral("b2"));
        b2->setMinimumSize(QSize(0, 12));
        b2->setMaximumSize(QSize(16777215, 12));

        horizontalLayout_3->addWidget(b2);

        b3 = new RhythmRectangle(frame);
        b3->setObjectName(QStringLiteral("b3"));
        b3->setMinimumSize(QSize(0, 12));
        b3->setMaximumSize(QSize(16777215, 12));

        horizontalLayout_3->addWidget(b3);

        b4 = new RhythmRectangle(frame);
        b4->setObjectName(QStringLiteral("b4"));
        b4->setMinimumSize(QSize(0, 12));
        b4->setMaximumSize(QSize(16777215, 12));

        horizontalLayout_3->addWidget(b4);

        b5 = new RhythmRectangle(frame);
        b5->setObjectName(QStringLiteral("b5"));
        b5->setMinimumSize(QSize(0, 12));
        b5->setMaximumSize(QSize(16777215, 12));

        horizontalLayout_3->addWidget(b5);


        verticalLayout->addLayout(horizontalLayout_3);


        horizontalLayout->addWidget(frame);


        retranslateUi(RhythmWidget);

        QMetaObject::connectSlotsByName(RhythmWidget);
    } // setupUi

    void retranslateUi(QWidget *RhythmWidget)
    {
        RhythmWidget->setWindowTitle(QApplication::translate("RhythmWidget", "Form", 0));
        lbBpm->setText(QApplication::translate("RhythmWidget", "120", 0));
        lbBeat->setText(QApplication::translate("RhythmWidget", "0:0", 0));
    } // retranslateUi

};

namespace Ui {
    class RhythmWidget: public Ui_RhythmWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RHYTHMWIDGET_H
