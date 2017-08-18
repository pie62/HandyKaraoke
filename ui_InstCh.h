/********************************************************************************
** Form generated from reading UI file 'InstCh.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_INSTCH_H
#define UI_INSTCH_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "LEDVu.h"
#include "Slider.h"

QT_BEGIN_NAMESPACE

class Ui_InstCh
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *btnMute;
    QPushButton *btnSolo;
    QHBoxLayout *horizontalLayout;
    Slider *slider;
    LEDVu *vuBar;
    QHBoxLayout *horizontalLayout_3;
    QLabel *lbImage;
    QLabel *lbName;

    void setupUi(QWidget *InstCh)
    {
        if (InstCh->objectName().isEmpty())
            InstCh->setObjectName(QStringLiteral("InstCh"));
        InstCh->resize(58, 217);
        verticalLayout = new QVBoxLayout(InstCh);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(2, 4, 2, 2);
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(2);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        btnMute = new QPushButton(InstCh);
        btnMute->setObjectName(QStringLiteral("btnMute"));
        btnMute->setMinimumSize(QSize(21, 17));
        btnMute->setMaximumSize(QSize(21, 17));
        btnMute->setFocusPolicy(Qt::NoFocus);

        horizontalLayout_2->addWidget(btnMute);

        btnSolo = new QPushButton(InstCh);
        btnSolo->setObjectName(QStringLiteral("btnSolo"));
        btnSolo->setMinimumSize(QSize(21, 17));
        btnSolo->setMaximumSize(QSize(21, 17));
        btnSolo->setFocusPolicy(Qt::NoFocus);

        horizontalLayout_2->addWidget(btnSolo);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(4);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        slider = new Slider(InstCh);
        slider->setObjectName(QStringLiteral("slider"));
        slider->setMinimumSize(QSize(21, 121));
        slider->setMaximumSize(QSize(21, 16777215));
        slider->setFrameShape(QFrame::StyledPanel);
        slider->setFrameShadow(QFrame::Raised);

        horizontalLayout->addWidget(slider);

        vuBar = new LEDVu(InstCh);
        vuBar->setObjectName(QStringLiteral("vuBar"));
        vuBar->setMinimumSize(QSize(10, 121));
        vuBar->setMaximumSize(QSize(10, 16777215));
        vuBar->setFrameShape(QFrame::StyledPanel);
        vuBar->setFrameShadow(QFrame::Raised);

        horizontalLayout->addWidget(vuBar);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        lbImage = new QLabel(InstCh);
        lbImage->setObjectName(QStringLiteral("lbImage"));
        lbImage->setMinimumSize(QSize(36, 35));
        lbImage->setMaximumSize(QSize(36, 35));
        lbImage->setScaledContents(true);
        lbImage->setAlignment(Qt::AlignCenter);

        horizontalLayout_3->addWidget(lbImage);


        verticalLayout->addLayout(horizontalLayout_3);

        lbName = new QLabel(InstCh);
        lbName->setObjectName(QStringLiteral("lbName"));
        QFont font;
        font.setPointSize(9);
        font.setBold(true);
        font.setWeight(75);
        lbName->setFont(font);
        lbName->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(lbName);


        retranslateUi(InstCh);

        QMetaObject::connectSlotsByName(InstCh);
    } // setupUi

    void retranslateUi(QWidget *InstCh)
    {
        InstCh->setWindowTitle(QApplication::translate("InstCh", "Form", Q_NULLPTR));
        btnMute->setText(QApplication::translate("InstCh", "M", Q_NULLPTR));
        btnSolo->setText(QApplication::translate("InstCh", "S", Q_NULLPTR));
        lbImage->setText(QString());
        lbName->setText(QApplication::translate("InstCh", "Text", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class InstCh: public Ui_InstCh {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_INSTCH_H
