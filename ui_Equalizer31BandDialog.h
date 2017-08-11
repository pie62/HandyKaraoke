/********************************************************************************
** Form generated from reading UI file 'Equalizer31BandDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EQUALIZER31BANDDIALOG_H
#define UI_EQUALIZER31BANDDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include "Slider.h"
#include "SwitchButton.h"

QT_BEGIN_NAMESPACE

class Ui_Equalizer31BandDialog
{
public:
    QVBoxLayout *verticalLayout_37;
    QHBoxLayout *horizontalLayout;
    SwitchButton *btnSwitch;
    QSpacerItem *horizontalSpacer;
    QPushButton *btnReset;
    QHBoxLayout *horizontalLayout_2;
    QVBoxLayout *verticalLayout_7;
    QLabel *lbdb_7;
    QGridLayout *gridLayout_7;
    Slider *slider_7;
    QLabel *label_7;
    QVBoxLayout *verticalLayout_8;
    QLabel *lbdb_8;
    QGridLayout *gridLayout_8;
    Slider *slider_8;
    QLabel *label_8;
    QVBoxLayout *verticalLayout_9;
    QLabel *lbdb_9;
    QGridLayout *gridLayout_9;
    Slider *slider_9;
    QLabel *label_9;
    QVBoxLayout *verticalLayout_10;
    QLabel *lbdb_10;
    QGridLayout *gridLayout_10;
    Slider *slider_10;
    QLabel *label_10;
    QVBoxLayout *verticalLayout_11;
    QLabel *lbdb_11;
    QGridLayout *gridLayout_11;
    Slider *slider_11;
    QLabel *label_11;
    QVBoxLayout *verticalLayout_12;
    QLabel *lbdb_12;
    QGridLayout *gridLayout_12;
    Slider *slider_12;
    QLabel *label_12;
    QVBoxLayout *verticalLayout_13;
    QLabel *lbdb_13;
    QGridLayout *gridLayout_13;
    Slider *slider_13;
    QLabel *label_13;
    QVBoxLayout *verticalLayout_14;
    QLabel *lbdb_14;
    QGridLayout *gridLayout_14;
    Slider *slider_14;
    QLabel *label_14;
    QVBoxLayout *verticalLayout_15;
    QLabel *lbdb_15;
    QGridLayout *gridLayout_15;
    Slider *slider_15;
    QLabel *label_15;
    QVBoxLayout *verticalLayout_16;
    QLabel *lbdb_16;
    QGridLayout *gridLayout_16;
    Slider *slider_16;
    QLabel *label_16;
    QVBoxLayout *verticalLayout_17;
    QLabel *lbdb_17;
    QGridLayout *gridLayout_17;
    Slider *slider_17;
    QLabel *label_17;
    QVBoxLayout *verticalLayout_18;
    QLabel *lbdb_18;
    QGridLayout *gridLayout_18;
    Slider *slider_18;
    QLabel *label_18;
    QVBoxLayout *verticalLayout_19;
    QLabel *lbdb_19;
    QGridLayout *gridLayout_19;
    Slider *slider_19;
    QLabel *label_19;
    QVBoxLayout *verticalLayout_20;
    QLabel *lbdb_20;
    QGridLayout *gridLayout_20;
    Slider *slider_20;
    QLabel *label_20;
    QVBoxLayout *verticalLayout_21;
    QLabel *lbdb_21;
    QGridLayout *gridLayout_21;
    Slider *slider_21;
    QLabel *label_21;
    QVBoxLayout *verticalLayout_22;
    QLabel *lbdb_22;
    QGridLayout *gridLayout_22;
    Slider *slider_22;
    QLabel *label_22;
    QVBoxLayout *verticalLayout_23;
    QLabel *lbdb_23;
    QGridLayout *gridLayout_23;
    Slider *slider_23;
    QLabel *label_23;
    QVBoxLayout *verticalLayout_24;
    QLabel *lbdb_24;
    QGridLayout *gridLayout_24;
    Slider *slider_24;
    QLabel *label_24;
    QVBoxLayout *verticalLayout_25;
    QLabel *lbdb_25;
    QGridLayout *gridLayout_25;
    Slider *slider_25;
    QLabel *label_25;
    QVBoxLayout *verticalLayout_26;
    QLabel *lbdb_26;
    QGridLayout *gridLayout_26;
    Slider *slider_26;
    QLabel *label_26;
    QVBoxLayout *verticalLayout_27;
    QLabel *lbdb_27;
    QGridLayout *gridLayout_27;
    Slider *slider_27;
    QLabel *label_27;
    QVBoxLayout *verticalLayout_28;
    QLabel *lbdb_28;
    QGridLayout *gridLayout_28;
    Slider *slider_28;
    QLabel *label_28;
    QVBoxLayout *verticalLayout_29;
    QLabel *lbdb_29;
    QGridLayout *gridLayout_29;
    Slider *slider_29;
    QLabel *label_29;
    QVBoxLayout *verticalLayout_30;
    QLabel *lbdb_30;
    QGridLayout *gridLayout_30;
    Slider *slider_30;
    QLabel *label_30;

    void setupUi(QDialog *Equalizer31BandDialog)
    {
        if (Equalizer31BandDialog->objectName().isEmpty())
            Equalizer31BandDialog->setObjectName(QStringLiteral("Equalizer31BandDialog"));
        Equalizer31BandDialog->resize(1037, 235);
        QFont font;
        font.setPointSize(10);
        Equalizer31BandDialog->setFont(font);
        verticalLayout_37 = new QVBoxLayout(Equalizer31BandDialog);
        verticalLayout_37->setObjectName(QStringLiteral("verticalLayout_37"));
        verticalLayout_37->setContentsMargins(6, 6, 6, 6);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        btnSwitch = new SwitchButton(Equalizer31BandDialog);
        btnSwitch->setObjectName(QStringLiteral("btnSwitch"));
        btnSwitch->setMinimumSize(QSize(51, 21));
        btnSwitch->setMaximumSize(QSize(51, 21));

        horizontalLayout->addWidget(btnSwitch);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        btnReset = new QPushButton(Equalizer31BandDialog);
        btnReset->setObjectName(QStringLiteral("btnReset"));

        horizontalLayout->addWidget(btnReset);


        verticalLayout_37->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        verticalLayout_7 = new QVBoxLayout();
        verticalLayout_7->setObjectName(QStringLiteral("verticalLayout_7"));
        lbdb_7 = new QLabel(Equalizer31BandDialog);
        lbdb_7->setObjectName(QStringLiteral("lbdb_7"));
        QFont font1;
        font1.setPointSize(8);
        lbdb_7->setFont(font1);
        lbdb_7->setAlignment(Qt::AlignCenter);

        verticalLayout_7->addWidget(lbdb_7);

        gridLayout_7 = new QGridLayout();
        gridLayout_7->setObjectName(QStringLiteral("gridLayout_7"));
        slider_7 = new Slider(Equalizer31BandDialog);
        slider_7->setObjectName(QStringLiteral("slider_7"));
        slider_7->setMinimumSize(QSize(19, 141));
        slider_7->setMaximumSize(QSize(19, 141));
        slider_7->setFrameShape(QFrame::StyledPanel);
        slider_7->setFrameShadow(QFrame::Raised);

        gridLayout_7->addWidget(slider_7, 0, 0, 1, 1);


        verticalLayout_7->addLayout(gridLayout_7);

        label_7 = new QLabel(Equalizer31BandDialog);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setMinimumSize(QSize(31, 0));
        label_7->setMaximumSize(QSize(31, 16777215));
        label_7->setFont(font1);
        label_7->setAlignment(Qt::AlignCenter);

        verticalLayout_7->addWidget(label_7);


        horizontalLayout_2->addLayout(verticalLayout_7);

        verticalLayout_8 = new QVBoxLayout();
        verticalLayout_8->setObjectName(QStringLiteral("verticalLayout_8"));
        lbdb_8 = new QLabel(Equalizer31BandDialog);
        lbdb_8->setObjectName(QStringLiteral("lbdb_8"));
        lbdb_8->setFont(font1);
        lbdb_8->setAlignment(Qt::AlignCenter);

        verticalLayout_8->addWidget(lbdb_8);

        gridLayout_8 = new QGridLayout();
        gridLayout_8->setObjectName(QStringLiteral("gridLayout_8"));
        slider_8 = new Slider(Equalizer31BandDialog);
        slider_8->setObjectName(QStringLiteral("slider_8"));
        slider_8->setMinimumSize(QSize(19, 141));
        slider_8->setMaximumSize(QSize(19, 141));
        slider_8->setFrameShape(QFrame::StyledPanel);
        slider_8->setFrameShadow(QFrame::Raised);

        gridLayout_8->addWidget(slider_8, 0, 0, 1, 1);


        verticalLayout_8->addLayout(gridLayout_8);

        label_8 = new QLabel(Equalizer31BandDialog);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setMinimumSize(QSize(31, 0));
        label_8->setMaximumSize(QSize(31, 16777215));
        label_8->setFont(font1);
        label_8->setAlignment(Qt::AlignCenter);

        verticalLayout_8->addWidget(label_8);


        horizontalLayout_2->addLayout(verticalLayout_8);

        verticalLayout_9 = new QVBoxLayout();
        verticalLayout_9->setObjectName(QStringLiteral("verticalLayout_9"));
        lbdb_9 = new QLabel(Equalizer31BandDialog);
        lbdb_9->setObjectName(QStringLiteral("lbdb_9"));
        lbdb_9->setFont(font1);
        lbdb_9->setAlignment(Qt::AlignCenter);

        verticalLayout_9->addWidget(lbdb_9);

        gridLayout_9 = new QGridLayout();
        gridLayout_9->setObjectName(QStringLiteral("gridLayout_9"));
        slider_9 = new Slider(Equalizer31BandDialog);
        slider_9->setObjectName(QStringLiteral("slider_9"));
        slider_9->setMinimumSize(QSize(19, 141));
        slider_9->setMaximumSize(QSize(19, 141));
        slider_9->setFrameShape(QFrame::StyledPanel);
        slider_9->setFrameShadow(QFrame::Raised);

        gridLayout_9->addWidget(slider_9, 0, 0, 1, 1);


        verticalLayout_9->addLayout(gridLayout_9);

        label_9 = new QLabel(Equalizer31BandDialog);
        label_9->setObjectName(QStringLiteral("label_9"));
        label_9->setMinimumSize(QSize(31, 0));
        label_9->setMaximumSize(QSize(31, 16777215));
        label_9->setFont(font1);
        label_9->setAlignment(Qt::AlignCenter);

        verticalLayout_9->addWidget(label_9);


        horizontalLayout_2->addLayout(verticalLayout_9);

        verticalLayout_10 = new QVBoxLayout();
        verticalLayout_10->setObjectName(QStringLiteral("verticalLayout_10"));
        lbdb_10 = new QLabel(Equalizer31BandDialog);
        lbdb_10->setObjectName(QStringLiteral("lbdb_10"));
        lbdb_10->setFont(font1);
        lbdb_10->setAlignment(Qt::AlignCenter);

        verticalLayout_10->addWidget(lbdb_10);

        gridLayout_10 = new QGridLayout();
        gridLayout_10->setObjectName(QStringLiteral("gridLayout_10"));
        slider_10 = new Slider(Equalizer31BandDialog);
        slider_10->setObjectName(QStringLiteral("slider_10"));
        slider_10->setMinimumSize(QSize(19, 141));
        slider_10->setMaximumSize(QSize(19, 141));
        slider_10->setFrameShape(QFrame::StyledPanel);
        slider_10->setFrameShadow(QFrame::Raised);

        gridLayout_10->addWidget(slider_10, 0, 0, 1, 1);


        verticalLayout_10->addLayout(gridLayout_10);

        label_10 = new QLabel(Equalizer31BandDialog);
        label_10->setObjectName(QStringLiteral("label_10"));
        label_10->setMinimumSize(QSize(31, 0));
        label_10->setMaximumSize(QSize(31, 16777215));
        label_10->setFont(font1);
        label_10->setAlignment(Qt::AlignCenter);

        verticalLayout_10->addWidget(label_10);


        horizontalLayout_2->addLayout(verticalLayout_10);

        verticalLayout_11 = new QVBoxLayout();
        verticalLayout_11->setObjectName(QStringLiteral("verticalLayout_11"));
        lbdb_11 = new QLabel(Equalizer31BandDialog);
        lbdb_11->setObjectName(QStringLiteral("lbdb_11"));
        lbdb_11->setFont(font1);
        lbdb_11->setAlignment(Qt::AlignCenter);

        verticalLayout_11->addWidget(lbdb_11);

        gridLayout_11 = new QGridLayout();
        gridLayout_11->setObjectName(QStringLiteral("gridLayout_11"));
        slider_11 = new Slider(Equalizer31BandDialog);
        slider_11->setObjectName(QStringLiteral("slider_11"));
        slider_11->setMinimumSize(QSize(19, 141));
        slider_11->setMaximumSize(QSize(19, 141));
        slider_11->setFrameShape(QFrame::StyledPanel);
        slider_11->setFrameShadow(QFrame::Raised);

        gridLayout_11->addWidget(slider_11, 0, 0, 1, 1);


        verticalLayout_11->addLayout(gridLayout_11);

        label_11 = new QLabel(Equalizer31BandDialog);
        label_11->setObjectName(QStringLiteral("label_11"));
        label_11->setMinimumSize(QSize(31, 0));
        label_11->setMaximumSize(QSize(31, 16777215));
        label_11->setFont(font1);
        label_11->setAlignment(Qt::AlignCenter);

        verticalLayout_11->addWidget(label_11);


        horizontalLayout_2->addLayout(verticalLayout_11);

        verticalLayout_12 = new QVBoxLayout();
        verticalLayout_12->setObjectName(QStringLiteral("verticalLayout_12"));
        lbdb_12 = new QLabel(Equalizer31BandDialog);
        lbdb_12->setObjectName(QStringLiteral("lbdb_12"));
        lbdb_12->setFont(font1);
        lbdb_12->setAlignment(Qt::AlignCenter);

        verticalLayout_12->addWidget(lbdb_12);

        gridLayout_12 = new QGridLayout();
        gridLayout_12->setObjectName(QStringLiteral("gridLayout_12"));
        slider_12 = new Slider(Equalizer31BandDialog);
        slider_12->setObjectName(QStringLiteral("slider_12"));
        slider_12->setMinimumSize(QSize(19, 141));
        slider_12->setMaximumSize(QSize(19, 141));
        slider_12->setFrameShape(QFrame::StyledPanel);
        slider_12->setFrameShadow(QFrame::Raised);

        gridLayout_12->addWidget(slider_12, 0, 0, 1, 1);


        verticalLayout_12->addLayout(gridLayout_12);

        label_12 = new QLabel(Equalizer31BandDialog);
        label_12->setObjectName(QStringLiteral("label_12"));
        label_12->setMinimumSize(QSize(31, 0));
        label_12->setMaximumSize(QSize(31, 16777215));
        label_12->setFont(font1);
        label_12->setAlignment(Qt::AlignCenter);

        verticalLayout_12->addWidget(label_12);


        horizontalLayout_2->addLayout(verticalLayout_12);

        verticalLayout_13 = new QVBoxLayout();
        verticalLayout_13->setObjectName(QStringLiteral("verticalLayout_13"));
        lbdb_13 = new QLabel(Equalizer31BandDialog);
        lbdb_13->setObjectName(QStringLiteral("lbdb_13"));
        lbdb_13->setFont(font1);
        lbdb_13->setAlignment(Qt::AlignCenter);

        verticalLayout_13->addWidget(lbdb_13);

        gridLayout_13 = new QGridLayout();
        gridLayout_13->setObjectName(QStringLiteral("gridLayout_13"));
        slider_13 = new Slider(Equalizer31BandDialog);
        slider_13->setObjectName(QStringLiteral("slider_13"));
        slider_13->setMinimumSize(QSize(19, 141));
        slider_13->setMaximumSize(QSize(19, 141));
        slider_13->setFrameShape(QFrame::StyledPanel);
        slider_13->setFrameShadow(QFrame::Raised);

        gridLayout_13->addWidget(slider_13, 0, 0, 1, 1);


        verticalLayout_13->addLayout(gridLayout_13);

        label_13 = new QLabel(Equalizer31BandDialog);
        label_13->setObjectName(QStringLiteral("label_13"));
        label_13->setMinimumSize(QSize(31, 0));
        label_13->setMaximumSize(QSize(31, 16777215));
        label_13->setFont(font1);
        label_13->setAlignment(Qt::AlignCenter);

        verticalLayout_13->addWidget(label_13);


        horizontalLayout_2->addLayout(verticalLayout_13);

        verticalLayout_14 = new QVBoxLayout();
        verticalLayout_14->setObjectName(QStringLiteral("verticalLayout_14"));
        lbdb_14 = new QLabel(Equalizer31BandDialog);
        lbdb_14->setObjectName(QStringLiteral("lbdb_14"));
        lbdb_14->setFont(font1);
        lbdb_14->setAlignment(Qt::AlignCenter);

        verticalLayout_14->addWidget(lbdb_14);

        gridLayout_14 = new QGridLayout();
        gridLayout_14->setObjectName(QStringLiteral("gridLayout_14"));
        slider_14 = new Slider(Equalizer31BandDialog);
        slider_14->setObjectName(QStringLiteral("slider_14"));
        slider_14->setMinimumSize(QSize(19, 141));
        slider_14->setMaximumSize(QSize(19, 141));
        slider_14->setFrameShape(QFrame::StyledPanel);
        slider_14->setFrameShadow(QFrame::Raised);

        gridLayout_14->addWidget(slider_14, 0, 0, 1, 1);


        verticalLayout_14->addLayout(gridLayout_14);

        label_14 = new QLabel(Equalizer31BandDialog);
        label_14->setObjectName(QStringLiteral("label_14"));
        label_14->setMinimumSize(QSize(31, 0));
        label_14->setMaximumSize(QSize(31, 16777215));
        label_14->setFont(font1);
        label_14->setAlignment(Qt::AlignCenter);

        verticalLayout_14->addWidget(label_14);


        horizontalLayout_2->addLayout(verticalLayout_14);

        verticalLayout_15 = new QVBoxLayout();
        verticalLayout_15->setObjectName(QStringLiteral("verticalLayout_15"));
        lbdb_15 = new QLabel(Equalizer31BandDialog);
        lbdb_15->setObjectName(QStringLiteral("lbdb_15"));
        lbdb_15->setFont(font1);
        lbdb_15->setAlignment(Qt::AlignCenter);

        verticalLayout_15->addWidget(lbdb_15);

        gridLayout_15 = new QGridLayout();
        gridLayout_15->setObjectName(QStringLiteral("gridLayout_15"));
        slider_15 = new Slider(Equalizer31BandDialog);
        slider_15->setObjectName(QStringLiteral("slider_15"));
        slider_15->setMinimumSize(QSize(19, 141));
        slider_15->setMaximumSize(QSize(19, 141));
        slider_15->setFrameShape(QFrame::StyledPanel);
        slider_15->setFrameShadow(QFrame::Raised);

        gridLayout_15->addWidget(slider_15, 0, 0, 1, 1);


        verticalLayout_15->addLayout(gridLayout_15);

        label_15 = new QLabel(Equalizer31BandDialog);
        label_15->setObjectName(QStringLiteral("label_15"));
        label_15->setMinimumSize(QSize(31, 0));
        label_15->setMaximumSize(QSize(31, 16777215));
        label_15->setFont(font1);
        label_15->setAlignment(Qt::AlignCenter);

        verticalLayout_15->addWidget(label_15);


        horizontalLayout_2->addLayout(verticalLayout_15);

        verticalLayout_16 = new QVBoxLayout();
        verticalLayout_16->setObjectName(QStringLiteral("verticalLayout_16"));
        lbdb_16 = new QLabel(Equalizer31BandDialog);
        lbdb_16->setObjectName(QStringLiteral("lbdb_16"));
        lbdb_16->setFont(font1);
        lbdb_16->setAlignment(Qt::AlignCenter);

        verticalLayout_16->addWidget(lbdb_16);

        gridLayout_16 = new QGridLayout();
        gridLayout_16->setObjectName(QStringLiteral("gridLayout_16"));
        slider_16 = new Slider(Equalizer31BandDialog);
        slider_16->setObjectName(QStringLiteral("slider_16"));
        slider_16->setMinimumSize(QSize(19, 141));
        slider_16->setMaximumSize(QSize(19, 141));
        slider_16->setFrameShape(QFrame::StyledPanel);
        slider_16->setFrameShadow(QFrame::Raised);

        gridLayout_16->addWidget(slider_16, 0, 0, 1, 1);


        verticalLayout_16->addLayout(gridLayout_16);

        label_16 = new QLabel(Equalizer31BandDialog);
        label_16->setObjectName(QStringLiteral("label_16"));
        label_16->setMinimumSize(QSize(31, 0));
        label_16->setMaximumSize(QSize(31, 16777215));
        label_16->setFont(font1);
        label_16->setAlignment(Qt::AlignCenter);

        verticalLayout_16->addWidget(label_16);


        horizontalLayout_2->addLayout(verticalLayout_16);

        verticalLayout_17 = new QVBoxLayout();
        verticalLayout_17->setObjectName(QStringLiteral("verticalLayout_17"));
        lbdb_17 = new QLabel(Equalizer31BandDialog);
        lbdb_17->setObjectName(QStringLiteral("lbdb_17"));
        lbdb_17->setFont(font1);
        lbdb_17->setAlignment(Qt::AlignCenter);

        verticalLayout_17->addWidget(lbdb_17);

        gridLayout_17 = new QGridLayout();
        gridLayout_17->setObjectName(QStringLiteral("gridLayout_17"));
        slider_17 = new Slider(Equalizer31BandDialog);
        slider_17->setObjectName(QStringLiteral("slider_17"));
        slider_17->setMinimumSize(QSize(19, 141));
        slider_17->setMaximumSize(QSize(19, 141));
        slider_17->setFrameShape(QFrame::StyledPanel);
        slider_17->setFrameShadow(QFrame::Raised);

        gridLayout_17->addWidget(slider_17, 0, 0, 1, 1);


        verticalLayout_17->addLayout(gridLayout_17);

        label_17 = new QLabel(Equalizer31BandDialog);
        label_17->setObjectName(QStringLiteral("label_17"));
        label_17->setMinimumSize(QSize(31, 0));
        label_17->setMaximumSize(QSize(31, 16777215));
        label_17->setFont(font1);
        label_17->setAlignment(Qt::AlignCenter);

        verticalLayout_17->addWidget(label_17);


        horizontalLayout_2->addLayout(verticalLayout_17);

        verticalLayout_18 = new QVBoxLayout();
        verticalLayout_18->setObjectName(QStringLiteral("verticalLayout_18"));
        lbdb_18 = new QLabel(Equalizer31BandDialog);
        lbdb_18->setObjectName(QStringLiteral("lbdb_18"));
        lbdb_18->setFont(font1);
        lbdb_18->setAlignment(Qt::AlignCenter);

        verticalLayout_18->addWidget(lbdb_18);

        gridLayout_18 = new QGridLayout();
        gridLayout_18->setObjectName(QStringLiteral("gridLayout_18"));
        slider_18 = new Slider(Equalizer31BandDialog);
        slider_18->setObjectName(QStringLiteral("slider_18"));
        slider_18->setMinimumSize(QSize(19, 141));
        slider_18->setMaximumSize(QSize(19, 141));
        slider_18->setFrameShape(QFrame::StyledPanel);
        slider_18->setFrameShadow(QFrame::Raised);

        gridLayout_18->addWidget(slider_18, 0, 0, 1, 1);


        verticalLayout_18->addLayout(gridLayout_18);

        label_18 = new QLabel(Equalizer31BandDialog);
        label_18->setObjectName(QStringLiteral("label_18"));
        label_18->setMinimumSize(QSize(31, 0));
        label_18->setMaximumSize(QSize(31, 16777215));
        label_18->setFont(font1);
        label_18->setAlignment(Qt::AlignCenter);

        verticalLayout_18->addWidget(label_18);


        horizontalLayout_2->addLayout(verticalLayout_18);

        verticalLayout_19 = new QVBoxLayout();
        verticalLayout_19->setObjectName(QStringLiteral("verticalLayout_19"));
        lbdb_19 = new QLabel(Equalizer31BandDialog);
        lbdb_19->setObjectName(QStringLiteral("lbdb_19"));
        lbdb_19->setFont(font1);
        lbdb_19->setAlignment(Qt::AlignCenter);

        verticalLayout_19->addWidget(lbdb_19);

        gridLayout_19 = new QGridLayout();
        gridLayout_19->setObjectName(QStringLiteral("gridLayout_19"));
        slider_19 = new Slider(Equalizer31BandDialog);
        slider_19->setObjectName(QStringLiteral("slider_19"));
        slider_19->setMinimumSize(QSize(19, 141));
        slider_19->setMaximumSize(QSize(19, 141));
        slider_19->setFrameShape(QFrame::StyledPanel);
        slider_19->setFrameShadow(QFrame::Raised);

        gridLayout_19->addWidget(slider_19, 0, 0, 1, 1);


        verticalLayout_19->addLayout(gridLayout_19);

        label_19 = new QLabel(Equalizer31BandDialog);
        label_19->setObjectName(QStringLiteral("label_19"));
        label_19->setMinimumSize(QSize(31, 0));
        label_19->setMaximumSize(QSize(31, 16777215));
        label_19->setFont(font1);
        label_19->setAlignment(Qt::AlignCenter);

        verticalLayout_19->addWidget(label_19);


        horizontalLayout_2->addLayout(verticalLayout_19);

        verticalLayout_20 = new QVBoxLayout();
        verticalLayout_20->setObjectName(QStringLiteral("verticalLayout_20"));
        lbdb_20 = new QLabel(Equalizer31BandDialog);
        lbdb_20->setObjectName(QStringLiteral("lbdb_20"));
        lbdb_20->setFont(font1);
        lbdb_20->setAlignment(Qt::AlignCenter);

        verticalLayout_20->addWidget(lbdb_20);

        gridLayout_20 = new QGridLayout();
        gridLayout_20->setObjectName(QStringLiteral("gridLayout_20"));
        slider_20 = new Slider(Equalizer31BandDialog);
        slider_20->setObjectName(QStringLiteral("slider_20"));
        slider_20->setMinimumSize(QSize(19, 141));
        slider_20->setMaximumSize(QSize(19, 141));
        slider_20->setFrameShape(QFrame::StyledPanel);
        slider_20->setFrameShadow(QFrame::Raised);

        gridLayout_20->addWidget(slider_20, 0, 0, 1, 1);


        verticalLayout_20->addLayout(gridLayout_20);

        label_20 = new QLabel(Equalizer31BandDialog);
        label_20->setObjectName(QStringLiteral("label_20"));
        label_20->setMinimumSize(QSize(31, 0));
        label_20->setMaximumSize(QSize(31, 16777215));
        label_20->setFont(font1);
        label_20->setAlignment(Qt::AlignCenter);

        verticalLayout_20->addWidget(label_20);


        horizontalLayout_2->addLayout(verticalLayout_20);

        verticalLayout_21 = new QVBoxLayout();
        verticalLayout_21->setObjectName(QStringLiteral("verticalLayout_21"));
        lbdb_21 = new QLabel(Equalizer31BandDialog);
        lbdb_21->setObjectName(QStringLiteral("lbdb_21"));
        lbdb_21->setFont(font1);
        lbdb_21->setAlignment(Qt::AlignCenter);

        verticalLayout_21->addWidget(lbdb_21);

        gridLayout_21 = new QGridLayout();
        gridLayout_21->setObjectName(QStringLiteral("gridLayout_21"));
        slider_21 = new Slider(Equalizer31BandDialog);
        slider_21->setObjectName(QStringLiteral("slider_21"));
        slider_21->setMinimumSize(QSize(19, 141));
        slider_21->setMaximumSize(QSize(19, 141));
        slider_21->setFrameShape(QFrame::StyledPanel);
        slider_21->setFrameShadow(QFrame::Raised);

        gridLayout_21->addWidget(slider_21, 0, 0, 1, 1);


        verticalLayout_21->addLayout(gridLayout_21);

        label_21 = new QLabel(Equalizer31BandDialog);
        label_21->setObjectName(QStringLiteral("label_21"));
        label_21->setMinimumSize(QSize(31, 0));
        label_21->setMaximumSize(QSize(31, 16777215));
        label_21->setFont(font1);
        label_21->setAlignment(Qt::AlignCenter);

        verticalLayout_21->addWidget(label_21);


        horizontalLayout_2->addLayout(verticalLayout_21);

        verticalLayout_22 = new QVBoxLayout();
        verticalLayout_22->setObjectName(QStringLiteral("verticalLayout_22"));
        lbdb_22 = new QLabel(Equalizer31BandDialog);
        lbdb_22->setObjectName(QStringLiteral("lbdb_22"));
        lbdb_22->setFont(font1);
        lbdb_22->setAlignment(Qt::AlignCenter);

        verticalLayout_22->addWidget(lbdb_22);

        gridLayout_22 = new QGridLayout();
        gridLayout_22->setObjectName(QStringLiteral("gridLayout_22"));
        slider_22 = new Slider(Equalizer31BandDialog);
        slider_22->setObjectName(QStringLiteral("slider_22"));
        slider_22->setMinimumSize(QSize(19, 141));
        slider_22->setMaximumSize(QSize(19, 141));
        slider_22->setFrameShape(QFrame::StyledPanel);
        slider_22->setFrameShadow(QFrame::Raised);

        gridLayout_22->addWidget(slider_22, 0, 0, 1, 1);


        verticalLayout_22->addLayout(gridLayout_22);

        label_22 = new QLabel(Equalizer31BandDialog);
        label_22->setObjectName(QStringLiteral("label_22"));
        label_22->setMinimumSize(QSize(31, 0));
        label_22->setMaximumSize(QSize(31, 16777215));
        label_22->setFont(font1);
        label_22->setAlignment(Qt::AlignCenter);

        verticalLayout_22->addWidget(label_22);


        horizontalLayout_2->addLayout(verticalLayout_22);

        verticalLayout_23 = new QVBoxLayout();
        verticalLayout_23->setObjectName(QStringLiteral("verticalLayout_23"));
        lbdb_23 = new QLabel(Equalizer31BandDialog);
        lbdb_23->setObjectName(QStringLiteral("lbdb_23"));
        lbdb_23->setFont(font1);
        lbdb_23->setAlignment(Qt::AlignCenter);

        verticalLayout_23->addWidget(lbdb_23);

        gridLayout_23 = new QGridLayout();
        gridLayout_23->setObjectName(QStringLiteral("gridLayout_23"));
        slider_23 = new Slider(Equalizer31BandDialog);
        slider_23->setObjectName(QStringLiteral("slider_23"));
        slider_23->setMinimumSize(QSize(19, 141));
        slider_23->setMaximumSize(QSize(19, 141));
        slider_23->setFrameShape(QFrame::StyledPanel);
        slider_23->setFrameShadow(QFrame::Raised);

        gridLayout_23->addWidget(slider_23, 0, 0, 1, 1);


        verticalLayout_23->addLayout(gridLayout_23);

        label_23 = new QLabel(Equalizer31BandDialog);
        label_23->setObjectName(QStringLiteral("label_23"));
        label_23->setMinimumSize(QSize(31, 0));
        label_23->setMaximumSize(QSize(31, 16777215));
        label_23->setFont(font1);
        label_23->setAlignment(Qt::AlignCenter);

        verticalLayout_23->addWidget(label_23);


        horizontalLayout_2->addLayout(verticalLayout_23);

        verticalLayout_24 = new QVBoxLayout();
        verticalLayout_24->setObjectName(QStringLiteral("verticalLayout_24"));
        lbdb_24 = new QLabel(Equalizer31BandDialog);
        lbdb_24->setObjectName(QStringLiteral("lbdb_24"));
        lbdb_24->setFont(font1);
        lbdb_24->setAlignment(Qt::AlignCenter);

        verticalLayout_24->addWidget(lbdb_24);

        gridLayout_24 = new QGridLayout();
        gridLayout_24->setObjectName(QStringLiteral("gridLayout_24"));
        slider_24 = new Slider(Equalizer31BandDialog);
        slider_24->setObjectName(QStringLiteral("slider_24"));
        slider_24->setMinimumSize(QSize(19, 141));
        slider_24->setMaximumSize(QSize(19, 141));
        slider_24->setFrameShape(QFrame::StyledPanel);
        slider_24->setFrameShadow(QFrame::Raised);

        gridLayout_24->addWidget(slider_24, 0, 0, 1, 1);


        verticalLayout_24->addLayout(gridLayout_24);

        label_24 = new QLabel(Equalizer31BandDialog);
        label_24->setObjectName(QStringLiteral("label_24"));
        label_24->setMinimumSize(QSize(31, 0));
        label_24->setMaximumSize(QSize(31, 16777215));
        label_24->setFont(font1);
        label_24->setAlignment(Qt::AlignCenter);

        verticalLayout_24->addWidget(label_24);


        horizontalLayout_2->addLayout(verticalLayout_24);

        verticalLayout_25 = new QVBoxLayout();
        verticalLayout_25->setObjectName(QStringLiteral("verticalLayout_25"));
        lbdb_25 = new QLabel(Equalizer31BandDialog);
        lbdb_25->setObjectName(QStringLiteral("lbdb_25"));
        lbdb_25->setFont(font1);
        lbdb_25->setAlignment(Qt::AlignCenter);

        verticalLayout_25->addWidget(lbdb_25);

        gridLayout_25 = new QGridLayout();
        gridLayout_25->setObjectName(QStringLiteral("gridLayout_25"));
        slider_25 = new Slider(Equalizer31BandDialog);
        slider_25->setObjectName(QStringLiteral("slider_25"));
        slider_25->setMinimumSize(QSize(19, 141));
        slider_25->setMaximumSize(QSize(19, 141));
        slider_25->setFrameShape(QFrame::StyledPanel);
        slider_25->setFrameShadow(QFrame::Raised);

        gridLayout_25->addWidget(slider_25, 0, 0, 1, 1);


        verticalLayout_25->addLayout(gridLayout_25);

        label_25 = new QLabel(Equalizer31BandDialog);
        label_25->setObjectName(QStringLiteral("label_25"));
        label_25->setMinimumSize(QSize(31, 0));
        label_25->setMaximumSize(QSize(31, 16777215));
        label_25->setFont(font1);
        label_25->setAlignment(Qt::AlignCenter);

        verticalLayout_25->addWidget(label_25);


        horizontalLayout_2->addLayout(verticalLayout_25);

        verticalLayout_26 = new QVBoxLayout();
        verticalLayout_26->setObjectName(QStringLiteral("verticalLayout_26"));
        lbdb_26 = new QLabel(Equalizer31BandDialog);
        lbdb_26->setObjectName(QStringLiteral("lbdb_26"));
        lbdb_26->setFont(font1);
        lbdb_26->setAlignment(Qt::AlignCenter);

        verticalLayout_26->addWidget(lbdb_26);

        gridLayout_26 = new QGridLayout();
        gridLayout_26->setObjectName(QStringLiteral("gridLayout_26"));
        slider_26 = new Slider(Equalizer31BandDialog);
        slider_26->setObjectName(QStringLiteral("slider_26"));
        slider_26->setMinimumSize(QSize(19, 141));
        slider_26->setMaximumSize(QSize(19, 141));
        slider_26->setFrameShape(QFrame::StyledPanel);
        slider_26->setFrameShadow(QFrame::Raised);

        gridLayout_26->addWidget(slider_26, 0, 0, 1, 1);


        verticalLayout_26->addLayout(gridLayout_26);

        label_26 = new QLabel(Equalizer31BandDialog);
        label_26->setObjectName(QStringLiteral("label_26"));
        label_26->setMinimumSize(QSize(31, 0));
        label_26->setMaximumSize(QSize(31, 16777215));
        label_26->setFont(font1);
        label_26->setAlignment(Qt::AlignCenter);

        verticalLayout_26->addWidget(label_26);


        horizontalLayout_2->addLayout(verticalLayout_26);

        verticalLayout_27 = new QVBoxLayout();
        verticalLayout_27->setObjectName(QStringLiteral("verticalLayout_27"));
        lbdb_27 = new QLabel(Equalizer31BandDialog);
        lbdb_27->setObjectName(QStringLiteral("lbdb_27"));
        lbdb_27->setFont(font1);
        lbdb_27->setAlignment(Qt::AlignCenter);

        verticalLayout_27->addWidget(lbdb_27);

        gridLayout_27 = new QGridLayout();
        gridLayout_27->setObjectName(QStringLiteral("gridLayout_27"));
        slider_27 = new Slider(Equalizer31BandDialog);
        slider_27->setObjectName(QStringLiteral("slider_27"));
        slider_27->setMinimumSize(QSize(19, 141));
        slider_27->setMaximumSize(QSize(19, 141));
        slider_27->setFrameShape(QFrame::StyledPanel);
        slider_27->setFrameShadow(QFrame::Raised);

        gridLayout_27->addWidget(slider_27, 0, 0, 1, 1);


        verticalLayout_27->addLayout(gridLayout_27);

        label_27 = new QLabel(Equalizer31BandDialog);
        label_27->setObjectName(QStringLiteral("label_27"));
        label_27->setMinimumSize(QSize(31, 0));
        label_27->setMaximumSize(QSize(31, 16777215));
        label_27->setFont(font1);
        label_27->setAlignment(Qt::AlignCenter);

        verticalLayout_27->addWidget(label_27);


        horizontalLayout_2->addLayout(verticalLayout_27);

        verticalLayout_28 = new QVBoxLayout();
        verticalLayout_28->setObjectName(QStringLiteral("verticalLayout_28"));
        lbdb_28 = new QLabel(Equalizer31BandDialog);
        lbdb_28->setObjectName(QStringLiteral("lbdb_28"));
        lbdb_28->setFont(font1);
        lbdb_28->setAlignment(Qt::AlignCenter);

        verticalLayout_28->addWidget(lbdb_28);

        gridLayout_28 = new QGridLayout();
        gridLayout_28->setObjectName(QStringLiteral("gridLayout_28"));
        slider_28 = new Slider(Equalizer31BandDialog);
        slider_28->setObjectName(QStringLiteral("slider_28"));
        slider_28->setMinimumSize(QSize(19, 141));
        slider_28->setMaximumSize(QSize(19, 141));
        slider_28->setFrameShape(QFrame::StyledPanel);
        slider_28->setFrameShadow(QFrame::Raised);

        gridLayout_28->addWidget(slider_28, 0, 0, 1, 1);


        verticalLayout_28->addLayout(gridLayout_28);

        label_28 = new QLabel(Equalizer31BandDialog);
        label_28->setObjectName(QStringLiteral("label_28"));
        label_28->setMinimumSize(QSize(31, 0));
        label_28->setMaximumSize(QSize(31, 16777215));
        label_28->setFont(font1);
        label_28->setAlignment(Qt::AlignCenter);

        verticalLayout_28->addWidget(label_28);


        horizontalLayout_2->addLayout(verticalLayout_28);

        verticalLayout_29 = new QVBoxLayout();
        verticalLayout_29->setObjectName(QStringLiteral("verticalLayout_29"));
        lbdb_29 = new QLabel(Equalizer31BandDialog);
        lbdb_29->setObjectName(QStringLiteral("lbdb_29"));
        lbdb_29->setFont(font1);
        lbdb_29->setAlignment(Qt::AlignCenter);

        verticalLayout_29->addWidget(lbdb_29);

        gridLayout_29 = new QGridLayout();
        gridLayout_29->setObjectName(QStringLiteral("gridLayout_29"));
        slider_29 = new Slider(Equalizer31BandDialog);
        slider_29->setObjectName(QStringLiteral("slider_29"));
        slider_29->setMinimumSize(QSize(19, 141));
        slider_29->setMaximumSize(QSize(19, 141));
        slider_29->setFrameShape(QFrame::StyledPanel);
        slider_29->setFrameShadow(QFrame::Raised);

        gridLayout_29->addWidget(slider_29, 0, 0, 1, 1);


        verticalLayout_29->addLayout(gridLayout_29);

        label_29 = new QLabel(Equalizer31BandDialog);
        label_29->setObjectName(QStringLiteral("label_29"));
        label_29->setMinimumSize(QSize(31, 0));
        label_29->setMaximumSize(QSize(31, 16777215));
        label_29->setFont(font1);
        label_29->setAlignment(Qt::AlignCenter);

        verticalLayout_29->addWidget(label_29);


        horizontalLayout_2->addLayout(verticalLayout_29);

        verticalLayout_30 = new QVBoxLayout();
        verticalLayout_30->setObjectName(QStringLiteral("verticalLayout_30"));
        lbdb_30 = new QLabel(Equalizer31BandDialog);
        lbdb_30->setObjectName(QStringLiteral("lbdb_30"));
        lbdb_30->setFont(font1);
        lbdb_30->setAlignment(Qt::AlignCenter);

        verticalLayout_30->addWidget(lbdb_30);

        gridLayout_30 = new QGridLayout();
        gridLayout_30->setObjectName(QStringLiteral("gridLayout_30"));
        slider_30 = new Slider(Equalizer31BandDialog);
        slider_30->setObjectName(QStringLiteral("slider_30"));
        slider_30->setMinimumSize(QSize(19, 141));
        slider_30->setMaximumSize(QSize(19, 141));
        slider_30->setFrameShape(QFrame::StyledPanel);
        slider_30->setFrameShadow(QFrame::Raised);

        gridLayout_30->addWidget(slider_30, 0, 0, 1, 1);


        verticalLayout_30->addLayout(gridLayout_30);

        label_30 = new QLabel(Equalizer31BandDialog);
        label_30->setObjectName(QStringLiteral("label_30"));
        label_30->setMinimumSize(QSize(31, 0));
        label_30->setMaximumSize(QSize(31, 16777215));
        label_30->setFont(font1);
        label_30->setAlignment(Qt::AlignCenter);

        verticalLayout_30->addWidget(label_30);


        horizontalLayout_2->addLayout(verticalLayout_30);


        verticalLayout_37->addLayout(horizontalLayout_2);


        retranslateUi(Equalizer31BandDialog);

        QMetaObject::connectSlotsByName(Equalizer31BandDialog);
    } // setupUi

    void retranslateUi(QDialog *Equalizer31BandDialog)
    {
        Equalizer31BandDialog->setWindowTitle(QApplication::translate("Equalizer31BandDialog", "Equalizer", 0));
#ifndef QT_NO_TOOLTIP
        btnSwitch->setToolTip(QApplication::translate("Equalizer31BandDialog", "\340\271\200\340\270\233\340\270\264\340\270\224 / \340\270\233\340\270\264\340\270\224  \340\270\255\340\270\265\340\270\204\340\270\247\340\270\255\340\271\204\340\270\245\340\271\200\340\270\213\340\270\255\340\270\243\340\271\214", 0));
#endif // QT_NO_TOOLTIP
        btnReset->setText(QApplication::translate("Equalizer31BandDialog", "\340\270\243\340\270\265\340\271\200\340\270\213\340\271\207\340\270\225", 0));
        lbdb_7->setText(QApplication::translate("Equalizer31BandDialog", "0.0", 0));
        label_7->setText(QApplication::translate("Equalizer31BandDialog", "80", 0));
        lbdb_8->setText(QApplication::translate("Equalizer31BandDialog", "0.0", 0));
        label_8->setText(QApplication::translate("Equalizer31BandDialog", "100", 0));
        lbdb_9->setText(QApplication::translate("Equalizer31BandDialog", "0.0", 0));
        label_9->setText(QApplication::translate("Equalizer31BandDialog", "125", 0));
        lbdb_10->setText(QApplication::translate("Equalizer31BandDialog", "0.0", 0));
        label_10->setText(QApplication::translate("Equalizer31BandDialog", "160", 0));
        lbdb_11->setText(QApplication::translate("Equalizer31BandDialog", "0.0", 0));
        label_11->setText(QApplication::translate("Equalizer31BandDialog", "200", 0));
        lbdb_12->setText(QApplication::translate("Equalizer31BandDialog", "0.0", 0));
        label_12->setText(QApplication::translate("Equalizer31BandDialog", "250", 0));
        lbdb_13->setText(QApplication::translate("Equalizer31BandDialog", "0.0", 0));
        label_13->setText(QApplication::translate("Equalizer31BandDialog", "315", 0));
        lbdb_14->setText(QApplication::translate("Equalizer31BandDialog", "0.0", 0));
        label_14->setText(QApplication::translate("Equalizer31BandDialog", "400", 0));
        lbdb_15->setText(QApplication::translate("Equalizer31BandDialog", "0.0", 0));
        label_15->setText(QApplication::translate("Equalizer31BandDialog", "500", 0));
        lbdb_16->setText(QApplication::translate("Equalizer31BandDialog", "0.0", 0));
        label_16->setText(QApplication::translate("Equalizer31BandDialog", "630", 0));
        lbdb_17->setText(QApplication::translate("Equalizer31BandDialog", "0.0", 0));
        label_17->setText(QApplication::translate("Equalizer31BandDialog", "800", 0));
        lbdb_18->setText(QApplication::translate("Equalizer31BandDialog", "0.0", 0));
        label_18->setText(QApplication::translate("Equalizer31BandDialog", "1k", 0));
        lbdb_19->setText(QApplication::translate("Equalizer31BandDialog", "0.0", 0));
        label_19->setText(QApplication::translate("Equalizer31BandDialog", "1.25k", 0));
        lbdb_20->setText(QApplication::translate("Equalizer31BandDialog", "0.0", 0));
        label_20->setText(QApplication::translate("Equalizer31BandDialog", "1.6k", 0));
        lbdb_21->setText(QApplication::translate("Equalizer31BandDialog", "0.0", 0));
        label_21->setText(QApplication::translate("Equalizer31BandDialog", "2k", 0));
        lbdb_22->setText(QApplication::translate("Equalizer31BandDialog", "0.0", 0));
        label_22->setText(QApplication::translate("Equalizer31BandDialog", "2.5k", 0));
        lbdb_23->setText(QApplication::translate("Equalizer31BandDialog", "0.0", 0));
        label_23->setText(QApplication::translate("Equalizer31BandDialog", "3.15k", 0));
        lbdb_24->setText(QApplication::translate("Equalizer31BandDialog", "0.0", 0));
        label_24->setText(QApplication::translate("Equalizer31BandDialog", "4k", 0));
        lbdb_25->setText(QApplication::translate("Equalizer31BandDialog", "0.0", 0));
        label_25->setText(QApplication::translate("Equalizer31BandDialog", "5k", 0));
        lbdb_26->setText(QApplication::translate("Equalizer31BandDialog", "0.0", 0));
        label_26->setText(QApplication::translate("Equalizer31BandDialog", "6.3k", 0));
        lbdb_27->setText(QApplication::translate("Equalizer31BandDialog", "0.0", 0));
        label_27->setText(QApplication::translate("Equalizer31BandDialog", "8k", 0));
        lbdb_28->setText(QApplication::translate("Equalizer31BandDialog", "0.0", 0));
        label_28->setText(QApplication::translate("Equalizer31BandDialog", "10k", 0));
        lbdb_29->setText(QApplication::translate("Equalizer31BandDialog", "0.0", 0));
        label_29->setText(QApplication::translate("Equalizer31BandDialog", "12.5k", 0));
        lbdb_30->setText(QApplication::translate("Equalizer31BandDialog", "0.0", 0));
        label_30->setText(QApplication::translate("Equalizer31BandDialog", "16k", 0));
    } // retranslateUi

};

namespace Ui {
    class Equalizer31BandDialog: public Ui_Equalizer31BandDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EQUALIZER31BANDDIALOG_H
