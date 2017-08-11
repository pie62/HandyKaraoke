/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QLocale>
#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "ChannelMixer.h"
#include "Detail.h"
#include "RhythmWidget.h"
#include "SongDetail.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout_4;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_13;
    ChannelMixer *chMix;
    QSpacerItem *horizontalSpacer_2;
    QVBoxLayout *verticalLayout_5;
    QHBoxLayout *horizontalLayout;
    Detail *detail;
    QLCDNumber *lcdTime;
    RhythmWidget *rhmWidget;
    QWidget *expandChMix;
    QFrame *frameSearch;
    QHBoxLayout *horizontalLayout_6;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_5;
    QFrame *frame_2;
    QHBoxLayout *horizontalLayout_2;
    QLabel *lbSearch;
    QFrame *frame_3;
    QHBoxLayout *horizontalLayout_3;
    QLabel *lbLyrics;
    QFrame *frame_4;
    QHBoxLayout *horizontalLayout_4;
    QLabel *lbId;
    QLabel *lbName;
    QLabel *lbBtw;
    QLabel *lbArtist;
    QLabel *lbTempoKey;
    QLabel *lbType;
    QSpacerItem *horizontalSpacer;
    QFrame *framePlaylist;
    QVBoxLayout *verticalLayout_2;
    QFrame *frame;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label;
    QListWidget *playlist;
    QHBoxLayout *horizontalLayout_14;
    SongDetail *songDetail;
    QSpacerItem *horizontalSpacer_3;
    QSpacerItem *verticalSpacer;
    QFrame *frameControl;
    QHBoxLayout *horizontalLayout_9;
    QHBoxLayout *horizontalLayout_8;
    QPushButton *btnPlay;
    QPushButton *btnPause;
    QPushButton *btnStop;
    QPushButton *btnPrevious;
    QPushButton *btnNext;
    QHBoxLayout *horizontalLayout_11;
    QHBoxLayout *horizontalLayout_10;
    QLabel *lbPosition;
    QLabel *label_4;
    QLabel *lbDuration;
    QSlider *sliderPosition;
    QHBoxLayout *horizontalLayout_12;
    QPushButton *btnVolumeMute;
    QSlider *sliderVolume;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1000, 670);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        MainWindow->setMinimumSize(QSize(1000, 670));
        QFont font;
        font.setPointSize(10);
        MainWindow->setFont(font);
        MainWindow->setMouseTracking(true);
        MainWindow->setContextMenuPolicy(Qt::CustomContextMenu);
        MainWindow->setStyleSheet(QLatin1String("/*#MainWindow {\n"
"background-color: rgb(51, 51, 51);\n"
"}*/"));
        MainWindow->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        centralWidget->setMouseTracking(true);
        verticalLayout_4 = new QVBoxLayout(centralWidget);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        verticalLayout_4->setContentsMargins(0, -1, 0, 0);
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(9, 0, 9, 0);
        horizontalLayout_13 = new QHBoxLayout();
        horizontalLayout_13->setSpacing(6);
        horizontalLayout_13->setObjectName(QStringLiteral("horizontalLayout_13"));
        chMix = new ChannelMixer(centralWidget);
        chMix->setObjectName(QStringLiteral("chMix"));
        chMix->setMinimumSize(QSize(400, 157));
        chMix->setMaximumSize(QSize(16777215, 157));
        chMix->setAutoFillBackground(true);

        horizontalLayout_13->addWidget(chMix);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_13->addItem(horizontalSpacer_2);

        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        detail = new Detail(centralWidget);
        detail->setObjectName(QStringLiteral("detail"));
        detail->setMinimumSize(QSize(150, 52));
        detail->setMaximumSize(QSize(150, 52));

        horizontalLayout->addWidget(detail);

        lcdTime = new QLCDNumber(centralWidget);
        lcdTime->setObjectName(QStringLiteral("lcdTime"));
        lcdTime->setMinimumSize(QSize(120, 52));
        lcdTime->setMaximumSize(QSize(120, 52));
        lcdTime->setStyleSheet(QLatin1String("color: rgb(255, 255, 255);\n"
"background-color: rgb(62, 62, 62, 200);\n"
"border: 1px solid rgb(255, 255, 255);"));
        lcdTime->setDigitCount(8);

        horizontalLayout->addWidget(lcdTime);

        rhmWidget = new RhythmWidget(centralWidget);
        rhmWidget->setObjectName(QStringLiteral("rhmWidget"));
        rhmWidget->setMinimumSize(QSize(150, 52));
        rhmWidget->setMaximumSize(QSize(150, 52));

        horizontalLayout->addWidget(rhmWidget);


        verticalLayout_5->addLayout(horizontalLayout);

        expandChMix = new QWidget(centralWidget);
        expandChMix->setObjectName(QStringLiteral("expandChMix"));
        sizePolicy.setHeightForWidth(expandChMix->sizePolicy().hasHeightForWidth());
        expandChMix->setSizePolicy(sizePolicy);
        expandChMix->setMinimumSize(QSize(0, 96));

        verticalLayout_5->addWidget(expandChMix);


        horizontalLayout_13->addLayout(verticalLayout_5);


        verticalLayout_3->addLayout(horizontalLayout_13);

        frameSearch = new QFrame(centralWidget);
        frameSearch->setObjectName(QStringLiteral("frameSearch"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(frameSearch->sizePolicy().hasHeightForWidth());
        frameSearch->setSizePolicy(sizePolicy1);
        frameSearch->setMinimumSize(QSize(0, 109));
        frameSearch->setMaximumSize(QSize(16777215, 109));
        frameSearch->setStyleSheet(QLatin1String("#frameSearch {\n"
"	border: 1px solid rgb(224, 224, 224);\n"
"	background-color: rgba(66, 66, 66);\n"
"}"));
        frameSearch->setFrameShape(QFrame::StyledPanel);
        frameSearch->setFrameShadow(QFrame::Raised);
        horizontalLayout_6 = new QHBoxLayout(frameSearch);
        horizontalLayout_6->setSpacing(0);
        horizontalLayout_6->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        horizontalLayout_6->setContentsMargins(4, 4, 4, 4);
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(4);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(4);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        frame_2 = new QFrame(frameSearch);
        frame_2->setObjectName(QStringLiteral("frame_2"));
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(frame_2->sizePolicy().hasHeightForWidth());
        frame_2->setSizePolicy(sizePolicy2);
        frame_2->setMinimumSize(QSize(331, 41));
        frame_2->setMaximumSize(QSize(331, 41));
        frame_2->setStyleSheet(QLatin1String("border: none;\n"
"border-radius: 2px;\n"
"background-color: rgb(224, 224, 224);\n"
"color: rgb(66, 66, 66);\n"
"font: bold 14pt;"));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        horizontalLayout_2 = new QHBoxLayout(frame_2);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(9, 0, -1, 0);
        lbSearch = new QLabel(frame_2);
        lbSearch->setObjectName(QStringLiteral("lbSearch"));

        horizontalLayout_2->addWidget(lbSearch);


        horizontalLayout_5->addWidget(frame_2);

        frame_3 = new QFrame(frameSearch);
        frame_3->setObjectName(QStringLiteral("frame_3"));
        QSizePolicy sizePolicy3(QSizePolicy::Ignored, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(frame_3->sizePolicy().hasHeightForWidth());
        frame_3->setSizePolicy(sizePolicy3);
        frame_3->setMinimumSize(QSize(0, 41));
        frame_3->setMaximumSize(QSize(16777215, 41));
        frame_3->setStyleSheet(QLatin1String("border: none;\n"
"border-radius: 2px;\n"
"background-color: rgb(41, 128, 185);\n"
"color: rgb(255, 244, 110);\n"
"font: bold 14pt;"));
        frame_3->setFrameShape(QFrame::StyledPanel);
        frame_3->setFrameShadow(QFrame::Raised);
        horizontalLayout_3 = new QHBoxLayout(frame_3);
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(9, 0, -1, 0);
        lbLyrics = new QLabel(frame_3);
        lbLyrics->setObjectName(QStringLiteral("lbLyrics"));

        horizontalLayout_3->addWidget(lbLyrics);


        horizontalLayout_5->addWidget(frame_3);


        verticalLayout->addLayout(horizontalLayout_5);

        frame_4 = new QFrame(frameSearch);
        frame_4->setObjectName(QStringLiteral("frame_4"));
        sizePolicy3.setHeightForWidth(frame_4->sizePolicy().hasHeightForWidth());
        frame_4->setSizePolicy(sizePolicy3);
        frame_4->setMinimumSize(QSize(0, 51));
        frame_4->setMaximumSize(QSize(16777215, 51));
        frame_4->setStyleSheet(QLatin1String("border: none;\n"
"border-radius: 2px;\n"
"background-color: rgb(0, 150, 136);\n"
"color: rgb(238, 238, 238);\n"
"font: bold 16pt;"));
        frame_4->setFrameShape(QFrame::StyledPanel);
        frame_4->setFrameShadow(QFrame::Raised);
        horizontalLayout_4 = new QHBoxLayout(frame_4);
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(9, 0, -1, 0);
        lbId = new QLabel(frame_4);
        lbId->setObjectName(QStringLiteral("lbId"));

        horizontalLayout_4->addWidget(lbId);

        lbName = new QLabel(frame_4);
        lbName->setObjectName(QStringLiteral("lbName"));

        horizontalLayout_4->addWidget(lbName);

        lbBtw = new QLabel(frame_4);
        lbBtw->setObjectName(QStringLiteral("lbBtw"));

        horizontalLayout_4->addWidget(lbBtw);

        lbArtist = new QLabel(frame_4);
        lbArtist->setObjectName(QStringLiteral("lbArtist"));

        horizontalLayout_4->addWidget(lbArtist);

        lbTempoKey = new QLabel(frame_4);
        lbTempoKey->setObjectName(QStringLiteral("lbTempoKey"));

        horizontalLayout_4->addWidget(lbTempoKey);

        lbType = new QLabel(frame_4);
        lbType->setObjectName(QStringLiteral("lbType"));

        horizontalLayout_4->addWidget(lbType);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer);


        verticalLayout->addWidget(frame_4);


        horizontalLayout_6->addLayout(verticalLayout);


        verticalLayout_3->addWidget(frameSearch);

        framePlaylist = new QFrame(centralWidget);
        framePlaylist->setObjectName(QStringLiteral("framePlaylist"));
        framePlaylist->setMinimumSize(QSize(0, 231));
        framePlaylist->setMaximumSize(QSize(16777215, 231));
        framePlaylist->setStyleSheet(QLatin1String("#framePlaylist {\n"
"	border: 1px solid rgb(224, 224, 224);\n"
"	background-color: rgba(66, 66, 66);\n"
"}"));
        framePlaylist->setFrameShape(QFrame::StyledPanel);
        framePlaylist->setFrameShadow(QFrame::Raised);
        verticalLayout_2 = new QVBoxLayout(framePlaylist);
        verticalLayout_2->setSpacing(4);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(4, 4, 4, 4);
        frame = new QFrame(framePlaylist);
        frame->setObjectName(QStringLiteral("frame"));
        QSizePolicy sizePolicy4(QSizePolicy::Ignored, QSizePolicy::Preferred);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(frame->sizePolicy().hasHeightForWidth());
        frame->setSizePolicy(sizePolicy4);
        frame->setMinimumSize(QSize(0, 51));
        frame->setMaximumSize(QSize(16777215, 51));
        frame->setStyleSheet(QLatin1String("border: none;\n"
"border-radius: 2px;\n"
"background-color: rgb(41, 128, 185);\n"
"color: rgb(255, 244, 110);\n"
"font: bold 16pt;"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        horizontalLayout_7 = new QHBoxLayout(frame);
        horizontalLayout_7->setSpacing(6);
        horizontalLayout_7->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        horizontalLayout_7->setContentsMargins(-1, 0, -1, 0);
        label = new QLabel(frame);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout_7->addWidget(label);


        verticalLayout_2->addWidget(frame);

        playlist = new QListWidget(framePlaylist);
        playlist->setObjectName(QStringLiteral("playlist"));
        QSizePolicy sizePolicy5(QSizePolicy::Ignored, QSizePolicy::Expanding);
        sizePolicy5.setHorizontalStretch(0);
        sizePolicy5.setVerticalStretch(0);
        sizePolicy5.setHeightForWidth(playlist->sizePolicy().hasHeightForWidth());
        playlist->setSizePolicy(sizePolicy5);
        playlist->setFocusPolicy(Qt::NoFocus);
        playlist->setStyleSheet(QLatin1String("border: none;\n"
"border-radius: 2px;\n"
"background-color: rgb(0, 150, 136);\n"
"color: rgb(238, 238, 238);\n"
"font: bold 14pt;"));

        verticalLayout_2->addWidget(playlist);


        verticalLayout_3->addWidget(framePlaylist);

        horizontalLayout_14 = new QHBoxLayout();
        horizontalLayout_14->setSpacing(6);
        horizontalLayout_14->setObjectName(QStringLiteral("horizontalLayout_14"));
        songDetail = new SongDetail(centralWidget);
        songDetail->setObjectName(QStringLiteral("songDetail"));
        songDetail->setMinimumSize(QSize(100, 50));

        horizontalLayout_14->addWidget(songDetail);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_14->addItem(horizontalSpacer_3);


        verticalLayout_3->addLayout(horizontalLayout_14);


        verticalLayout_4->addLayout(verticalLayout_3);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_4->addItem(verticalSpacer);

        frameControl = new QFrame(centralWidget);
        frameControl->setObjectName(QStringLiteral("frameControl"));
        frameControl->setMinimumSize(QSize(0, 41));
        frameControl->setMaximumSize(QSize(16777215, 41));
        frameControl->setStyleSheet(QLatin1String("#frameControl {\n"
"border: none;\n"
"	background-color: rgb(229, 229, 229);\n"
"}\n"
"QToolTip {\n"
"	color: rgb(14, 14, 14);\n"
"}\n"
"QLabel {\n"
"	font: bold 10pt;\n"
"	color: rgb(88, 88, 88);\n"
"}\n"
"\n"
"QSlider::groove:horizontal {\n"
"border: 1px solid #bbb;\n"
"background: rgb(189, 189, 189);\n"
"height: 10px;\n"
"border-radius: 4px;\n"
"}\n"
"\n"
"QSlider::sub-page:horizontal {\n"
"background-color:  rgb(56, 152, 247);\n"
"border: 1px solid #777;\n"
"height: 10px;\n"
"border-radius: 4px;\n"
"}\n"
"\n"
"QSlider::add-page:horizontal {\n"
"background:  rgb(189, 189, 189);\n"
"border: 1px solid #777;\n"
"height: 10px;\n"
"border-radius: 4px;\n"
"}\n"
"\n"
"QSlider::handle:horizontal {\n"
"background: qlineargradient(x1:0, y1:0, x2:1, y2:1,\n"
"    stop:0 #eee, stop:1 #ccc);\n"
"border: 1px solid #777;\n"
"width: 14px;\n"
"margin-top: -2px;\n"
"margin-bottom: -2px;\n"
"border-radius: 4px;\n"
"}\n"
"\n"
"QSlider::handle:horizontal:hover {\n"
"background: qlineargradient(x1:0, y1:0, x2:1, y2:1,\n"
"    st"
                        "op:0 #fff, stop:1 #ddd);\n"
"border: 1px solid #444;\n"
"border-radius: 4px;\n"
"}\n"
"\n"
"QSlider::sub-page:horizontal:disabled {\n"
"/*background: #bbb;*/\n"
"	background-color: rgb(173, 196, 255);\n"
"border-color: #999;\n"
"}\n"
"\n"
"QSlider::add-page:horizontal:disabled {\n"
"/*background: #eee;*/\n"
"	background-color: rgb(210, 210, 210);\n"
"border-color: #999;\n"
"}\n"
"\n"
"QSlider::handle:horizontal:disabled {\n"
"background: #eee;\n"
"border: 1px solid #aaa;\n"
"border-radius: 4px;\n"
"}"));
        frameControl->setFrameShape(QFrame::StyledPanel);
        frameControl->setFrameShadow(QFrame::Raised);
        horizontalLayout_9 = new QHBoxLayout(frameControl);
        horizontalLayout_9->setSpacing(9);
        horizontalLayout_9->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_9->setObjectName(QStringLiteral("horizontalLayout_9"));
        horizontalLayout_9->setContentsMargins(2, 0, -1, 0);
        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setSpacing(1);
        horizontalLayout_8->setObjectName(QStringLiteral("horizontalLayout_8"));
        btnPlay = new QPushButton(frameControl);
        btnPlay->setObjectName(QStringLiteral("btnPlay"));
        btnPlay->setMinimumSize(QSize(43, 37));
        btnPlay->setMaximumSize(QSize(43, 37));
        btnPlay->setFocusPolicy(Qt::NoFocus);
        btnPlay->setStyleSheet(QStringLiteral("border: none;"));
        QIcon icon;
        icon.addFile(QStringLiteral(":/Icons/play-button.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnPlay->setIcon(icon);
        btnPlay->setIconSize(QSize(22, 22));

        horizontalLayout_8->addWidget(btnPlay);

        btnPause = new QPushButton(frameControl);
        btnPause->setObjectName(QStringLiteral("btnPause"));
        btnPause->setMinimumSize(QSize(43, 37));
        btnPause->setMaximumSize(QSize(43, 37));
        btnPause->setFocusPolicy(Qt::NoFocus);
        btnPause->setStyleSheet(QStringLiteral("border: none;"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/Icons/pause-button.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnPause->setIcon(icon1);
        btnPause->setIconSize(QSize(22, 22));

        horizontalLayout_8->addWidget(btnPause);

        btnStop = new QPushButton(frameControl);
        btnStop->setObjectName(QStringLiteral("btnStop"));
        btnStop->setMinimumSize(QSize(43, 37));
        btnStop->setMaximumSize(QSize(43, 37));
        btnStop->setFocusPolicy(Qt::NoFocus);
        btnStop->setStyleSheet(QStringLiteral("border: none;"));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/Icons/video-player-stop-button.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnStop->setIcon(icon2);
        btnStop->setIconSize(QSize(22, 22));

        horizontalLayout_8->addWidget(btnStop);

        btnPrevious = new QPushButton(frameControl);
        btnPrevious->setObjectName(QStringLiteral("btnPrevious"));
        btnPrevious->setMinimumSize(QSize(43, 37));
        btnPrevious->setMaximumSize(QSize(43, 37));
        btnPrevious->setFocusPolicy(Qt::NoFocus);
        btnPrevious->setStyleSheet(QStringLiteral("border: none;"));
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/Icons/previous-track.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnPrevious->setIcon(icon3);
        btnPrevious->setIconSize(QSize(22, 22));

        horizontalLayout_8->addWidget(btnPrevious);

        btnNext = new QPushButton(frameControl);
        btnNext->setObjectName(QStringLiteral("btnNext"));
        btnNext->setMinimumSize(QSize(43, 37));
        btnNext->setMaximumSize(QSize(43, 37));
        btnNext->setFocusPolicy(Qt::NoFocus);
        btnNext->setStyleSheet(QStringLiteral("border: none;"));
        QIcon icon4;
        icon4.addFile(QStringLiteral(":/Icons/next-track-button.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnNext->setIcon(icon4);
        btnNext->setIconSize(QSize(22, 22));

        horizontalLayout_8->addWidget(btnNext);


        horizontalLayout_9->addLayout(horizontalLayout_8);

        horizontalLayout_11 = new QHBoxLayout();
        horizontalLayout_11->setSpacing(6);
        horizontalLayout_11->setObjectName(QStringLiteral("horizontalLayout_11"));
        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setSpacing(4);
        horizontalLayout_10->setObjectName(QStringLiteral("horizontalLayout_10"));
        lbPosition = new QLabel(frameControl);
        lbPosition->setObjectName(QStringLiteral("lbPosition"));

        horizontalLayout_10->addWidget(lbPosition);

        label_4 = new QLabel(frameControl);
        label_4->setObjectName(QStringLiteral("label_4"));

        horizontalLayout_10->addWidget(label_4);

        lbDuration = new QLabel(frameControl);
        lbDuration->setObjectName(QStringLiteral("lbDuration"));

        horizontalLayout_10->addWidget(lbDuration);


        horizontalLayout_11->addLayout(horizontalLayout_10);

        sliderPosition = new QSlider(frameControl);
        sliderPosition->setObjectName(QStringLiteral("sliderPosition"));
        sliderPosition->setFocusPolicy(Qt::NoFocus);
        sliderPosition->setMaximum(100);
        sliderPosition->setValue(0);
        sliderPosition->setOrientation(Qt::Horizontal);

        horizontalLayout_11->addWidget(sliderPosition);


        horizontalLayout_9->addLayout(horizontalLayout_11);

        horizontalLayout_12 = new QHBoxLayout();
        horizontalLayout_12->setSpacing(2);
        horizontalLayout_12->setObjectName(QStringLiteral("horizontalLayout_12"));
        btnVolumeMute = new QPushButton(frameControl);
        btnVolumeMute->setObjectName(QStringLiteral("btnVolumeMute"));
        btnVolumeMute->setMinimumSize(QSize(31, 31));
        btnVolumeMute->setMaximumSize(QSize(31, 31));
        btnVolumeMute->setFocusPolicy(Qt::NoFocus);
        btnVolumeMute->setStyleSheet(QStringLiteral("border: none;"));
        QIcon icon5;
        icon5.addFile(QStringLiteral(":/Icons/volume-up-interface-symbol.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnVolumeMute->setIcon(icon5);
        btnVolumeMute->setIconSize(QSize(20, 20));

        horizontalLayout_12->addWidget(btnVolumeMute);

        sliderVolume = new QSlider(frameControl);
        sliderVolume->setObjectName(QStringLiteral("sliderVolume"));
        sizePolicy2.setHeightForWidth(sliderVolume->sizePolicy().hasHeightForWidth());
        sliderVolume->setSizePolicy(sizePolicy2);
        sliderVolume->setMinimumSize(QSize(111, 0));
        sliderVolume->setMaximumSize(QSize(111, 16777215));
        sliderVolume->setFocusPolicy(Qt::NoFocus);
        sliderVolume->setMaximum(100);
        sliderVolume->setPageStep(5);
        sliderVolume->setOrientation(Qt::Horizontal);

        horizontalLayout_12->addWidget(sliderVolume);


        horizontalLayout_9->addLayout(horizontalLayout_12);


        verticalLayout_4->addWidget(frameControl);

        MainWindow->setCentralWidget(centralWidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Handy Karaoke", 0));
        lbSearch->setText(QApplication::translate("MainWindow", "_", 0));
        lbLyrics->setText(QString());
        lbId->setText(QString());
        lbName->setText(QString());
        lbBtw->setText(QString());
        lbArtist->setText(QString());
        lbTempoKey->setText(QString());
        lbType->setText(QString());
        label->setText(QApplication::translate("MainWindow", "\340\270\243\340\270\262\340\270\242\340\270\201\340\270\262\340\270\243\340\271\200\340\270\245\340\271\210\340\270\231", 0));
#ifndef QT_NO_TOOLTIP
        btnPlay->setToolTip(QApplication::translate("MainWindow", "\340\271\200\340\270\245\340\271\210\340\270\231 (F5)", 0));
#endif // QT_NO_TOOLTIP
        btnPlay->setText(QString());
#ifndef QT_NO_TOOLTIP
        btnPause->setToolTip(QApplication::translate("MainWindow", "\340\270\253\340\270\242\340\270\270\340\270\224\340\270\212\340\270\261\340\271\210\340\270\247\340\270\204\340\270\243\340\270\262\340\270\247 (F6)", 0));
#endif // QT_NO_TOOLTIP
        btnPause->setText(QString());
#ifndef QT_NO_TOOLTIP
        btnStop->setToolTip(QApplication::translate("MainWindow", "\340\270\253\340\270\242\340\270\270\340\270\224 (F7)", 0));
#endif // QT_NO_TOOLTIP
        btnStop->setText(QString());
#ifndef QT_NO_TOOLTIP
        btnPrevious->setToolTip(QApplication::translate("MainWindow", "\340\271\200\340\270\236\340\270\245\340\270\207\340\270\201\340\271\210\340\270\255\340\270\231\340\270\253\340\270\231\340\271\211\340\270\262 (F8)", 0));
#endif // QT_NO_TOOLTIP
        btnPrevious->setText(QString());
#ifndef QT_NO_TOOLTIP
        btnNext->setToolTip(QApplication::translate("MainWindow", "\340\271\200\340\270\236\340\270\245\340\270\207\340\270\226\340\270\261\340\270\224\340\271\204\340\270\233 (F9)", 0));
#endif // QT_NO_TOOLTIP
        btnNext->setText(QString());
        lbPosition->setText(QApplication::translate("MainWindow", "00:00", 0));
        label_4->setText(QApplication::translate("MainWindow", "/", 0));
        lbDuration->setText(QApplication::translate("MainWindow", "00:00", 0));
        btnVolumeMute->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
