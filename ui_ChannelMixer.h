/********************************************************************************
** Form generated from reading UI file 'ChannelMixer.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHANNELMIXER_H
#define UI_CHANNELMIXER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDial>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "ChMx.h"
#include "ChannelMixer.h"

QT_BEGIN_NAMESPACE

class Ui_ChannelMixer
{
public:
    QVBoxLayout *verticalLayout;
    BackgroundChMx *frame;
    QHBoxLayout *horizontalLayout_8;
    ChMx *ch1;
    ChMx *ch2;
    ChMx *ch3;
    ChMx *ch4;
    ChMx *ch5;
    ChMx *ch6;
    ChMx *ch7;
    ChMx *ch8;
    ChMx *ch9;
    ChMx *ch10;
    ChMx *ch11;
    ChMx *ch12;
    ChMx *ch13;
    ChMx *ch14;
    ChMx *ch15;
    ChMx *ch16;
    QHBoxLayout *horizontalLayout_7;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    QComboBox *cbCh;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_2;
    QComboBox *cbInts;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_3;
    QDial *dialPan;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_4;
    QDial *dialReverb;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_5;
    QDial *dialChorus;
    QSpacerItem *horizontalSpacer;
    QCheckBox *chbMuteVoice;
    QPushButton *btnSettingVu;
    QPushButton *btnClose;

    void setupUi(QWidget *ChannelMixer)
    {
        if (ChannelMixer->objectName().isEmpty())
            ChannelMixer->setObjectName(QStringLiteral("ChannelMixer"));
        ChannelMixer->resize(748, 171);
        ChannelMixer->setAutoFillBackground(false);
        verticalLayout = new QVBoxLayout(ChannelMixer);
        verticalLayout->setSpacing(2);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 2);
        frame = new BackgroundChMx(ChannelMixer);
        frame->setObjectName(QStringLiteral("frame"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(frame->sizePolicy().hasHeightForWidth());
        frame->setSizePolicy(sizePolicy);
        frame->setMinimumSize(QSize(0, 120));
        frame->setMaximumSize(QSize(16777215, 120));
        frame->setAutoFillBackground(true);
        frame->setStyleSheet(QStringLiteral(""));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        frame->setLineWidth(1);
        horizontalLayout_8 = new QHBoxLayout(frame);
        horizontalLayout_8->setObjectName(QStringLiteral("horizontalLayout_8"));
        horizontalLayout_8->setContentsMargins(0, 0, 0, 0);
        ch1 = new ChMx(frame);
        ch1->setObjectName(QStringLiteral("ch1"));
        ch1->setMinimumSize(QSize(41, 0));
        ch1->setMaximumSize(QSize(45, 16777215));
        ch1->setAutoFillBackground(true);

        horizontalLayout_8->addWidget(ch1);

        ch2 = new ChMx(frame);
        ch2->setObjectName(QStringLiteral("ch2"));
        ch2->setMinimumSize(QSize(41, 0));
        ch2->setMaximumSize(QSize(45, 16777215));
        ch2->setAutoFillBackground(true);

        horizontalLayout_8->addWidget(ch2);

        ch3 = new ChMx(frame);
        ch3->setObjectName(QStringLiteral("ch3"));
        ch3->setMinimumSize(QSize(41, 0));
        ch3->setMaximumSize(QSize(45, 16777215));
        ch3->setAutoFillBackground(true);

        horizontalLayout_8->addWidget(ch3);

        ch4 = new ChMx(frame);
        ch4->setObjectName(QStringLiteral("ch4"));
        ch4->setMinimumSize(QSize(41, 0));
        ch4->setMaximumSize(QSize(45, 16777215));
        ch4->setAutoFillBackground(true);

        horizontalLayout_8->addWidget(ch4);

        ch5 = new ChMx(frame);
        ch5->setObjectName(QStringLiteral("ch5"));
        ch5->setMinimumSize(QSize(41, 0));
        ch5->setMaximumSize(QSize(45, 16777215));
        ch5->setAutoFillBackground(true);

        horizontalLayout_8->addWidget(ch5);

        ch6 = new ChMx(frame);
        ch6->setObjectName(QStringLiteral("ch6"));
        ch6->setMinimumSize(QSize(41, 0));
        ch6->setMaximumSize(QSize(45, 16777215));
        ch6->setAutoFillBackground(true);

        horizontalLayout_8->addWidget(ch6);

        ch7 = new ChMx(frame);
        ch7->setObjectName(QStringLiteral("ch7"));
        ch7->setMinimumSize(QSize(41, 0));
        ch7->setMaximumSize(QSize(45, 16777215));
        ch7->setAutoFillBackground(true);

        horizontalLayout_8->addWidget(ch7);

        ch8 = new ChMx(frame);
        ch8->setObjectName(QStringLiteral("ch8"));
        ch8->setMinimumSize(QSize(41, 0));
        ch8->setMaximumSize(QSize(45, 16777215));
        ch8->setAutoFillBackground(true);

        horizontalLayout_8->addWidget(ch8);

        ch9 = new ChMx(frame);
        ch9->setObjectName(QStringLiteral("ch9"));
        ch9->setMinimumSize(QSize(41, 0));
        ch9->setMaximumSize(QSize(45, 16777215));
        ch9->setAutoFillBackground(true);

        horizontalLayout_8->addWidget(ch9);

        ch10 = new ChMx(frame);
        ch10->setObjectName(QStringLiteral("ch10"));
        ch10->setMinimumSize(QSize(41, 0));
        ch10->setMaximumSize(QSize(45, 16777215));
        ch10->setAutoFillBackground(true);

        horizontalLayout_8->addWidget(ch10);

        ch11 = new ChMx(frame);
        ch11->setObjectName(QStringLiteral("ch11"));
        ch11->setMinimumSize(QSize(41, 0));
        ch11->setMaximumSize(QSize(45, 16777215));
        ch11->setAutoFillBackground(true);

        horizontalLayout_8->addWidget(ch11);

        ch12 = new ChMx(frame);
        ch12->setObjectName(QStringLiteral("ch12"));
        ch12->setMinimumSize(QSize(41, 0));
        ch12->setMaximumSize(QSize(45, 16777215));
        ch12->setAutoFillBackground(true);

        horizontalLayout_8->addWidget(ch12);

        ch13 = new ChMx(frame);
        ch13->setObjectName(QStringLiteral("ch13"));
        ch13->setMinimumSize(QSize(41, 0));
        ch13->setMaximumSize(QSize(45, 16777215));
        ch13->setAutoFillBackground(true);

        horizontalLayout_8->addWidget(ch13);

        ch14 = new ChMx(frame);
        ch14->setObjectName(QStringLiteral("ch14"));
        ch14->setMinimumSize(QSize(41, 0));
        ch14->setMaximumSize(QSize(45, 16777215));
        ch14->setAutoFillBackground(true);

        horizontalLayout_8->addWidget(ch14);

        ch15 = new ChMx(frame);
        ch15->setObjectName(QStringLiteral("ch15"));
        ch15->setMinimumSize(QSize(41, 0));
        ch15->setMaximumSize(QSize(45, 16777215));
        ch15->setAutoFillBackground(true);

        horizontalLayout_8->addWidget(ch15);

        ch16 = new ChMx(frame);
        ch16->setObjectName(QStringLiteral("ch16"));
        ch16->setMinimumSize(QSize(41, 0));
        ch16->setMaximumSize(QSize(45, 16777215));
        ch16->setAutoFillBackground(true);

        horizontalLayout_8->addWidget(ch16);


        verticalLayout->addWidget(frame);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        horizontalLayout_7->setSizeConstraint(QLayout::SetDefaultConstraint);
        horizontalLayout_7->setContentsMargins(4, 0, 4, 0);
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(4);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        label = new QLabel(ChannelMixer);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout_2->addWidget(label);

        cbCh = new QComboBox(ChannelMixer);
        cbCh->setObjectName(QStringLiteral("cbCh"));
        cbCh->setMinimumSize(QSize(60, 0));
        cbCh->setFocusPolicy(Qt::NoFocus);
        cbCh->setSizeAdjustPolicy(QComboBox::AdjustToContents);

        horizontalLayout_2->addWidget(cbCh);


        horizontalLayout_7->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(4);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        label_2 = new QLabel(ChannelMixer);
        label_2->setObjectName(QStringLiteral("label_2"));

        horizontalLayout_3->addWidget(label_2);

        cbInts = new QComboBox(ChannelMixer);
        cbInts->setObjectName(QStringLiteral("cbInts"));
        cbInts->setMinimumSize(QSize(230, 0));
        cbInts->setFocusPolicy(Qt::NoFocus);
        cbInts->setMaxVisibleItems(14);
        cbInts->setSizeAdjustPolicy(QComboBox::AdjustToContents);

        horizontalLayout_3->addWidget(cbInts);


        horizontalLayout_7->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(0);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        label_3 = new QLabel(ChannelMixer);
        label_3->setObjectName(QStringLiteral("label_3"));

        horizontalLayout_4->addWidget(label_3);

        dialPan = new QDial(ChannelMixer);
        dialPan->setObjectName(QStringLiteral("dialPan"));
        dialPan->setMinimumSize(QSize(31, 31));
        dialPan->setMaximumSize(QSize(31, 31));
        dialPan->setFocusPolicy(Qt::NoFocus);
        dialPan->setMaximum(127);
        dialPan->setPageStep(1);
        dialPan->setValue(64);

        horizontalLayout_4->addWidget(dialPan);


        horizontalLayout_7->addLayout(horizontalLayout_4);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(0);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        label_4 = new QLabel(ChannelMixer);
        label_4->setObjectName(QStringLiteral("label_4"));

        horizontalLayout_5->addWidget(label_4);

        dialReverb = new QDial(ChannelMixer);
        dialReverb->setObjectName(QStringLiteral("dialReverb"));
        dialReverb->setMinimumSize(QSize(31, 31));
        dialReverb->setMaximumSize(QSize(31, 31));
        dialReverb->setFocusPolicy(Qt::NoFocus);
        dialReverb->setMaximum(127);
        dialReverb->setPageStep(1);
        dialReverb->setValue(0);

        horizontalLayout_5->addWidget(dialReverb);


        horizontalLayout_7->addLayout(horizontalLayout_5);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(0);
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        label_5 = new QLabel(ChannelMixer);
        label_5->setObjectName(QStringLiteral("label_5"));

        horizontalLayout_6->addWidget(label_5);

        dialChorus = new QDial(ChannelMixer);
        dialChorus->setObjectName(QStringLiteral("dialChorus"));
        dialChorus->setMinimumSize(QSize(31, 31));
        dialChorus->setMaximumSize(QSize(31, 31));
        dialChorus->setFocusPolicy(Qt::NoFocus);
        dialChorus->setMaximum(127);
        dialChorus->setPageStep(1);
        dialChorus->setValue(0);

        horizontalLayout_6->addWidget(dialChorus);


        horizontalLayout_7->addLayout(horizontalLayout_6);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer);

        chbMuteVoice = new QCheckBox(ChannelMixer);
        chbMuteVoice->setObjectName(QStringLiteral("chbMuteVoice"));
        chbMuteVoice->setFocusPolicy(Qt::NoFocus);

        horizontalLayout_7->addWidget(chbMuteVoice);

        btnSettingVu = new QPushButton(ChannelMixer);
        btnSettingVu->setObjectName(QStringLiteral("btnSettingVu"));
        btnSettingVu->setMinimumSize(QSize(30, 26));
        btnSettingVu->setMaximumSize(QSize(30, 26));
        btnSettingVu->setFocusPolicy(Qt::NoFocus);
        QIcon icon;
        icon.addFile(QStringLiteral(":/Icons/settings.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnSettingVu->setIcon(icon);
        btnSettingVu->setIconSize(QSize(12, 12));

        horizontalLayout_7->addWidget(btnSettingVu);

        btnClose = new QPushButton(ChannelMixer);
        btnClose->setObjectName(QStringLiteral("btnClose"));
        btnClose->setMinimumSize(QSize(30, 26));
        btnClose->setMaximumSize(QSize(30, 26));
        btnClose->setFocusPolicy(Qt::NoFocus);
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/Icons/delete.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnClose->setIcon(icon1);
        btnClose->setIconSize(QSize(12, 12));

        horizontalLayout_7->addWidget(btnClose);


        verticalLayout->addLayout(horizontalLayout_7);


        retranslateUi(ChannelMixer);

        QMetaObject::connectSlotsByName(ChannelMixer);
    } // setupUi

    void retranslateUi(QWidget *ChannelMixer)
    {
        ChannelMixer->setWindowTitle(QApplication::translate("ChannelMixer", "Form", Q_NULLPTR));
        label->setText(QApplication::translate("ChannelMixer", "\340\270\212\340\271\210\340\270\255\340\270\207:", Q_NULLPTR));
        cbCh->clear();
        cbCh->insertItems(0, QStringList()
         << QApplication::translate("ChannelMixer", "1", Q_NULLPTR)
         << QApplication::translate("ChannelMixer", "2", Q_NULLPTR)
         << QApplication::translate("ChannelMixer", "3", Q_NULLPTR)
         << QApplication::translate("ChannelMixer", "4", Q_NULLPTR)
         << QApplication::translate("ChannelMixer", "5", Q_NULLPTR)
         << QApplication::translate("ChannelMixer", "6", Q_NULLPTR)
         << QApplication::translate("ChannelMixer", "7", Q_NULLPTR)
         << QApplication::translate("ChannelMixer", "8", Q_NULLPTR)
         << QApplication::translate("ChannelMixer", "9", Q_NULLPTR)
         << QApplication::translate("ChannelMixer", "10", Q_NULLPTR)
         << QApplication::translate("ChannelMixer", "11", Q_NULLPTR)
         << QApplication::translate("ChannelMixer", "12", Q_NULLPTR)
         << QApplication::translate("ChannelMixer", "13", Q_NULLPTR)
         << QApplication::translate("ChannelMixer", "14", Q_NULLPTR)
         << QApplication::translate("ChannelMixer", "15", Q_NULLPTR)
         << QApplication::translate("ChannelMixer", "16", Q_NULLPTR)
        );
        label_2->setText(QApplication::translate("ChannelMixer", "\340\271\200\340\270\204\340\270\243\340\270\267\340\271\210\340\270\255\340\270\207\340\270\224\340\270\231\340\270\225\340\270\243\340\270\265:", Q_NULLPTR));
        label_3->setText(QApplication::translate("ChannelMixer", "P.", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        dialPan->setToolTip(QApplication::translate("ChannelMixer", "\340\270\201\340\270\266\340\271\210\340\270\207\340\270\201\340\270\245\340\270\262\340\270\207", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        label_4->setText(QApplication::translate("ChannelMixer", "R.", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        dialReverb->setToolTip(QApplication::translate("ChannelMixer", "\340\271\200\340\270\252\340\270\265\340\270\242\340\270\207\340\270\201\340\271\211\340\270\255\340\270\207  0", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        label_5->setText(QApplication::translate("ChannelMixer", "C.", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        dialChorus->setToolTip(QApplication::translate("ChannelMixer", "\340\271\200\340\270\252\340\270\265\340\270\242\340\270\207\340\270\233\340\270\243\340\270\260\340\270\252\340\270\262\340\270\231  0", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        chbMuteVoice->setText(QApplication::translate("ChannelMixer", "\340\270\233\340\270\264\340\270\224\340\271\200\340\270\252\340\270\265\340\270\242\340\270\207\340\270\243\340\271\211\340\270\255\340\270\207", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        btnSettingVu->setToolTip(QApplication::translate("ChannelMixer", "\340\270\225\340\270\261\340\271\211\340\270\207\340\270\204\340\271\210\340\270\262 LED Meter", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        btnSettingVu->setText(QString());
#ifndef QT_NO_TOOLTIP
        btnClose->setToolTip(QApplication::translate("ChannelMixer", "\340\270\233\340\270\264\340\270\224", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
    } // retranslateUi

};

namespace Ui {
    class ChannelMixer: public Ui_ChannelMixer {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHANNELMIXER_H
