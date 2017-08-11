/********************************************************************************
** Form generated from reading UI file 'SynthMixerDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SYNTHMIXERDIALOG_H
#define UI_SYNTHMIXERDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "ChannelMixer.h"
#include "InstCh.h"

QT_BEGIN_NAMESPACE

class Ui_SynthMixerDialog
{
public:
    QVBoxLayout *verticalLayout_43;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *btnEq;
    QPushButton *btnReverb;
    QPushButton *btnChorus;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *btnReset;
    QPushButton *btnSettingVu;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QHBoxLayout *horizontalLayout;
    BackgroundChMx *frame;
    QVBoxLayout *verticalLayout;
    InstCh *ch;
    BackgroundChMx *frame_2;
    QVBoxLayout *verticalLayout_2;
    InstCh *ch_2;
    BackgroundChMx *frame_3;
    QVBoxLayout *verticalLayout_3;
    InstCh *ch_3;
    BackgroundChMx *frame_4;
    QVBoxLayout *verticalLayout_4;
    InstCh *ch_4;
    BackgroundChMx *frame_5;
    QVBoxLayout *verticalLayout_5;
    InstCh *ch_5;
    BackgroundChMx *frame_6;
    QVBoxLayout *verticalLayout_6;
    InstCh *ch_6;
    BackgroundChMx *frame_7;
    QVBoxLayout *verticalLayout_7;
    InstCh *ch_7;
    BackgroundChMx *frame_8;
    QVBoxLayout *verticalLayout_8;
    InstCh *ch_8;
    BackgroundChMx *frame_9;
    QVBoxLayout *verticalLayout_9;
    InstCh *ch_9;
    BackgroundChMx *frame_10;
    QVBoxLayout *verticalLayout_10;
    InstCh *ch_10;
    BackgroundChMx *frame_11;
    QVBoxLayout *verticalLayout_11;
    InstCh *ch_11;
    BackgroundChMx *frame_12;
    QVBoxLayout *verticalLayout_12;
    InstCh *ch_12;
    BackgroundChMx *frame_13;
    QVBoxLayout *verticalLayout_13;
    InstCh *ch_13;
    BackgroundChMx *frame_14;
    QVBoxLayout *verticalLayout_14;
    InstCh *ch_14;
    BackgroundChMx *frame_15;
    QVBoxLayout *verticalLayout_15;
    InstCh *ch_15;
    BackgroundChMx *frame_16;
    QVBoxLayout *verticalLayout_16;
    InstCh *ch_16;
    BackgroundChMx *frame_17;
    QVBoxLayout *verticalLayout_17;
    InstCh *ch_17;
    BackgroundChMx *frame_18;
    QVBoxLayout *verticalLayout_18;
    InstCh *ch_18;
    BackgroundChMx *frame_19;
    QVBoxLayout *verticalLayout_19;
    InstCh *ch_19;
    BackgroundChMx *frame_20;
    QVBoxLayout *verticalLayout_20;
    InstCh *ch_20;
    BackgroundChMx *frame_21;
    QVBoxLayout *verticalLayout_21;
    InstCh *ch_21;
    BackgroundChMx *frame_22;
    QVBoxLayout *verticalLayout_22;
    InstCh *ch_22;
    BackgroundChMx *frame_23;
    QVBoxLayout *verticalLayout_23;
    InstCh *ch_23;
    BackgroundChMx *frame_24;
    QVBoxLayout *verticalLayout_24;
    InstCh *ch_24;
    BackgroundChMx *frame_25;
    QVBoxLayout *verticalLayout_25;
    InstCh *ch_25;
    BackgroundChMx *frame_26;
    QVBoxLayout *verticalLayout_26;
    InstCh *ch_26;
    BackgroundChMx *frame_27;
    QVBoxLayout *verticalLayout_27;
    InstCh *ch_27;
    BackgroundChMx *frame_28;
    QVBoxLayout *verticalLayout_28;
    InstCh *ch_28;
    BackgroundChMx *frame_29;
    QVBoxLayout *verticalLayout_29;
    InstCh *ch_29;
    BackgroundChMx *frame_30;
    QVBoxLayout *verticalLayout_30;
    InstCh *ch_30;
    BackgroundChMx *frame_31;
    QVBoxLayout *verticalLayout_31;
    InstCh *ch_31;
    BackgroundChMx *frame_32;
    QVBoxLayout *verticalLayout_32;
    InstCh *ch_32;
    BackgroundChMx *frame_33;
    QVBoxLayout *verticalLayout_33;
    InstCh *ch_33;
    BackgroundChMx *frame_34;
    QVBoxLayout *verticalLayout_34;
    InstCh *ch_34;
    BackgroundChMx *frame_35;
    QVBoxLayout *verticalLayout_35;
    InstCh *ch_35;
    BackgroundChMx *frame_36;
    QVBoxLayout *verticalLayout_36;
    InstCh *ch_36;
    BackgroundChMx *frame_37;
    QVBoxLayout *verticalLayout_37;
    InstCh *ch_37;
    BackgroundChMx *frame_38;
    QVBoxLayout *verticalLayout_38;
    InstCh *ch_38;
    BackgroundChMx *frame_39;
    QVBoxLayout *verticalLayout_39;
    InstCh *ch_39;
    BackgroundChMx *frame_40;
    QVBoxLayout *verticalLayout_40;
    InstCh *ch_40;
    BackgroundChMx *frame_41;
    QVBoxLayout *verticalLayout_41;
    InstCh *ch_41;
    BackgroundChMx *frame_42;
    QVBoxLayout *verticalLayout_42;
    InstCh *ch_42;
    QSpacerItem *horizontalSpacer;

    void setupUi(QDialog *SynthMixerDialog)
    {
        if (SynthMixerDialog->objectName().isEmpty())
            SynthMixerDialog->setObjectName(QStringLiteral("SynthMixerDialog"));
        SynthMixerDialog->resize(970, 308);
        SynthMixerDialog->setMinimumSize(QSize(970, 0));
        QFont font;
        font.setPointSize(10);
        SynthMixerDialog->setFont(font);
        verticalLayout_43 = new QVBoxLayout(SynthMixerDialog);
        verticalLayout_43->setObjectName(QStringLiteral("verticalLayout_43"));
        verticalLayout_43->setContentsMargins(6, 6, 6, 6);
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        btnEq = new QPushButton(SynthMixerDialog);
        btnEq->setObjectName(QStringLiteral("btnEq"));
        btnEq->setFocusPolicy(Qt::NoFocus);
        QIcon icon;
        icon.addFile(QStringLiteral(":/Icons/circle_red.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnEq->setIcon(icon);
        btnEq->setIconSize(QSize(10, 10));

        horizontalLayout_2->addWidget(btnEq);

        btnReverb = new QPushButton(SynthMixerDialog);
        btnReverb->setObjectName(QStringLiteral("btnReverb"));
        btnReverb->setFocusPolicy(Qt::NoFocus);
        btnReverb->setIcon(icon);
        btnReverb->setIconSize(QSize(10, 10));

        horizontalLayout_2->addWidget(btnReverb);

        btnChorus = new QPushButton(SynthMixerDialog);
        btnChorus->setObjectName(QStringLiteral("btnChorus"));
        btnChorus->setFocusPolicy(Qt::NoFocus);
        btnChorus->setIcon(icon);
        btnChorus->setIconSize(QSize(10, 10));

        horizontalLayout_2->addWidget(btnChorus);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        btnReset = new QPushButton(SynthMixerDialog);
        btnReset->setObjectName(QStringLiteral("btnReset"));
        btnReset->setFocusPolicy(Qt::NoFocus);

        horizontalLayout_2->addWidget(btnReset);

        btnSettingVu = new QPushButton(SynthMixerDialog);
        btnSettingVu->setObjectName(QStringLiteral("btnSettingVu"));
        btnSettingVu->setFocusPolicy(Qt::NoFocus);
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/Icons/settings.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnSettingVu->setIcon(icon1);
        btnSettingVu->setIconSize(QSize(12, 12));

        horizontalLayout_2->addWidget(btnSettingVu);


        verticalLayout_43->addLayout(horizontalLayout_2);

        scrollArea = new QScrollArea(SynthMixerDialog);
        scrollArea->setObjectName(QStringLiteral("scrollArea"));
        scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        scrollArea->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QStringLiteral("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 2436, 245));
        horizontalLayout = new QHBoxLayout(scrollAreaWidgetContents);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        frame = new BackgroundChMx(scrollAreaWidgetContents);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        verticalLayout = new QVBoxLayout(frame);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(1, 1, 1, 1);
        ch = new InstCh(frame);
        ch->setObjectName(QStringLiteral("ch"));
        ch->setMinimumSize(QSize(54, 216));
        ch->setMaximumSize(QSize(54, 216));
        ch->setAutoFillBackground(true);

        verticalLayout->addWidget(ch);


        horizontalLayout->addWidget(frame);

        frame_2 = new BackgroundChMx(scrollAreaWidgetContents);
        frame_2->setObjectName(QStringLiteral("frame_2"));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        verticalLayout_2 = new QVBoxLayout(frame_2);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(1, 1, 1, 1);
        ch_2 = new InstCh(frame_2);
        ch_2->setObjectName(QStringLiteral("ch_2"));
        ch_2->setMinimumSize(QSize(54, 216));
        ch_2->setMaximumSize(QSize(54, 216));
        ch_2->setAutoFillBackground(true);

        verticalLayout_2->addWidget(ch_2);


        horizontalLayout->addWidget(frame_2);

        frame_3 = new BackgroundChMx(scrollAreaWidgetContents);
        frame_3->setObjectName(QStringLiteral("frame_3"));
        frame_3->setFrameShape(QFrame::StyledPanel);
        frame_3->setFrameShadow(QFrame::Raised);
        verticalLayout_3 = new QVBoxLayout(frame_3);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(1, 1, 1, 1);
        ch_3 = new InstCh(frame_3);
        ch_3->setObjectName(QStringLiteral("ch_3"));
        ch_3->setMinimumSize(QSize(54, 216));
        ch_3->setMaximumSize(QSize(54, 216));
        ch_3->setAutoFillBackground(true);

        verticalLayout_3->addWidget(ch_3);


        horizontalLayout->addWidget(frame_3);

        frame_4 = new BackgroundChMx(scrollAreaWidgetContents);
        frame_4->setObjectName(QStringLiteral("frame_4"));
        frame_4->setFrameShape(QFrame::StyledPanel);
        frame_4->setFrameShadow(QFrame::Raised);
        verticalLayout_4 = new QVBoxLayout(frame_4);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        verticalLayout_4->setContentsMargins(1, 1, 1, 1);
        ch_4 = new InstCh(frame_4);
        ch_4->setObjectName(QStringLiteral("ch_4"));
        ch_4->setMinimumSize(QSize(54, 216));
        ch_4->setMaximumSize(QSize(54, 216));
        ch_4->setAutoFillBackground(true);

        verticalLayout_4->addWidget(ch_4);


        horizontalLayout->addWidget(frame_4);

        frame_5 = new BackgroundChMx(scrollAreaWidgetContents);
        frame_5->setObjectName(QStringLiteral("frame_5"));
        frame_5->setFrameShape(QFrame::StyledPanel);
        frame_5->setFrameShadow(QFrame::Raised);
        verticalLayout_5 = new QVBoxLayout(frame_5);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        verticalLayout_5->setContentsMargins(1, 1, 1, 1);
        ch_5 = new InstCh(frame_5);
        ch_5->setObjectName(QStringLiteral("ch_5"));
        ch_5->setMinimumSize(QSize(54, 216));
        ch_5->setMaximumSize(QSize(54, 216));
        ch_5->setAutoFillBackground(true);

        verticalLayout_5->addWidget(ch_5);


        horizontalLayout->addWidget(frame_5);

        frame_6 = new BackgroundChMx(scrollAreaWidgetContents);
        frame_6->setObjectName(QStringLiteral("frame_6"));
        frame_6->setFrameShape(QFrame::StyledPanel);
        frame_6->setFrameShadow(QFrame::Raised);
        verticalLayout_6 = new QVBoxLayout(frame_6);
        verticalLayout_6->setObjectName(QStringLiteral("verticalLayout_6"));
        verticalLayout_6->setContentsMargins(1, 1, 1, 1);
        ch_6 = new InstCh(frame_6);
        ch_6->setObjectName(QStringLiteral("ch_6"));
        ch_6->setMinimumSize(QSize(54, 216));
        ch_6->setMaximumSize(QSize(54, 216));
        ch_6->setAutoFillBackground(true);

        verticalLayout_6->addWidget(ch_6);


        horizontalLayout->addWidget(frame_6);

        frame_7 = new BackgroundChMx(scrollAreaWidgetContents);
        frame_7->setObjectName(QStringLiteral("frame_7"));
        frame_7->setFrameShape(QFrame::StyledPanel);
        frame_7->setFrameShadow(QFrame::Raised);
        verticalLayout_7 = new QVBoxLayout(frame_7);
        verticalLayout_7->setObjectName(QStringLiteral("verticalLayout_7"));
        verticalLayout_7->setContentsMargins(1, 1, 1, 1);
        ch_7 = new InstCh(frame_7);
        ch_7->setObjectName(QStringLiteral("ch_7"));
        ch_7->setMinimumSize(QSize(54, 216));
        ch_7->setMaximumSize(QSize(54, 216));
        ch_7->setAutoFillBackground(true);

        verticalLayout_7->addWidget(ch_7);


        horizontalLayout->addWidget(frame_7);

        frame_8 = new BackgroundChMx(scrollAreaWidgetContents);
        frame_8->setObjectName(QStringLiteral("frame_8"));
        frame_8->setFrameShape(QFrame::StyledPanel);
        frame_8->setFrameShadow(QFrame::Raised);
        verticalLayout_8 = new QVBoxLayout(frame_8);
        verticalLayout_8->setObjectName(QStringLiteral("verticalLayout_8"));
        verticalLayout_8->setContentsMargins(1, 1, 1, 1);
        ch_8 = new InstCh(frame_8);
        ch_8->setObjectName(QStringLiteral("ch_8"));
        ch_8->setMinimumSize(QSize(54, 216));
        ch_8->setMaximumSize(QSize(54, 216));
        ch_8->setAutoFillBackground(true);

        verticalLayout_8->addWidget(ch_8);


        horizontalLayout->addWidget(frame_8);

        frame_9 = new BackgroundChMx(scrollAreaWidgetContents);
        frame_9->setObjectName(QStringLiteral("frame_9"));
        frame_9->setFrameShape(QFrame::StyledPanel);
        frame_9->setFrameShadow(QFrame::Raised);
        verticalLayout_9 = new QVBoxLayout(frame_9);
        verticalLayout_9->setObjectName(QStringLiteral("verticalLayout_9"));
        verticalLayout_9->setContentsMargins(1, 1, 1, 1);
        ch_9 = new InstCh(frame_9);
        ch_9->setObjectName(QStringLiteral("ch_9"));
        ch_9->setMinimumSize(QSize(54, 216));
        ch_9->setMaximumSize(QSize(54, 216));
        ch_9->setAutoFillBackground(true);

        verticalLayout_9->addWidget(ch_9);


        horizontalLayout->addWidget(frame_9);

        frame_10 = new BackgroundChMx(scrollAreaWidgetContents);
        frame_10->setObjectName(QStringLiteral("frame_10"));
        frame_10->setFrameShape(QFrame::StyledPanel);
        frame_10->setFrameShadow(QFrame::Raised);
        verticalLayout_10 = new QVBoxLayout(frame_10);
        verticalLayout_10->setObjectName(QStringLiteral("verticalLayout_10"));
        verticalLayout_10->setContentsMargins(1, 1, 1, 1);
        ch_10 = new InstCh(frame_10);
        ch_10->setObjectName(QStringLiteral("ch_10"));
        ch_10->setMinimumSize(QSize(54, 216));
        ch_10->setMaximumSize(QSize(54, 216));
        ch_10->setAutoFillBackground(true);

        verticalLayout_10->addWidget(ch_10);


        horizontalLayout->addWidget(frame_10);

        frame_11 = new BackgroundChMx(scrollAreaWidgetContents);
        frame_11->setObjectName(QStringLiteral("frame_11"));
        frame_11->setFrameShape(QFrame::StyledPanel);
        frame_11->setFrameShadow(QFrame::Raised);
        verticalLayout_11 = new QVBoxLayout(frame_11);
        verticalLayout_11->setObjectName(QStringLiteral("verticalLayout_11"));
        verticalLayout_11->setContentsMargins(1, 1, 1, 1);
        ch_11 = new InstCh(frame_11);
        ch_11->setObjectName(QStringLiteral("ch_11"));
        ch_11->setMinimumSize(QSize(54, 216));
        ch_11->setMaximumSize(QSize(54, 216));
        ch_11->setAutoFillBackground(true);

        verticalLayout_11->addWidget(ch_11);


        horizontalLayout->addWidget(frame_11);

        frame_12 = new BackgroundChMx(scrollAreaWidgetContents);
        frame_12->setObjectName(QStringLiteral("frame_12"));
        frame_12->setFrameShape(QFrame::StyledPanel);
        frame_12->setFrameShadow(QFrame::Raised);
        verticalLayout_12 = new QVBoxLayout(frame_12);
        verticalLayout_12->setObjectName(QStringLiteral("verticalLayout_12"));
        verticalLayout_12->setContentsMargins(1, 1, 1, 1);
        ch_12 = new InstCh(frame_12);
        ch_12->setObjectName(QStringLiteral("ch_12"));
        ch_12->setMinimumSize(QSize(54, 216));
        ch_12->setMaximumSize(QSize(54, 216));
        ch_12->setAutoFillBackground(true);

        verticalLayout_12->addWidget(ch_12);


        horizontalLayout->addWidget(frame_12);

        frame_13 = new BackgroundChMx(scrollAreaWidgetContents);
        frame_13->setObjectName(QStringLiteral("frame_13"));
        frame_13->setFrameShape(QFrame::StyledPanel);
        frame_13->setFrameShadow(QFrame::Raised);
        verticalLayout_13 = new QVBoxLayout(frame_13);
        verticalLayout_13->setObjectName(QStringLiteral("verticalLayout_13"));
        verticalLayout_13->setContentsMargins(1, 1, 1, 1);
        ch_13 = new InstCh(frame_13);
        ch_13->setObjectName(QStringLiteral("ch_13"));
        ch_13->setMinimumSize(QSize(54, 216));
        ch_13->setMaximumSize(QSize(54, 216));
        ch_13->setAutoFillBackground(true);

        verticalLayout_13->addWidget(ch_13);


        horizontalLayout->addWidget(frame_13);

        frame_14 = new BackgroundChMx(scrollAreaWidgetContents);
        frame_14->setObjectName(QStringLiteral("frame_14"));
        frame_14->setFrameShape(QFrame::StyledPanel);
        frame_14->setFrameShadow(QFrame::Raised);
        verticalLayout_14 = new QVBoxLayout(frame_14);
        verticalLayout_14->setObjectName(QStringLiteral("verticalLayout_14"));
        verticalLayout_14->setContentsMargins(1, 1, 1, 1);
        ch_14 = new InstCh(frame_14);
        ch_14->setObjectName(QStringLiteral("ch_14"));
        ch_14->setMinimumSize(QSize(54, 216));
        ch_14->setMaximumSize(QSize(54, 216));
        ch_14->setAutoFillBackground(true);

        verticalLayout_14->addWidget(ch_14);


        horizontalLayout->addWidget(frame_14);

        frame_15 = new BackgroundChMx(scrollAreaWidgetContents);
        frame_15->setObjectName(QStringLiteral("frame_15"));
        frame_15->setFrameShape(QFrame::StyledPanel);
        frame_15->setFrameShadow(QFrame::Raised);
        verticalLayout_15 = new QVBoxLayout(frame_15);
        verticalLayout_15->setObjectName(QStringLiteral("verticalLayout_15"));
        verticalLayout_15->setContentsMargins(1, 1, 1, 1);
        ch_15 = new InstCh(frame_15);
        ch_15->setObjectName(QStringLiteral("ch_15"));
        ch_15->setMinimumSize(QSize(54, 216));
        ch_15->setMaximumSize(QSize(54, 216));
        ch_15->setAutoFillBackground(true);

        verticalLayout_15->addWidget(ch_15);


        horizontalLayout->addWidget(frame_15);

        frame_16 = new BackgroundChMx(scrollAreaWidgetContents);
        frame_16->setObjectName(QStringLiteral("frame_16"));
        frame_16->setFrameShape(QFrame::StyledPanel);
        frame_16->setFrameShadow(QFrame::Raised);
        verticalLayout_16 = new QVBoxLayout(frame_16);
        verticalLayout_16->setObjectName(QStringLiteral("verticalLayout_16"));
        verticalLayout_16->setContentsMargins(1, 1, 1, 1);
        ch_16 = new InstCh(frame_16);
        ch_16->setObjectName(QStringLiteral("ch_16"));
        ch_16->setMinimumSize(QSize(54, 216));
        ch_16->setMaximumSize(QSize(54, 216));
        ch_16->setAutoFillBackground(true);

        verticalLayout_16->addWidget(ch_16);


        horizontalLayout->addWidget(frame_16);

        frame_17 = new BackgroundChMx(scrollAreaWidgetContents);
        frame_17->setObjectName(QStringLiteral("frame_17"));
        frame_17->setFrameShape(QFrame::StyledPanel);
        frame_17->setFrameShadow(QFrame::Raised);
        verticalLayout_17 = new QVBoxLayout(frame_17);
        verticalLayout_17->setObjectName(QStringLiteral("verticalLayout_17"));
        verticalLayout_17->setContentsMargins(1, 1, 1, 1);
        ch_17 = new InstCh(frame_17);
        ch_17->setObjectName(QStringLiteral("ch_17"));
        ch_17->setMinimumSize(QSize(54, 216));
        ch_17->setMaximumSize(QSize(54, 216));
        ch_17->setAutoFillBackground(true);

        verticalLayout_17->addWidget(ch_17);


        horizontalLayout->addWidget(frame_17);

        frame_18 = new BackgroundChMx(scrollAreaWidgetContents);
        frame_18->setObjectName(QStringLiteral("frame_18"));
        frame_18->setFrameShape(QFrame::StyledPanel);
        frame_18->setFrameShadow(QFrame::Raised);
        verticalLayout_18 = new QVBoxLayout(frame_18);
        verticalLayout_18->setObjectName(QStringLiteral("verticalLayout_18"));
        verticalLayout_18->setContentsMargins(1, 1, 1, 1);
        ch_18 = new InstCh(frame_18);
        ch_18->setObjectName(QStringLiteral("ch_18"));
        ch_18->setMinimumSize(QSize(54, 216));
        ch_18->setMaximumSize(QSize(54, 216));
        ch_18->setAutoFillBackground(true);

        verticalLayout_18->addWidget(ch_18);


        horizontalLayout->addWidget(frame_18);

        frame_19 = new BackgroundChMx(scrollAreaWidgetContents);
        frame_19->setObjectName(QStringLiteral("frame_19"));
        frame_19->setFrameShape(QFrame::StyledPanel);
        frame_19->setFrameShadow(QFrame::Raised);
        verticalLayout_19 = new QVBoxLayout(frame_19);
        verticalLayout_19->setObjectName(QStringLiteral("verticalLayout_19"));
        verticalLayout_19->setContentsMargins(1, 1, 1, 1);
        ch_19 = new InstCh(frame_19);
        ch_19->setObjectName(QStringLiteral("ch_19"));
        ch_19->setMinimumSize(QSize(54, 216));
        ch_19->setMaximumSize(QSize(54, 216));
        ch_19->setAutoFillBackground(true);

        verticalLayout_19->addWidget(ch_19);


        horizontalLayout->addWidget(frame_19);

        frame_20 = new BackgroundChMx(scrollAreaWidgetContents);
        frame_20->setObjectName(QStringLiteral("frame_20"));
        frame_20->setFrameShape(QFrame::StyledPanel);
        frame_20->setFrameShadow(QFrame::Raised);
        verticalLayout_20 = new QVBoxLayout(frame_20);
        verticalLayout_20->setObjectName(QStringLiteral("verticalLayout_20"));
        verticalLayout_20->setContentsMargins(1, 1, 1, 1);
        ch_20 = new InstCh(frame_20);
        ch_20->setObjectName(QStringLiteral("ch_20"));
        ch_20->setMinimumSize(QSize(54, 216));
        ch_20->setMaximumSize(QSize(54, 216));
        ch_20->setAutoFillBackground(true);

        verticalLayout_20->addWidget(ch_20);


        horizontalLayout->addWidget(frame_20);

        frame_21 = new BackgroundChMx(scrollAreaWidgetContents);
        frame_21->setObjectName(QStringLiteral("frame_21"));
        frame_21->setFrameShape(QFrame::StyledPanel);
        frame_21->setFrameShadow(QFrame::Raised);
        verticalLayout_21 = new QVBoxLayout(frame_21);
        verticalLayout_21->setObjectName(QStringLiteral("verticalLayout_21"));
        verticalLayout_21->setContentsMargins(1, 1, 1, 1);
        ch_21 = new InstCh(frame_21);
        ch_21->setObjectName(QStringLiteral("ch_21"));
        ch_21->setMinimumSize(QSize(54, 216));
        ch_21->setMaximumSize(QSize(54, 216));
        ch_21->setAutoFillBackground(true);

        verticalLayout_21->addWidget(ch_21);


        horizontalLayout->addWidget(frame_21);

        frame_22 = new BackgroundChMx(scrollAreaWidgetContents);
        frame_22->setObjectName(QStringLiteral("frame_22"));
        frame_22->setFrameShape(QFrame::StyledPanel);
        frame_22->setFrameShadow(QFrame::Raised);
        verticalLayout_22 = new QVBoxLayout(frame_22);
        verticalLayout_22->setObjectName(QStringLiteral("verticalLayout_22"));
        verticalLayout_22->setContentsMargins(1, 1, 1, 1);
        ch_22 = new InstCh(frame_22);
        ch_22->setObjectName(QStringLiteral("ch_22"));
        ch_22->setMinimumSize(QSize(54, 216));
        ch_22->setMaximumSize(QSize(54, 216));
        ch_22->setAutoFillBackground(true);

        verticalLayout_22->addWidget(ch_22);


        horizontalLayout->addWidget(frame_22);

        frame_23 = new BackgroundChMx(scrollAreaWidgetContents);
        frame_23->setObjectName(QStringLiteral("frame_23"));
        frame_23->setFrameShape(QFrame::StyledPanel);
        frame_23->setFrameShadow(QFrame::Raised);
        verticalLayout_23 = new QVBoxLayout(frame_23);
        verticalLayout_23->setObjectName(QStringLiteral("verticalLayout_23"));
        verticalLayout_23->setContentsMargins(1, 1, 1, 1);
        ch_23 = new InstCh(frame_23);
        ch_23->setObjectName(QStringLiteral("ch_23"));
        ch_23->setMinimumSize(QSize(54, 216));
        ch_23->setMaximumSize(QSize(54, 216));
        ch_23->setAutoFillBackground(true);

        verticalLayout_23->addWidget(ch_23);


        horizontalLayout->addWidget(frame_23);

        frame_24 = new BackgroundChMx(scrollAreaWidgetContents);
        frame_24->setObjectName(QStringLiteral("frame_24"));
        frame_24->setFrameShape(QFrame::StyledPanel);
        frame_24->setFrameShadow(QFrame::Raised);
        verticalLayout_24 = new QVBoxLayout(frame_24);
        verticalLayout_24->setObjectName(QStringLiteral("verticalLayout_24"));
        verticalLayout_24->setContentsMargins(1, 1, 1, 1);
        ch_24 = new InstCh(frame_24);
        ch_24->setObjectName(QStringLiteral("ch_24"));
        ch_24->setMinimumSize(QSize(54, 216));
        ch_24->setMaximumSize(QSize(54, 216));
        ch_24->setAutoFillBackground(true);

        verticalLayout_24->addWidget(ch_24);


        horizontalLayout->addWidget(frame_24);

        frame_25 = new BackgroundChMx(scrollAreaWidgetContents);
        frame_25->setObjectName(QStringLiteral("frame_25"));
        frame_25->setFrameShape(QFrame::StyledPanel);
        frame_25->setFrameShadow(QFrame::Raised);
        verticalLayout_25 = new QVBoxLayout(frame_25);
        verticalLayout_25->setObjectName(QStringLiteral("verticalLayout_25"));
        verticalLayout_25->setContentsMargins(1, 1, 1, 1);
        ch_25 = new InstCh(frame_25);
        ch_25->setObjectName(QStringLiteral("ch_25"));
        ch_25->setMinimumSize(QSize(54, 216));
        ch_25->setMaximumSize(QSize(54, 216));
        ch_25->setAutoFillBackground(true);

        verticalLayout_25->addWidget(ch_25);


        horizontalLayout->addWidget(frame_25);

        frame_26 = new BackgroundChMx(scrollAreaWidgetContents);
        frame_26->setObjectName(QStringLiteral("frame_26"));
        frame_26->setFrameShape(QFrame::StyledPanel);
        frame_26->setFrameShadow(QFrame::Raised);
        verticalLayout_26 = new QVBoxLayout(frame_26);
        verticalLayout_26->setObjectName(QStringLiteral("verticalLayout_26"));
        verticalLayout_26->setContentsMargins(1, 1, 1, 1);
        ch_26 = new InstCh(frame_26);
        ch_26->setObjectName(QStringLiteral("ch_26"));
        ch_26->setMinimumSize(QSize(54, 216));
        ch_26->setMaximumSize(QSize(54, 216));
        ch_26->setAutoFillBackground(true);

        verticalLayout_26->addWidget(ch_26);


        horizontalLayout->addWidget(frame_26);

        frame_27 = new BackgroundChMx(scrollAreaWidgetContents);
        frame_27->setObjectName(QStringLiteral("frame_27"));
        frame_27->setFrameShape(QFrame::StyledPanel);
        frame_27->setFrameShadow(QFrame::Raised);
        verticalLayout_27 = new QVBoxLayout(frame_27);
        verticalLayout_27->setObjectName(QStringLiteral("verticalLayout_27"));
        verticalLayout_27->setContentsMargins(1, 1, 1, 1);
        ch_27 = new InstCh(frame_27);
        ch_27->setObjectName(QStringLiteral("ch_27"));
        ch_27->setMinimumSize(QSize(54, 216));
        ch_27->setMaximumSize(QSize(54, 216));
        ch_27->setAutoFillBackground(true);

        verticalLayout_27->addWidget(ch_27);


        horizontalLayout->addWidget(frame_27);

        frame_28 = new BackgroundChMx(scrollAreaWidgetContents);
        frame_28->setObjectName(QStringLiteral("frame_28"));
        frame_28->setFrameShape(QFrame::StyledPanel);
        frame_28->setFrameShadow(QFrame::Raised);
        verticalLayout_28 = new QVBoxLayout(frame_28);
        verticalLayout_28->setObjectName(QStringLiteral("verticalLayout_28"));
        verticalLayout_28->setContentsMargins(1, 1, 1, 1);
        ch_28 = new InstCh(frame_28);
        ch_28->setObjectName(QStringLiteral("ch_28"));
        ch_28->setMinimumSize(QSize(54, 216));
        ch_28->setMaximumSize(QSize(54, 216));
        ch_28->setAutoFillBackground(true);

        verticalLayout_28->addWidget(ch_28);


        horizontalLayout->addWidget(frame_28);

        frame_29 = new BackgroundChMx(scrollAreaWidgetContents);
        frame_29->setObjectName(QStringLiteral("frame_29"));
        frame_29->setFrameShape(QFrame::StyledPanel);
        frame_29->setFrameShadow(QFrame::Raised);
        verticalLayout_29 = new QVBoxLayout(frame_29);
        verticalLayout_29->setObjectName(QStringLiteral("verticalLayout_29"));
        verticalLayout_29->setContentsMargins(1, 1, 1, 1);
        ch_29 = new InstCh(frame_29);
        ch_29->setObjectName(QStringLiteral("ch_29"));
        ch_29->setMinimumSize(QSize(54, 216));
        ch_29->setMaximumSize(QSize(54, 216));
        ch_29->setAutoFillBackground(true);

        verticalLayout_29->addWidget(ch_29);


        horizontalLayout->addWidget(frame_29);

        frame_30 = new BackgroundChMx(scrollAreaWidgetContents);
        frame_30->setObjectName(QStringLiteral("frame_30"));
        frame_30->setFrameShape(QFrame::StyledPanel);
        frame_30->setFrameShadow(QFrame::Raised);
        verticalLayout_30 = new QVBoxLayout(frame_30);
        verticalLayout_30->setObjectName(QStringLiteral("verticalLayout_30"));
        verticalLayout_30->setContentsMargins(1, 1, 1, 1);
        ch_30 = new InstCh(frame_30);
        ch_30->setObjectName(QStringLiteral("ch_30"));
        ch_30->setMinimumSize(QSize(54, 216));
        ch_30->setMaximumSize(QSize(54, 216));
        ch_30->setAutoFillBackground(true);

        verticalLayout_30->addWidget(ch_30);


        horizontalLayout->addWidget(frame_30);

        frame_31 = new BackgroundChMx(scrollAreaWidgetContents);
        frame_31->setObjectName(QStringLiteral("frame_31"));
        frame_31->setFrameShape(QFrame::StyledPanel);
        frame_31->setFrameShadow(QFrame::Raised);
        verticalLayout_31 = new QVBoxLayout(frame_31);
        verticalLayout_31->setObjectName(QStringLiteral("verticalLayout_31"));
        verticalLayout_31->setContentsMargins(1, 1, 1, 1);
        ch_31 = new InstCh(frame_31);
        ch_31->setObjectName(QStringLiteral("ch_31"));
        ch_31->setMinimumSize(QSize(54, 216));
        ch_31->setMaximumSize(QSize(54, 216));
        ch_31->setAutoFillBackground(true);

        verticalLayout_31->addWidget(ch_31);


        horizontalLayout->addWidget(frame_31);

        frame_32 = new BackgroundChMx(scrollAreaWidgetContents);
        frame_32->setObjectName(QStringLiteral("frame_32"));
        frame_32->setFrameShape(QFrame::StyledPanel);
        frame_32->setFrameShadow(QFrame::Raised);
        verticalLayout_32 = new QVBoxLayout(frame_32);
        verticalLayout_32->setObjectName(QStringLiteral("verticalLayout_32"));
        verticalLayout_32->setContentsMargins(1, 1, 1, 1);
        ch_32 = new InstCh(frame_32);
        ch_32->setObjectName(QStringLiteral("ch_32"));
        ch_32->setMinimumSize(QSize(54, 216));
        ch_32->setMaximumSize(QSize(54, 216));
        ch_32->setAutoFillBackground(true);

        verticalLayout_32->addWidget(ch_32);


        horizontalLayout->addWidget(frame_32);

        frame_33 = new BackgroundChMx(scrollAreaWidgetContents);
        frame_33->setObjectName(QStringLiteral("frame_33"));
        frame_33->setFrameShape(QFrame::StyledPanel);
        frame_33->setFrameShadow(QFrame::Raised);
        verticalLayout_33 = new QVBoxLayout(frame_33);
        verticalLayout_33->setObjectName(QStringLiteral("verticalLayout_33"));
        verticalLayout_33->setContentsMargins(1, 1, 1, 1);
        ch_33 = new InstCh(frame_33);
        ch_33->setObjectName(QStringLiteral("ch_33"));
        ch_33->setMinimumSize(QSize(54, 216));
        ch_33->setMaximumSize(QSize(54, 216));
        ch_33->setAutoFillBackground(true);

        verticalLayout_33->addWidget(ch_33);


        horizontalLayout->addWidget(frame_33);

        frame_34 = new BackgroundChMx(scrollAreaWidgetContents);
        frame_34->setObjectName(QStringLiteral("frame_34"));
        frame_34->setFrameShape(QFrame::StyledPanel);
        frame_34->setFrameShadow(QFrame::Raised);
        verticalLayout_34 = new QVBoxLayout(frame_34);
        verticalLayout_34->setObjectName(QStringLiteral("verticalLayout_34"));
        verticalLayout_34->setContentsMargins(1, 1, 1, 1);
        ch_34 = new InstCh(frame_34);
        ch_34->setObjectName(QStringLiteral("ch_34"));
        ch_34->setMinimumSize(QSize(54, 216));
        ch_34->setMaximumSize(QSize(54, 216));
        ch_34->setAutoFillBackground(true);

        verticalLayout_34->addWidget(ch_34);


        horizontalLayout->addWidget(frame_34);

        frame_35 = new BackgroundChMx(scrollAreaWidgetContents);
        frame_35->setObjectName(QStringLiteral("frame_35"));
        frame_35->setFrameShape(QFrame::StyledPanel);
        frame_35->setFrameShadow(QFrame::Raised);
        verticalLayout_35 = new QVBoxLayout(frame_35);
        verticalLayout_35->setObjectName(QStringLiteral("verticalLayout_35"));
        verticalLayout_35->setContentsMargins(1, 1, 1, 1);
        ch_35 = new InstCh(frame_35);
        ch_35->setObjectName(QStringLiteral("ch_35"));
        ch_35->setMinimumSize(QSize(54, 216));
        ch_35->setMaximumSize(QSize(54, 216));
        ch_35->setAutoFillBackground(true);

        verticalLayout_35->addWidget(ch_35);


        horizontalLayout->addWidget(frame_35);

        frame_36 = new BackgroundChMx(scrollAreaWidgetContents);
        frame_36->setObjectName(QStringLiteral("frame_36"));
        frame_36->setFrameShape(QFrame::StyledPanel);
        frame_36->setFrameShadow(QFrame::Raised);
        verticalLayout_36 = new QVBoxLayout(frame_36);
        verticalLayout_36->setObjectName(QStringLiteral("verticalLayout_36"));
        verticalLayout_36->setContentsMargins(1, 1, 1, 1);
        ch_36 = new InstCh(frame_36);
        ch_36->setObjectName(QStringLiteral("ch_36"));
        ch_36->setMinimumSize(QSize(54, 216));
        ch_36->setMaximumSize(QSize(54, 216));
        ch_36->setAutoFillBackground(true);

        verticalLayout_36->addWidget(ch_36);


        horizontalLayout->addWidget(frame_36);

        frame_37 = new BackgroundChMx(scrollAreaWidgetContents);
        frame_37->setObjectName(QStringLiteral("frame_37"));
        frame_37->setFrameShape(QFrame::StyledPanel);
        frame_37->setFrameShadow(QFrame::Raised);
        verticalLayout_37 = new QVBoxLayout(frame_37);
        verticalLayout_37->setObjectName(QStringLiteral("verticalLayout_37"));
        verticalLayout_37->setContentsMargins(1, 1, 1, 1);
        ch_37 = new InstCh(frame_37);
        ch_37->setObjectName(QStringLiteral("ch_37"));
        ch_37->setMinimumSize(QSize(54, 216));
        ch_37->setMaximumSize(QSize(54, 216));
        ch_37->setAutoFillBackground(true);

        verticalLayout_37->addWidget(ch_37);


        horizontalLayout->addWidget(frame_37);

        frame_38 = new BackgroundChMx(scrollAreaWidgetContents);
        frame_38->setObjectName(QStringLiteral("frame_38"));
        frame_38->setFrameShape(QFrame::StyledPanel);
        frame_38->setFrameShadow(QFrame::Raised);
        verticalLayout_38 = new QVBoxLayout(frame_38);
        verticalLayout_38->setObjectName(QStringLiteral("verticalLayout_38"));
        verticalLayout_38->setContentsMargins(1, 1, 1, 1);
        ch_38 = new InstCh(frame_38);
        ch_38->setObjectName(QStringLiteral("ch_38"));
        ch_38->setMinimumSize(QSize(54, 216));
        ch_38->setMaximumSize(QSize(54, 216));
        ch_38->setAutoFillBackground(true);

        verticalLayout_38->addWidget(ch_38);


        horizontalLayout->addWidget(frame_38);

        frame_39 = new BackgroundChMx(scrollAreaWidgetContents);
        frame_39->setObjectName(QStringLiteral("frame_39"));
        frame_39->setFrameShape(QFrame::StyledPanel);
        frame_39->setFrameShadow(QFrame::Raised);
        verticalLayout_39 = new QVBoxLayout(frame_39);
        verticalLayout_39->setObjectName(QStringLiteral("verticalLayout_39"));
        verticalLayout_39->setContentsMargins(1, 1, 1, 1);
        ch_39 = new InstCh(frame_39);
        ch_39->setObjectName(QStringLiteral("ch_39"));
        ch_39->setMinimumSize(QSize(54, 216));
        ch_39->setMaximumSize(QSize(54, 216));
        ch_39->setAutoFillBackground(true);

        verticalLayout_39->addWidget(ch_39);


        horizontalLayout->addWidget(frame_39);

        frame_40 = new BackgroundChMx(scrollAreaWidgetContents);
        frame_40->setObjectName(QStringLiteral("frame_40"));
        frame_40->setFrameShape(QFrame::StyledPanel);
        frame_40->setFrameShadow(QFrame::Raised);
        verticalLayout_40 = new QVBoxLayout(frame_40);
        verticalLayout_40->setObjectName(QStringLiteral("verticalLayout_40"));
        verticalLayout_40->setContentsMargins(1, 1, 1, 1);
        ch_40 = new InstCh(frame_40);
        ch_40->setObjectName(QStringLiteral("ch_40"));
        ch_40->setMinimumSize(QSize(54, 216));
        ch_40->setMaximumSize(QSize(54, 216));
        ch_40->setAutoFillBackground(true);

        verticalLayout_40->addWidget(ch_40);


        horizontalLayout->addWidget(frame_40);

        frame_41 = new BackgroundChMx(scrollAreaWidgetContents);
        frame_41->setObjectName(QStringLiteral("frame_41"));
        frame_41->setFrameShape(QFrame::StyledPanel);
        frame_41->setFrameShadow(QFrame::Raised);
        verticalLayout_41 = new QVBoxLayout(frame_41);
        verticalLayout_41->setObjectName(QStringLiteral("verticalLayout_41"));
        verticalLayout_41->setContentsMargins(1, 1, 1, 1);
        ch_41 = new InstCh(frame_41);
        ch_41->setObjectName(QStringLiteral("ch_41"));
        ch_41->setMinimumSize(QSize(54, 216));
        ch_41->setMaximumSize(QSize(54, 216));
        ch_41->setAutoFillBackground(true);

        verticalLayout_41->addWidget(ch_41);


        horizontalLayout->addWidget(frame_41);

        frame_42 = new BackgroundChMx(scrollAreaWidgetContents);
        frame_42->setObjectName(QStringLiteral("frame_42"));
        frame_42->setFrameShape(QFrame::StyledPanel);
        frame_42->setFrameShadow(QFrame::Raised);
        verticalLayout_42 = new QVBoxLayout(frame_42);
        verticalLayout_42->setObjectName(QStringLiteral("verticalLayout_42"));
        verticalLayout_42->setContentsMargins(1, 1, 1, 1);
        ch_42 = new InstCh(frame_42);
        ch_42->setObjectName(QStringLiteral("ch_42"));
        ch_42->setMinimumSize(QSize(54, 216));
        ch_42->setMaximumSize(QSize(54, 216));
        ch_42->setAutoFillBackground(true);

        verticalLayout_42->addWidget(ch_42);


        horizontalLayout->addWidget(frame_42);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        scrollArea->setWidget(scrollAreaWidgetContents);

        verticalLayout_43->addWidget(scrollArea);


        retranslateUi(SynthMixerDialog);

        QMetaObject::connectSlotsByName(SynthMixerDialog);
    } // setupUi

    void retranslateUi(QDialog *SynthMixerDialog)
    {
        SynthMixerDialog->setWindowTitle(QApplication::translate("SynthMixerDialog", "Handy Synth Mixer", 0));
        btnEq->setText(QApplication::translate("SynthMixerDialog", "\340\270\255\340\270\265\340\270\204\340\270\247\340\270\255\340\271\204\340\270\245\340\271\200\340\270\213\340\270\255\340\270\243\340\271\214", 0));
        btnReverb->setText(QApplication::translate("SynthMixerDialog", "\340\271\200\340\270\255\340\270\237\340\271\200\340\270\237\340\271\207\340\270\201\340\270\225\340\271\214\340\271\200\340\270\252\340\270\265\340\270\242\340\270\207\340\270\201\340\271\211\340\270\255\340\270\207", 0));
        btnChorus->setText(QApplication::translate("SynthMixerDialog", "\340\271\200\340\270\255\340\270\237\340\271\200\340\270\237\340\271\207\340\270\201\340\270\225\340\271\214\340\271\200\340\270\252\340\270\265\340\270\242\340\270\207\340\270\233\340\270\243\340\270\260\340\270\252\340\270\262\340\270\231", 0));
        btnReset->setText(QApplication::translate("SynthMixerDialog", "\340\270\243\340\270\265\340\271\200\340\270\213\340\271\207\340\270\225", 0));
#ifndef QT_NO_TOOLTIP
        btnSettingVu->setToolTip(QApplication::translate("SynthMixerDialog", "\340\270\225\340\270\261\340\271\211\340\270\207\340\270\204\340\271\210\340\270\262 LED Meter", 0));
#endif // QT_NO_TOOLTIP
        btnSettingVu->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class SynthMixerDialog: public Ui_SynthMixerDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SYNTHMIXERDIALOG_H
