/********************************************************************************
** Form generated from reading UI file 'ChMx.ui'
**
** Created by: Qt User Interface Compiler version 5.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHMX_H
#define UI_CHMX_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "ChMx.h"
#include "LEDVu.h"
#include "Slider.h"

QT_BEGIN_NAMESPACE

class Ui_ChMx
{
public:
    QVBoxLayout *verticalLayout;
    ChLabel *lbNumber;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *btnMute;
    QPushButton *btnSolo;
    QHBoxLayout *horizontalLayout;
    Slider *slider;
    LEDVu *vuBar;

    void setupUi(QWidget *ChMx)
    {
        if (ChMx->objectName().isEmpty())
            ChMx->setObjectName(QStringLiteral("ChMx"));
        ChMx->resize(41, 114);
        ChMx->setMinimumSize(QSize(41, 0));
        ChMx->setMaximumSize(QSize(41, 16777215));
        ChMx->setMouseTracking(true);
        ChMx->setStyleSheet(QLatin1String("/*#frame {\n"
"background-color: rgb(238, 238, 238);\n"
"}\n"
"*/\n"
"#lbNumber {\n"
"background-color: rgb(117, 117, 117);\n"
"color: rgb(238, 238, 238);\n"
"}\n"
"\n"
"/*#slider::groove:vertical {\n"
"background: rgb(210, 210, 210);\n"
"position: absolute;\n"
"left: 13px; right: 13px;\n"
"	border: 1px solid rgb(171, 171, 171);\n"
"	margin: -1px 0;\n"
"}\n"
" \n"
"#slider::handle:vertical {\n"
"	background: rgb(245, 245, 245);\n"
"	border: 1px solid #aaa;\n"
"border-radius: 2px;\n"
"	height: 10px;\n"
"	margin: 0 -8px;\n"
"}*/\n"
""));
        verticalLayout = new QVBoxLayout(ChMx);
        verticalLayout->setSpacing(2);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 2);
        lbNumber = new ChLabel(ChMx);
        lbNumber->setObjectName(QStringLiteral("lbNumber"));
        lbNumber->setMinimumSize(QSize(0, 23));
        lbNumber->setMaximumSize(QSize(16777215, 23));
        QFont font;
        font.setPointSize(11);
        font.setBold(true);
        font.setWeight(75);
        lbNumber->setFont(font);
        lbNumber->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(lbNumber);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(1);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        btnMute = new QPushButton(ChMx);
        btnMute->setObjectName(QStringLiteral("btnMute"));
        btnMute->setMinimumSize(QSize(18, 17));
        btnMute->setMaximumSize(QSize(18, 17));
        btnMute->setFocusPolicy(Qt::NoFocus);

        horizontalLayout_3->addWidget(btnMute);

        btnSolo = new QPushButton(ChMx);
        btnSolo->setObjectName(QStringLiteral("btnSolo"));
        btnSolo->setMinimumSize(QSize(18, 17));
        btnSolo->setMaximumSize(QSize(18, 17));
        btnSolo->setFocusPolicy(Qt::NoFocus);

        horizontalLayout_3->addWidget(btnSolo);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(3);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(-1, 3, -1, 3);
        slider = new Slider(ChMx);
        slider->setObjectName(QStringLiteral("slider"));
        slider->setMinimumSize(QSize(20, 0));
        slider->setMaximumSize(QSize(20, 16777215));
        slider->setFrameShape(QFrame::StyledPanel);
        slider->setFrameShadow(QFrame::Raised);

        horizontalLayout->addWidget(slider);

        vuBar = new LEDVu(ChMx);
        vuBar->setObjectName(QStringLiteral("vuBar"));
        vuBar->setMinimumSize(QSize(8, 60));
        vuBar->setMaximumSize(QSize(8, 16777215));
        vuBar->setFrameShape(QFrame::StyledPanel);
        vuBar->setFrameShadow(QFrame::Raised);

        horizontalLayout->addWidget(vuBar);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(ChMx);

        QMetaObject::connectSlotsByName(ChMx);
    } // setupUi

    void retranslateUi(QWidget *ChMx)
    {
        ChMx->setWindowTitle(QApplication::translate("ChMx", "Form", 0));
        lbNumber->setText(QApplication::translate("ChMx", "1", 0));
        btnMute->setText(QApplication::translate("ChMx", "M", 0));
        btnSolo->setText(QApplication::translate("ChMx", "S", 0));
    } // retranslateUi

};

namespace Ui {
    class ChMx: public Ui_ChMx {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHMX_H
