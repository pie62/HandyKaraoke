/********************************************************************************
** Form generated from reading UI file 'ReverbDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REVERBDIALOG_H
#define UI_REVERBDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDial>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>
#include "SwitchButton.h"

QT_BEGIN_NAMESPACE

class Ui_ReverbDialog
{
public:
    QVBoxLayout *verticalLayout_7;
    QHBoxLayout *horizontalLayout;
    SwitchButton *btnSwitch;
    QSpacerItem *horizontalSpacer;
    QPushButton *btnReset;
    QHBoxLayout *horizontalLayout_2;
    QVBoxLayout *verticalLayout_5;
    QFormLayout *formLayout;
    QDial *dialInGain;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QSpinBox *spinInGain;
    QFormLayout *formLayout_2;
    QDial *dialRvTime;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_3;
    QSpinBox *spinRvTime;
    QVBoxLayout *verticalLayout_6;
    QFormLayout *formLayout_3;
    QDial *dialRvMix;
    QVBoxLayout *verticalLayout_3;
    QLabel *label_2;
    QSpinBox *spinRvMix;
    QFormLayout *formLayout_4;
    QDial *dialHF;
    QVBoxLayout *verticalLayout_4;
    QLabel *label_4;
    QDoubleSpinBox *spinHF;

    void setupUi(QDialog *ReverbDialog)
    {
        if (ReverbDialog->objectName().isEmpty())
            ReverbDialog->setObjectName(QStringLiteral("ReverbDialog"));
        ReverbDialog->resize(344, 183);
        QFont font;
        font.setPointSize(10);
        ReverbDialog->setFont(font);
        verticalLayout_7 = new QVBoxLayout(ReverbDialog);
        verticalLayout_7->setObjectName(QStringLiteral("verticalLayout_7"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        btnSwitch = new SwitchButton(ReverbDialog);
        btnSwitch->setObjectName(QStringLiteral("btnSwitch"));
        btnSwitch->setMinimumSize(QSize(51, 21));
        btnSwitch->setMaximumSize(QSize(51, 21));

        horizontalLayout->addWidget(btnSwitch);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        btnReset = new QPushButton(ReverbDialog);
        btnReset->setObjectName(QStringLiteral("btnReset"));

        horizontalLayout->addWidget(btnReset);


        verticalLayout_7->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(24);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setSpacing(15);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        formLayout = new QFormLayout();
        formLayout->setObjectName(QStringLiteral("formLayout"));
        formLayout->setHorizontalSpacing(2);
        dialInGain = new QDial(ReverbDialog);
        dialInGain->setObjectName(QStringLiteral("dialInGain"));
        dialInGain->setMinimumSize(QSize(51, 51));
        dialInGain->setMaximumSize(QSize(51, 51));
        dialInGain->setMinimum(-96);
        dialInGain->setMaximum(0);
        dialInGain->setNotchTarget(2.7);
        dialInGain->setNotchesVisible(true);

        formLayout->setWidget(0, QFormLayout::LabelRole, dialInGain);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        label = new QLabel(ReverbDialog);
        label->setObjectName(QStringLiteral("label"));
        QFont font1;
        font1.setBold(true);
        font1.setWeight(75);
        label->setFont(font1);

        verticalLayout->addWidget(label);

        spinInGain = new QSpinBox(ReverbDialog);
        spinInGain->setObjectName(QStringLiteral("spinInGain"));
        spinInGain->setMinimumSize(QSize(61, 0));
        spinInGain->setMaximumSize(QSize(61, 16777215));
        spinInGain->setMinimum(-96);
        spinInGain->setMaximum(0);
        spinInGain->setValue(-96);

        verticalLayout->addWidget(spinInGain);


        formLayout->setLayout(0, QFormLayout::FieldRole, verticalLayout);


        verticalLayout_5->addLayout(formLayout);

        formLayout_2 = new QFormLayout();
        formLayout_2->setObjectName(QStringLiteral("formLayout_2"));
        formLayout_2->setHorizontalSpacing(2);
        dialRvTime = new QDial(ReverbDialog);
        dialRvTime->setObjectName(QStringLiteral("dialRvTime"));
        dialRvTime->setMinimumSize(QSize(51, 51));
        dialRvTime->setMaximumSize(QSize(51, 51));
        dialRvTime->setMinimum(1);
        dialRvTime->setMaximum(3000);
        dialRvTime->setNotchTarget(34.7);
        dialRvTime->setNotchesVisible(true);

        formLayout_2->setWidget(0, QFormLayout::LabelRole, dialRvTime);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        label_3 = new QLabel(ReverbDialog);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setFont(font1);

        verticalLayout_2->addWidget(label_3);

        spinRvTime = new QSpinBox(ReverbDialog);
        spinRvTime->setObjectName(QStringLiteral("spinRvTime"));
        spinRvTime->setMinimumSize(QSize(61, 0));
        spinRvTime->setMaximumSize(QSize(61, 16777215));
        spinRvTime->setMinimum(1);
        spinRvTime->setMaximum(3000);

        verticalLayout_2->addWidget(spinRvTime);


        formLayout_2->setLayout(0, QFormLayout::FieldRole, verticalLayout_2);


        verticalLayout_5->addLayout(formLayout_2);


        horizontalLayout_2->addLayout(verticalLayout_5);

        verticalLayout_6 = new QVBoxLayout();
        verticalLayout_6->setSpacing(15);
        verticalLayout_6->setObjectName(QStringLiteral("verticalLayout_6"));
        formLayout_3 = new QFormLayout();
        formLayout_3->setObjectName(QStringLiteral("formLayout_3"));
        formLayout_3->setHorizontalSpacing(2);
        dialRvMix = new QDial(ReverbDialog);
        dialRvMix->setObjectName(QStringLiteral("dialRvMix"));
        dialRvMix->setMinimumSize(QSize(51, 51));
        dialRvMix->setMaximumSize(QSize(51, 51));
        dialRvMix->setMinimum(-96);
        dialRvMix->setMaximum(0);
        dialRvMix->setNotchTarget(2.7);
        dialRvMix->setNotchesVisible(true);

        formLayout_3->setWidget(0, QFormLayout::LabelRole, dialRvMix);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(0);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        label_2 = new QLabel(ReverbDialog);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setFont(font1);

        verticalLayout_3->addWidget(label_2);

        spinRvMix = new QSpinBox(ReverbDialog);
        spinRvMix->setObjectName(QStringLiteral("spinRvMix"));
        spinRvMix->setMinimumSize(QSize(61, 0));
        spinRvMix->setMaximumSize(QSize(61, 16777215));
        spinRvMix->setMinimum(-96);
        spinRvMix->setMaximum(0);

        verticalLayout_3->addWidget(spinRvMix);


        formLayout_3->setLayout(0, QFormLayout::FieldRole, verticalLayout_3);


        verticalLayout_6->addLayout(formLayout_3);

        formLayout_4 = new QFormLayout();
        formLayout_4->setObjectName(QStringLiteral("formLayout_4"));
        formLayout_4->setHorizontalSpacing(2);
        dialHF = new QDial(ReverbDialog);
        dialHF->setObjectName(QStringLiteral("dialHF"));
        dialHF->setMinimumSize(QSize(51, 51));
        dialHF->setMaximumSize(QSize(51, 51));
        dialHF->setMinimum(1);
        dialHF->setMaximum(999);
        dialHF->setNotchTarget(34.7);
        dialHF->setNotchesVisible(true);

        formLayout_4->setWidget(0, QFormLayout::LabelRole, dialHF);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setSpacing(0);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        label_4 = new QLabel(ReverbDialog);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setFont(font1);

        verticalLayout_4->addWidget(label_4);

        spinHF = new QDoubleSpinBox(ReverbDialog);
        spinHF->setObjectName(QStringLiteral("spinHF"));
        spinHF->setMinimumSize(QSize(61, 0));
        spinHF->setMaximumSize(QSize(61, 16777215));
        spinHF->setDecimals(3);
        spinHF->setMinimum(0.001);
        spinHF->setMaximum(0.999);
        spinHF->setSingleStep(0);

        verticalLayout_4->addWidget(spinHF);


        formLayout_4->setLayout(0, QFormLayout::FieldRole, verticalLayout_4);


        verticalLayout_6->addLayout(formLayout_4);


        horizontalLayout_2->addLayout(verticalLayout_6);


        verticalLayout_7->addLayout(horizontalLayout_2);


        retranslateUi(ReverbDialog);

        QMetaObject::connectSlotsByName(ReverbDialog);
    } // setupUi

    void retranslateUi(QDialog *ReverbDialog)
    {
        ReverbDialog->setWindowTitle(QApplication::translate("ReverbDialog", "Reverb", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        btnSwitch->setToolTip(QApplication::translate("ReverbDialog", "\340\271\200\340\270\233\340\270\264\340\270\224 / \340\270\233\340\270\264\340\270\224  \340\271\200\340\270\252\340\270\265\340\270\242\340\270\207\340\270\201\340\271\211\340\270\255\340\270\207", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        btnReset->setText(QApplication::translate("ReverbDialog", "\340\270\243\340\270\265\340\271\200\340\270\213\340\271\207\340\270\225", Q_NULLPTR));
        label->setText(QApplication::translate("ReverbDialog", "InGain", Q_NULLPTR));
        label_3->setText(QApplication::translate("ReverbDialog", "ReverbTime", Q_NULLPTR));
        label_2->setText(QApplication::translate("ReverbDialog", "ReverbMix", Q_NULLPTR));
        label_4->setText(QApplication::translate("ReverbDialog", "HighFreqRTRatio", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class ReverbDialog: public Ui_ReverbDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REVERBDIALOG_H
