/********************************************************************************
** Form generated from reading UI file 'SongDetail.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SONGDETAIL_H
#define UI_SONGDETAIL_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SongDetail
{
public:
    QVBoxLayout *verticalLayout;
    QFrame *frame;
    QFormLayout *formLayout;
    QLabel *label;
    QLabel *lbId;
    QLabel *label_2;
    QLabel *lbName;
    QLabel *label_3;
    QLabel *lbArtist;
    QLabel *label_4;
    QLabel *lbBpm;
    QLabel *label_5;
    QLabel *lbType;
    QLabel *label_6;
    QLabel *lbKey;

    void setupUi(QWidget *SongDetail)
    {
        if (SongDetail->objectName().isEmpty())
            SongDetail->setObjectName(QStringLiteral("SongDetail"));
        SongDetail->resize(300, 206);
        QFont font;
        font.setPointSize(11);
        font.setBold(true);
        font.setWeight(75);
        SongDetail->setFont(font);
        verticalLayout = new QVBoxLayout(SongDetail);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        frame = new QFrame(SongDetail);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setFont(font);
        frame->setStyleSheet(QLatin1String("#frame {\n"
"background-color: rgb(62, 62, 62, 200);\n"
"border:1px solid rgb(255, 255, 255);\n"
"}\n"
"\n"
"QLabel {\n"
"color: rgb(255, 255, 255);\n"
"	font: bold 11pt;\n"
"}"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        formLayout = new QFormLayout(frame);
        formLayout->setObjectName(QStringLiteral("formLayout"));
        label = new QLabel(frame);
        label->setObjectName(QStringLiteral("label"));

        formLayout->setWidget(0, QFormLayout::LabelRole, label);

        lbId = new QLabel(frame);
        lbId->setObjectName(QStringLiteral("lbId"));

        formLayout->setWidget(0, QFormLayout::FieldRole, lbId);

        label_2 = new QLabel(frame);
        label_2->setObjectName(QStringLiteral("label_2"));

        formLayout->setWidget(1, QFormLayout::LabelRole, label_2);

        lbName = new QLabel(frame);
        lbName->setObjectName(QStringLiteral("lbName"));

        formLayout->setWidget(1, QFormLayout::FieldRole, lbName);

        label_3 = new QLabel(frame);
        label_3->setObjectName(QStringLiteral("label_3"));

        formLayout->setWidget(2, QFormLayout::LabelRole, label_3);

        lbArtist = new QLabel(frame);
        lbArtist->setObjectName(QStringLiteral("lbArtist"));

        formLayout->setWidget(2, QFormLayout::FieldRole, lbArtist);

        label_4 = new QLabel(frame);
        label_4->setObjectName(QStringLiteral("label_4"));

        formLayout->setWidget(4, QFormLayout::LabelRole, label_4);

        lbBpm = new QLabel(frame);
        lbBpm->setObjectName(QStringLiteral("lbBpm"));

        formLayout->setWidget(4, QFormLayout::FieldRole, lbBpm);

        label_5 = new QLabel(frame);
        label_5->setObjectName(QStringLiteral("label_5"));

        formLayout->setWidget(5, QFormLayout::LabelRole, label_5);

        lbType = new QLabel(frame);
        lbType->setObjectName(QStringLiteral("lbType"));

        formLayout->setWidget(5, QFormLayout::FieldRole, lbType);

        label_6 = new QLabel(frame);
        label_6->setObjectName(QStringLiteral("label_6"));

        formLayout->setWidget(3, QFormLayout::LabelRole, label_6);

        lbKey = new QLabel(frame);
        lbKey->setObjectName(QStringLiteral("lbKey"));

        formLayout->setWidget(3, QFormLayout::FieldRole, lbKey);


        verticalLayout->addWidget(frame);


        retranslateUi(SongDetail);

        QMetaObject::connectSlotsByName(SongDetail);
    } // setupUi

    void retranslateUi(QWidget *SongDetail)
    {
        SongDetail->setWindowTitle(QApplication::translate("SongDetail", "Form", Q_NULLPTR));
        label->setText(QApplication::translate("SongDetail", "\340\270\243\340\270\253\340\270\261\340\270\252\340\271\200\340\270\236\340\270\245\340\270\207 :", Q_NULLPTR));
        lbId->setText(QString());
        label_2->setText(QApplication::translate("SongDetail", "\340\270\212\340\270\267\340\271\210\340\270\255\340\271\200\340\270\236\340\270\245\340\270\207 :", Q_NULLPTR));
        lbName->setText(QString());
        label_3->setText(QApplication::translate("SongDetail", "\340\270\212\340\270\267\340\271\210\340\270\255\340\270\250\340\270\264\340\270\245\340\270\233\340\270\264\340\270\231 :", Q_NULLPTR));
        lbArtist->setText(QString());
        label_4->setText(QApplication::translate("SongDetail", "\340\270\204\340\270\247\340\270\262\340\270\241\340\271\200\340\270\243\340\271\207\340\270\247 :", Q_NULLPTR));
        lbBpm->setText(QString());
        label_5->setText(QApplication::translate("SongDetail", "\340\270\212\340\270\231\340\270\264\340\270\224\340\271\200\340\270\236\340\270\245\340\270\207 :", Q_NULLPTR));
        lbType->setText(QString());
        label_6->setText(QApplication::translate("SongDetail", "\340\270\204\340\270\265\340\270\242\340\271\214\340\271\200\340\270\236\340\270\245\340\270\207 :", Q_NULLPTR));
        lbKey->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class SongDetail: public Ui_SongDetail {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SONGDETAIL_H
