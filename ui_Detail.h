/********************************************************************************
** Form generated from reading UI file 'Detail.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DETAIL_H
#define UI_DETAIL_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Detail
{
public:
    QHBoxLayout *horizontalLayout;
    QFrame *frame;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_2;
    QLabel *lbText;
    QLabel *lbValue;
    QSpacerItem *horizontalSpacer;

    void setupUi(QWidget *Detail)
    {
        if (Detail->objectName().isEmpty())
            Detail->setObjectName(QStringLiteral("Detail"));
        Detail->resize(203, 52);
        Detail->setStyleSheet(QLatin1String("#frame {\n"
"background-color: rgb(62, 62, 62, 200);\n"
"border:1px solid rgb(255, 255, 255);\n"
"}\n"
"\n"
"QLabel {\n"
"color: rgb(255, 255, 255);\n"
"	font: bold 12pt;\n"
"}"));
        horizontalLayout = new QHBoxLayout(Detail);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        frame = new QFrame(Detail);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        horizontalLayout_2 = new QHBoxLayout(frame);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        lbText = new QLabel(frame);
        lbText->setObjectName(QStringLiteral("lbText"));

        horizontalLayout_2->addWidget(lbText);

        lbValue = new QLabel(frame);
        lbValue->setObjectName(QStringLiteral("lbValue"));

        horizontalLayout_2->addWidget(lbValue);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);


        horizontalLayout->addWidget(frame);


        retranslateUi(Detail);

        QMetaObject::connectSlotsByName(Detail);
    } // setupUi

    void retranslateUi(QWidget *Detail)
    {
        Detail->setWindowTitle(QApplication::translate("Detail", "Form", Q_NULLPTR));
        lbText->setText(QString());
        lbValue->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class Detail: public Ui_Detail {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DETAIL_H
