/********************************************************************************
** Form generated from reading UI file 'ChorusDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHORUSDIALOG_H
#define UI_CHORUSDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDial>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>
#include "SwitchButton.h"

QT_BEGIN_NAMESPACE

class Ui_ChorusDialog
{
public:
    QVBoxLayout *verticalLayout_9;
    QHBoxLayout *horizontalLayout_3;
    SwitchButton *btnSwitch;
    QSpacerItem *horizontalSpacer;
    QPushButton *btnReset;
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout_6;
    QFormLayout *formLayout;
    QLabel *label_6;
    QComboBox *cbWaveform;
    QLabel *label_7;
    QComboBox *cbPhase;
    QFormLayout *formLayout_2;
    QDial *dialFeedback;
    QVBoxLayout *verticalLayout_5;
    QLabel *label_3;
    QSpinBox *spinFeedback;
    QVBoxLayout *verticalLayout_7;
    QFormLayout *formLayout_4;
    QDial *dialWetDryMix;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QSpinBox *spinWetDryMix;
    QFormLayout *formLayout_3;
    QDial *dialFrequency;
    QVBoxLayout *verticalLayout_4;
    QLabel *label_4;
    QSpinBox *spinFrequency;
    QVBoxLayout *verticalLayout_8;
    QFormLayout *formLayout_5;
    QDial *dialDepth;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_2;
    QSpinBox *spinDepth;
    QFormLayout *formLayout_6;
    QDial *dialDelay;
    QVBoxLayout *verticalLayout_3;
    QLabel *label_5;
    QSpinBox *spinDelay;

    void setupUi(QDialog *ChorusDialog)
    {
        if (ChorusDialog->objectName().isEmpty())
            ChorusDialog->setObjectName(QStringLiteral("ChorusDialog"));
        ChorusDialog->resize(442, 186);
        QFont font;
        font.setPointSize(10);
        ChorusDialog->setFont(font);
        verticalLayout_9 = new QVBoxLayout(ChorusDialog);
        verticalLayout_9->setSpacing(12);
        verticalLayout_9->setObjectName(QStringLiteral("verticalLayout_9"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        btnSwitch = new SwitchButton(ChorusDialog);
        btnSwitch->setObjectName(QStringLiteral("btnSwitch"));
        btnSwitch->setMinimumSize(QSize(51, 21));
        btnSwitch->setMaximumSize(QSize(51, 21));

        horizontalLayout_3->addWidget(btnSwitch);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer);

        btnReset = new QPushButton(ChorusDialog);
        btnReset->setObjectName(QStringLiteral("btnReset"));

        horizontalLayout_3->addWidget(btnReset);


        verticalLayout_9->addLayout(horizontalLayout_3);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setHorizontalSpacing(12);
        verticalLayout_6 = new QVBoxLayout();
        verticalLayout_6->setSpacing(12);
        verticalLayout_6->setObjectName(QStringLiteral("verticalLayout_6"));
        formLayout = new QFormLayout();
        formLayout->setObjectName(QStringLiteral("formLayout"));
        formLayout->setHorizontalSpacing(2);
        formLayout->setVerticalSpacing(2);
        label_6 = new QLabel(ChorusDialog);
        label_6->setObjectName(QStringLiteral("label_6"));
        QFont font1;
        font1.setBold(true);
        font1.setWeight(75);
        label_6->setFont(font1);

        formLayout->setWidget(0, QFormLayout::LabelRole, label_6);

        cbWaveform = new QComboBox(ChorusDialog);
        cbWaveform->setObjectName(QStringLiteral("cbWaveform"));
        cbWaveform->setSizeAdjustPolicy(QComboBox::AdjustToContents);

        formLayout->setWidget(0, QFormLayout::FieldRole, cbWaveform);

        label_7 = new QLabel(ChorusDialog);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setFont(font1);

        formLayout->setWidget(1, QFormLayout::LabelRole, label_7);

        cbPhase = new QComboBox(ChorusDialog);
        cbPhase->setObjectName(QStringLiteral("cbPhase"));
        cbPhase->setSizeAdjustPolicy(QComboBox::AdjustToContents);

        formLayout->setWidget(1, QFormLayout::FieldRole, cbPhase);


        verticalLayout_6->addLayout(formLayout);

        formLayout_2 = new QFormLayout();
        formLayout_2->setObjectName(QStringLiteral("formLayout_2"));
        formLayout_2->setHorizontalSpacing(2);
        dialFeedback = new QDial(ChorusDialog);
        dialFeedback->setObjectName(QStringLiteral("dialFeedback"));
        dialFeedback->setMinimumSize(QSize(51, 51));
        dialFeedback->setMaximumSize(QSize(51, 51));
        dialFeedback->setMinimum(-99);
        dialFeedback->setMaximum(99);
        dialFeedback->setNotchTarget(5.7);
        dialFeedback->setNotchesVisible(true);

        formLayout_2->setWidget(0, QFormLayout::LabelRole, dialFeedback);

        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        label_3 = new QLabel(ChorusDialog);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setFont(font1);

        verticalLayout_5->addWidget(label_3);

        spinFeedback = new QSpinBox(ChorusDialog);
        spinFeedback->setObjectName(QStringLiteral("spinFeedback"));
        spinFeedback->setMinimumSize(QSize(61, 0));
        spinFeedback->setMaximumSize(QSize(61, 16777215));
        spinFeedback->setMinimum(-99);

        verticalLayout_5->addWidget(spinFeedback);


        formLayout_2->setLayout(0, QFormLayout::FieldRole, verticalLayout_5);


        verticalLayout_6->addLayout(formLayout_2);


        gridLayout->addLayout(verticalLayout_6, 0, 0, 1, 1);

        verticalLayout_7 = new QVBoxLayout();
        verticalLayout_7->setSpacing(12);
        verticalLayout_7->setObjectName(QStringLiteral("verticalLayout_7"));
        formLayout_4 = new QFormLayout();
        formLayout_4->setObjectName(QStringLiteral("formLayout_4"));
        formLayout_4->setHorizontalSpacing(2);
        dialWetDryMix = new QDial(ChorusDialog);
        dialWetDryMix->setObjectName(QStringLiteral("dialWetDryMix"));
        dialWetDryMix->setMinimumSize(QSize(51, 51));
        dialWetDryMix->setMaximumSize(QSize(51, 51));
        dialWetDryMix->setMinimum(0);
        dialWetDryMix->setMaximum(100);
        dialWetDryMix->setNotchTarget(2.7);
        dialWetDryMix->setNotchesVisible(true);

        formLayout_4->setWidget(0, QFormLayout::LabelRole, dialWetDryMix);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        label = new QLabel(ChorusDialog);
        label->setObjectName(QStringLiteral("label"));
        label->setFont(font1);

        verticalLayout->addWidget(label);

        spinWetDryMix = new QSpinBox(ChorusDialog);
        spinWetDryMix->setObjectName(QStringLiteral("spinWetDryMix"));
        spinWetDryMix->setMinimumSize(QSize(61, 0));
        spinWetDryMix->setMaximumSize(QSize(61, 16777215));
        spinWetDryMix->setMaximum(100);

        verticalLayout->addWidget(spinWetDryMix);


        formLayout_4->setLayout(0, QFormLayout::FieldRole, verticalLayout);


        verticalLayout_7->addLayout(formLayout_4);

        formLayout_3 = new QFormLayout();
        formLayout_3->setObjectName(QStringLiteral("formLayout_3"));
        formLayout_3->setHorizontalSpacing(2);
        dialFrequency = new QDial(ChorusDialog);
        dialFrequency->setObjectName(QStringLiteral("dialFrequency"));
        dialFrequency->setMinimumSize(QSize(51, 51));
        dialFrequency->setMaximumSize(QSize(51, 51));
        dialFrequency->setMinimum(0);
        dialFrequency->setMaximum(10);
        dialFrequency->setPageStep(1);
        dialFrequency->setNotchesVisible(true);

        formLayout_3->setWidget(0, QFormLayout::LabelRole, dialFrequency);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        label_4 = new QLabel(ChorusDialog);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setFont(font1);

        verticalLayout_4->addWidget(label_4);

        spinFrequency = new QSpinBox(ChorusDialog);
        spinFrequency->setObjectName(QStringLiteral("spinFrequency"));
        spinFrequency->setMinimumSize(QSize(61, 0));
        spinFrequency->setMaximumSize(QSize(61, 16777215));
        spinFrequency->setMaximum(10);

        verticalLayout_4->addWidget(spinFrequency);


        formLayout_3->setLayout(0, QFormLayout::FieldRole, verticalLayout_4);


        verticalLayout_7->addLayout(formLayout_3);


        gridLayout->addLayout(verticalLayout_7, 0, 1, 1, 1);

        verticalLayout_8 = new QVBoxLayout();
        verticalLayout_8->setSpacing(12);
        verticalLayout_8->setObjectName(QStringLiteral("verticalLayout_8"));
        formLayout_5 = new QFormLayout();
        formLayout_5->setObjectName(QStringLiteral("formLayout_5"));
        formLayout_5->setHorizontalSpacing(2);
        dialDepth = new QDial(ChorusDialog);
        dialDepth->setObjectName(QStringLiteral("dialDepth"));
        dialDepth->setMinimumSize(QSize(51, 51));
        dialDepth->setMaximumSize(QSize(51, 51));
        dialDepth->setMinimum(0);
        dialDepth->setMaximum(100);
        dialDepth->setNotchTarget(2.7);
        dialDepth->setNotchesVisible(true);

        formLayout_5->setWidget(0, QFormLayout::LabelRole, dialDepth);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        label_2 = new QLabel(ChorusDialog);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setFont(font1);

        verticalLayout_2->addWidget(label_2);

        spinDepth = new QSpinBox(ChorusDialog);
        spinDepth->setObjectName(QStringLiteral("spinDepth"));
        spinDepth->setMinimumSize(QSize(61, 0));
        spinDepth->setMaximumSize(QSize(61, 16777215));
        spinDepth->setMaximum(100);

        verticalLayout_2->addWidget(spinDepth);


        formLayout_5->setLayout(0, QFormLayout::FieldRole, verticalLayout_2);


        verticalLayout_8->addLayout(formLayout_5);

        formLayout_6 = new QFormLayout();
        formLayout_6->setObjectName(QStringLiteral("formLayout_6"));
        formLayout_6->setHorizontalSpacing(2);
        dialDelay = new QDial(ChorusDialog);
        dialDelay->setObjectName(QStringLiteral("dialDelay"));
        dialDelay->setMinimumSize(QSize(51, 51));
        dialDelay->setMaximumSize(QSize(51, 51));
        dialDelay->setMinimum(0);
        dialDelay->setMaximum(20);
        dialDelay->setPageStep(1);
        dialDelay->setNotchTarget(2.7);
        dialDelay->setNotchesVisible(true);

        formLayout_6->setWidget(0, QFormLayout::LabelRole, dialDelay);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        label_5 = new QLabel(ChorusDialog);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setFont(font1);

        verticalLayout_3->addWidget(label_5);

        spinDelay = new QSpinBox(ChorusDialog);
        spinDelay->setObjectName(QStringLiteral("spinDelay"));
        spinDelay->setMinimumSize(QSize(61, 0));
        spinDelay->setMaximumSize(QSize(61, 16777215));
        spinDelay->setMaximum(20);

        verticalLayout_3->addWidget(spinDelay);


        formLayout_6->setLayout(0, QFormLayout::FieldRole, verticalLayout_3);


        verticalLayout_8->addLayout(formLayout_6);


        gridLayout->addLayout(verticalLayout_8, 0, 2, 1, 1);


        verticalLayout_9->addLayout(gridLayout);


        retranslateUi(ChorusDialog);

        cbWaveform->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(ChorusDialog);
    } // setupUi

    void retranslateUi(QDialog *ChorusDialog)
    {
        ChorusDialog->setWindowTitle(QApplication::translate("ChorusDialog", "Chorus", 0));
#ifndef QT_NO_TOOLTIP
        btnSwitch->setToolTip(QApplication::translate("ChorusDialog", "\340\271\200\340\270\233\340\270\264\340\270\224 / \340\270\233\340\270\264\340\270\224  \340\271\200\340\270\252\340\270\265\340\270\242\340\270\207\340\270\233\340\270\243\340\270\260\340\270\252\340\270\262\340\270\231", 0));
#endif // QT_NO_TOOLTIP
        btnReset->setText(QApplication::translate("ChorusDialog", "\340\270\243\340\270\265\340\271\200\340\270\213\340\271\207\340\270\225", 0));
        label_6->setText(QApplication::translate("ChorusDialog", "Waveform", 0));
        cbWaveform->clear();
        cbWaveform->insertItems(0, QStringList()
         << QApplication::translate("ChorusDialog", "Triangle", 0)
         << QApplication::translate("ChorusDialog", "Sine", 0)
        );
        label_7->setText(QApplication::translate("ChorusDialog", "Phase", 0));
        cbPhase->clear();
        cbPhase->insertItems(0, QStringList()
         << QApplication::translate("ChorusDialog", "-180", 0)
         << QApplication::translate("ChorusDialog", "-90", 0)
         << QApplication::translate("ChorusDialog", "0", 0)
         << QApplication::translate("ChorusDialog", "90", 0)
         << QApplication::translate("ChorusDialog", "180", 0)
        );
        label_3->setText(QApplication::translate("ChorusDialog", "Feedback", 0));
        label->setText(QApplication::translate("ChorusDialog", "WetDryMix", 0));
        label_4->setText(QApplication::translate("ChorusDialog", "Frequency", 0));
        label_2->setText(QApplication::translate("ChorusDialog", "Depth", 0));
        label_5->setText(QApplication::translate("ChorusDialog", "Delay", 0));
    } // retranslateUi

};

namespace Ui {
    class ChorusDialog: public Ui_ChorusDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHORUSDIALOG_H
