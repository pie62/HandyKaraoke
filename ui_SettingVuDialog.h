/********************************************************************************
** Form generated from reading UI file 'SettingVuDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETTINGVUDIALOG_H
#define UI_SETTINGVUDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_SettingVuDialog
{
public:
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout_10;
    QHBoxLayout *horizontalLayout_8;
    QFormLayout *formLayout;
    QLabel *label_2;
    QHBoxLayout *horizontalLayout_2;
    QFrame *fOn1;
    QPushButton *btnOn1;
    QLabel *label_3;
    QHBoxLayout *horizontalLayout_3;
    QFrame *fOn2;
    QPushButton *btnOn2;
    QLabel *label_4;
    QHBoxLayout *horizontalLayout_4;
    QFrame *fOn3;
    QPushButton *btnOn3;
    QLabel *label;
    QHBoxLayout *horizontalLayout;
    QFrame *fBg;
    QPushButton *btnBg;
    QFormLayout *formLayout_2;
    QLabel *label_5;
    QHBoxLayout *horizontalLayout_5;
    QFrame *fOff1;
    QPushButton *btnOff1;
    QLabel *label_6;
    QHBoxLayout *horizontalLayout_6;
    QFrame *fOff2;
    QPushButton *btnOff2;
    QLabel *label_7;
    QHBoxLayout *horizontalLayout_7;
    QFrame *fOff3;
    QPushButton *btnOff3;
    QGroupBox *groupBox_2;
    QHBoxLayout *horizontalLayout_12;
    QCheckBox *chbPh;
    QHBoxLayout *horizontalLayout_11;
    QSpinBox *spbPh;
    QLabel *lbPh;
    QSpacerItem *horizontalSpacer;
    QHBoxLayout *horizontalLayout_9;
    QPushButton *btnDefault;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *btnClose;

    void setupUi(QDialog *SettingVuDialog)
    {
        if (SettingVuDialog->objectName().isEmpty())
            SettingVuDialog->setObjectName(QStringLiteral("SettingVuDialog"));
        SettingVuDialog->resize(386, 266);
        QFont font;
        font.setPointSize(11);
        SettingVuDialog->setFont(font);
        verticalLayout = new QVBoxLayout(SettingVuDialog);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(-1, 5, -1, 9);
        groupBox = new QGroupBox(SettingVuDialog);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        horizontalLayout_10 = new QHBoxLayout(groupBox);
        horizontalLayout_10->setObjectName(QStringLiteral("horizontalLayout_10"));
        horizontalLayout_10->setContentsMargins(-1, 5, -1, 5);
        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setSpacing(12);
        horizontalLayout_8->setObjectName(QStringLiteral("horizontalLayout_8"));
        formLayout = new QFormLayout();
        formLayout->setObjectName(QStringLiteral("formLayout"));
        formLayout->setVerticalSpacing(2);
        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QStringLiteral("label_2"));

        formLayout->setWidget(0, QFormLayout::LabelRole, label_2);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        fOn1 = new QFrame(groupBox);
        fOn1->setObjectName(QStringLiteral("fOn1"));
        fOn1->setMinimumSize(QSize(61, 25));
        fOn1->setMaximumSize(QSize(61, 25));
        fOn1->setFrameShape(QFrame::StyledPanel);
        fOn1->setFrameShadow(QFrame::Raised);

        horizontalLayout_2->addWidget(fOn1);

        btnOn1 = new QPushButton(groupBox);
        btnOn1->setObjectName(QStringLiteral("btnOn1"));
        btnOn1->setMinimumSize(QSize(41, 27));
        btnOn1->setMaximumSize(QSize(41, 27));

        horizontalLayout_2->addWidget(btnOn1);


        formLayout->setLayout(0, QFormLayout::FieldRole, horizontalLayout_2);

        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QStringLiteral("label_3"));

        formLayout->setWidget(1, QFormLayout::LabelRole, label_3);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        fOn2 = new QFrame(groupBox);
        fOn2->setObjectName(QStringLiteral("fOn2"));
        fOn2->setMinimumSize(QSize(61, 25));
        fOn2->setMaximumSize(QSize(61, 25));
        fOn2->setFrameShape(QFrame::StyledPanel);
        fOn2->setFrameShadow(QFrame::Raised);

        horizontalLayout_3->addWidget(fOn2);

        btnOn2 = new QPushButton(groupBox);
        btnOn2->setObjectName(QStringLiteral("btnOn2"));
        btnOn2->setMinimumSize(QSize(41, 27));
        btnOn2->setMaximumSize(QSize(41, 27));

        horizontalLayout_3->addWidget(btnOn2);


        formLayout->setLayout(1, QFormLayout::FieldRole, horizontalLayout_3);

        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QStringLiteral("label_4"));

        formLayout->setWidget(2, QFormLayout::LabelRole, label_4);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        fOn3 = new QFrame(groupBox);
        fOn3->setObjectName(QStringLiteral("fOn3"));
        fOn3->setMinimumSize(QSize(61, 25));
        fOn3->setMaximumSize(QSize(61, 25));
        fOn3->setFrameShape(QFrame::StyledPanel);
        fOn3->setFrameShadow(QFrame::Raised);

        horizontalLayout_4->addWidget(fOn3);

        btnOn3 = new QPushButton(groupBox);
        btnOn3->setObjectName(QStringLiteral("btnOn3"));
        btnOn3->setMinimumSize(QSize(41, 27));
        btnOn3->setMaximumSize(QSize(41, 27));

        horizontalLayout_4->addWidget(btnOn3);


        formLayout->setLayout(2, QFormLayout::FieldRole, horizontalLayout_4);

        label = new QLabel(groupBox);
        label->setObjectName(QStringLiteral("label"));

        formLayout->setWidget(3, QFormLayout::LabelRole, label);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        fBg = new QFrame(groupBox);
        fBg->setObjectName(QStringLiteral("fBg"));
        fBg->setMinimumSize(QSize(61, 25));
        fBg->setMaximumSize(QSize(61, 25));
        fBg->setFrameShape(QFrame::StyledPanel);
        fBg->setFrameShadow(QFrame::Raised);

        horizontalLayout->addWidget(fBg);

        btnBg = new QPushButton(groupBox);
        btnBg->setObjectName(QStringLiteral("btnBg"));
        btnBg->setMinimumSize(QSize(41, 27));
        btnBg->setMaximumSize(QSize(41, 27));

        horizontalLayout->addWidget(btnBg);


        formLayout->setLayout(3, QFormLayout::FieldRole, horizontalLayout);


        horizontalLayout_8->addLayout(formLayout);

        formLayout_2 = new QFormLayout();
        formLayout_2->setObjectName(QStringLiteral("formLayout_2"));
        formLayout_2->setVerticalSpacing(2);
        label_5 = new QLabel(groupBox);
        label_5->setObjectName(QStringLiteral("label_5"));

        formLayout_2->setWidget(0, QFormLayout::LabelRole, label_5);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        fOff1 = new QFrame(groupBox);
        fOff1->setObjectName(QStringLiteral("fOff1"));
        fOff1->setMinimumSize(QSize(61, 25));
        fOff1->setMaximumSize(QSize(61, 25));
        fOff1->setFrameShape(QFrame::StyledPanel);
        fOff1->setFrameShadow(QFrame::Raised);

        horizontalLayout_5->addWidget(fOff1);

        btnOff1 = new QPushButton(groupBox);
        btnOff1->setObjectName(QStringLiteral("btnOff1"));
        btnOff1->setMinimumSize(QSize(41, 27));
        btnOff1->setMaximumSize(QSize(41, 27));

        horizontalLayout_5->addWidget(btnOff1);


        formLayout_2->setLayout(0, QFormLayout::FieldRole, horizontalLayout_5);

        label_6 = new QLabel(groupBox);
        label_6->setObjectName(QStringLiteral("label_6"));

        formLayout_2->setWidget(1, QFormLayout::LabelRole, label_6);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        fOff2 = new QFrame(groupBox);
        fOff2->setObjectName(QStringLiteral("fOff2"));
        fOff2->setMinimumSize(QSize(61, 25));
        fOff2->setMaximumSize(QSize(61, 25));
        fOff2->setFrameShape(QFrame::StyledPanel);
        fOff2->setFrameShadow(QFrame::Raised);

        horizontalLayout_6->addWidget(fOff2);

        btnOff2 = new QPushButton(groupBox);
        btnOff2->setObjectName(QStringLiteral("btnOff2"));
        btnOff2->setMinimumSize(QSize(41, 27));
        btnOff2->setMaximumSize(QSize(41, 27));

        horizontalLayout_6->addWidget(btnOff2);


        formLayout_2->setLayout(1, QFormLayout::FieldRole, horizontalLayout_6);

        label_7 = new QLabel(groupBox);
        label_7->setObjectName(QStringLiteral("label_7"));

        formLayout_2->setWidget(2, QFormLayout::LabelRole, label_7);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        fOff3 = new QFrame(groupBox);
        fOff3->setObjectName(QStringLiteral("fOff3"));
        fOff3->setMinimumSize(QSize(61, 25));
        fOff3->setMaximumSize(QSize(61, 25));
        fOff3->setFrameShape(QFrame::StyledPanel);
        fOff3->setFrameShadow(QFrame::Raised);

        horizontalLayout_7->addWidget(fOff3);

        btnOff3 = new QPushButton(groupBox);
        btnOff3->setObjectName(QStringLiteral("btnOff3"));
        btnOff3->setMinimumSize(QSize(41, 27));
        btnOff3->setMaximumSize(QSize(41, 27));

        horizontalLayout_7->addWidget(btnOff3);


        formLayout_2->setLayout(2, QFormLayout::FieldRole, horizontalLayout_7);


        horizontalLayout_8->addLayout(formLayout_2);


        horizontalLayout_10->addLayout(horizontalLayout_8);


        verticalLayout->addWidget(groupBox);

        groupBox_2 = new QGroupBox(SettingVuDialog);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        horizontalLayout_12 = new QHBoxLayout(groupBox_2);
        horizontalLayout_12->setSpacing(10);
        horizontalLayout_12->setObjectName(QStringLiteral("horizontalLayout_12"));
        horizontalLayout_12->setContentsMargins(-1, 5, -1, 5);
        chbPh = new QCheckBox(groupBox_2);
        chbPh->setObjectName(QStringLiteral("chbPh"));

        horizontalLayout_12->addWidget(chbPh);

        horizontalLayout_11 = new QHBoxLayout();
        horizontalLayout_11->setSpacing(2);
        horizontalLayout_11->setObjectName(QStringLiteral("horizontalLayout_11"));
        spbPh = new QSpinBox(groupBox_2);
        spbPh->setObjectName(QStringLiteral("spbPh"));
        spbPh->setMinimumSize(QSize(71, 0));
        spbPh->setMaximumSize(QSize(71, 16777215));
        spbPh->setMinimum(100);
        spbPh->setMaximum(5000);

        horizontalLayout_11->addWidget(spbPh);

        lbPh = new QLabel(groupBox_2);
        lbPh->setObjectName(QStringLiteral("lbPh"));

        horizontalLayout_11->addWidget(lbPh);


        horizontalLayout_12->addLayout(horizontalLayout_11);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_12->addItem(horizontalSpacer);


        verticalLayout->addWidget(groupBox_2);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setObjectName(QStringLiteral("horizontalLayout_9"));
        btnDefault = new QPushButton(SettingVuDialog);
        btnDefault->setObjectName(QStringLiteral("btnDefault"));

        horizontalLayout_9->addWidget(btnDefault);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_9->addItem(horizontalSpacer_2);

        btnClose = new QPushButton(SettingVuDialog);
        btnClose->setObjectName(QStringLiteral("btnClose"));

        horizontalLayout_9->addWidget(btnClose);


        verticalLayout->addLayout(horizontalLayout_9);


        retranslateUi(SettingVuDialog);

        QMetaObject::connectSlotsByName(SettingVuDialog);
    } // setupUi

    void retranslateUi(QDialog *SettingVuDialog)
    {
        SettingVuDialog->setWindowTitle(QApplication::translate("SettingVuDialog", "\340\270\225\340\270\261\340\271\211\340\270\207\340\270\204\340\271\210\340\270\262 LED Meter", 0));
        groupBox->setTitle(QApplication::translate("SettingVuDialog", "\340\270\252\340\270\265 LED Meter", 0));
        label_2->setText(QApplication::translate("SettingVuDialog", "\340\270\252\340\270\265\340\271\200\340\270\233\340\270\264\340\270\224 1 :", 0));
        btnOn1->setText(QApplication::translate("SettingVuDialog", "..", 0));
        label_3->setText(QApplication::translate("SettingVuDialog", "\340\270\252\340\270\265\340\271\200\340\270\233\340\270\264\340\270\224 2 :", 0));
        btnOn2->setText(QApplication::translate("SettingVuDialog", "..", 0));
        label_4->setText(QApplication::translate("SettingVuDialog", "\340\270\252\340\270\265\340\271\200\340\270\233\340\270\264\340\270\224 3 :", 0));
        btnOn3->setText(QApplication::translate("SettingVuDialog", "..", 0));
        label->setText(QApplication::translate("SettingVuDialog", "\340\270\252\340\270\265\340\270\236\340\270\267\340\271\211\340\270\231\340\270\253\340\270\245\340\270\261\340\270\207 :", 0));
        btnBg->setText(QApplication::translate("SettingVuDialog", "..", 0));
        label_5->setText(QApplication::translate("SettingVuDialog", "\340\270\252\340\270\265\340\270\233\340\270\264\340\270\224 1 :", 0));
        btnOff1->setText(QApplication::translate("SettingVuDialog", "..", 0));
        label_6->setText(QApplication::translate("SettingVuDialog", "\340\270\252\340\270\265\340\270\233\340\270\264\340\270\224 2 :", 0));
        btnOff2->setText(QApplication::translate("SettingVuDialog", "..", 0));
        label_7->setText(QApplication::translate("SettingVuDialog", "\340\270\252\340\270\265\340\270\233\340\270\264\340\270\224 3 :", 0));
        btnOff3->setText(QApplication::translate("SettingVuDialog", "..", 0));
        groupBox_2->setTitle(QApplication::translate("SettingVuDialog", "Peak hold", 0));
        chbPh->setText(QApplication::translate("SettingVuDialog", "\340\271\201\340\270\252\340\270\224\340\270\207 Peak hold", 0));
        lbPh->setText(QApplication::translate("SettingVuDialog", "ms", 0));
        btnDefault->setText(QApplication::translate("SettingVuDialog", "\340\270\204\340\271\210\340\270\262\340\271\200\340\270\243\340\270\264\340\271\210\340\270\241\340\270\225\340\271\211\340\270\231", 0));
        btnClose->setText(QApplication::translate("SettingVuDialog", "\340\270\233\340\270\264\340\270\224", 0));
    } // retranslateUi

};

namespace Ui {
    class SettingVuDialog: public Ui_SettingVuDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETTINGVUDIALOG_H
