/********************************************************************************
** Form generated from reading UI file 'AboutDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ABOUTDIALOG_H
#define UI_ABOUTDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_AboutDialog
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QLabel *label_5;
    QLabel *label_6;
    QSpacerItem *horizontalSpacer_2;
    QLabel *label_7;
    QLabel *label_8;

    void setupUi(QDialog *AboutDialog)
    {
        if (AboutDialog->objectName().isEmpty())
            AboutDialog->setObjectName(QStringLiteral("AboutDialog"));
        AboutDialog->resize(330, 272);
        AboutDialog->setMinimumSize(QSize(330, 272));
        AboutDialog->setMaximumSize(QSize(330, 272));
        QFont font;
        font.setPointSize(9);
        AboutDialog->setFont(font);
        verticalLayout = new QVBoxLayout(AboutDialog);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        label = new QLabel(AboutDialog);
        label->setObjectName(QStringLiteral("label"));
        label->setMinimumSize(QSize(120, 120));
        label->setMaximumSize(QSize(120, 120));
        label->setPixmap(QPixmap(QString::fromUtf8(":/Icons/App/icon.svg")));
        label->setScaledContents(true);

        horizontalLayout_2->addWidget(label);


        verticalLayout->addLayout(horizontalLayout_2);

        label_2 = new QLabel(AboutDialog);
        label_2->setObjectName(QStringLiteral("label_2"));
        QFont font1;
        font1.setPointSize(11);
        font1.setBold(true);
        font1.setWeight(75);
        label_2->setFont(font1);
        label_2->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label_2);

        label_3 = new QLabel(AboutDialog);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label_3);

        label_4 = new QLabel(AboutDialog);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label_4);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        label_5 = new QLabel(AboutDialog);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setAlignment(Qt::AlignCenter);

        horizontalLayout->addWidget(label_5);

        label_6 = new QLabel(AboutDialog);
        label_6->setObjectName(QStringLiteral("label_6"));
        QFont font2;
        font2.setBold(true);
        font2.setWeight(75);
        label_6->setFont(font2);
        label_6->setAlignment(Qt::AlignCenter);

        horizontalLayout->addWidget(label_6);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);


        verticalLayout->addLayout(horizontalLayout);

        label_7 = new QLabel(AboutDialog);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label_7);

        label_8 = new QLabel(AboutDialog);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label_8);


        retranslateUi(AboutDialog);

        QMetaObject::connectSlotsByName(AboutDialog);
    } // setupUi

    void retranslateUi(QDialog *AboutDialog)
    {
        AboutDialog->setWindowTitle(QApplication::translate("AboutDialog", "\340\271\200\340\270\201\340\270\265\340\271\210\340\270\242\340\270\247\340\270\201\340\270\261\340\270\232", Q_NULLPTR));
        label->setText(QString());
        label_2->setText(QApplication::translate("AboutDialog", "Handy Karaoke", Q_NULLPTR));
        label_3->setText(QApplication::translate("AboutDialog", "1.0.0 beta", Q_NULLPTR));
        label_4->setText(QApplication::translate("AboutDialog", "Using Qt version 5.6.2", Q_NULLPTR));
        label_5->setText(QApplication::translate("AboutDialog", "License", Q_NULLPTR));
        label_6->setText(QApplication::translate("AboutDialog", "LGPLv3 ", Q_NULLPTR));
        label_7->setText(QApplication::translate("AboutDialog", "https://github.com/pie62/HandyKaraoke", Q_NULLPTR));
        label_8->setText(QApplication::translate("AboutDialog", "(C) 2017 pie62", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class AboutDialog: public Ui_AboutDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ABOUTDIALOG_H
