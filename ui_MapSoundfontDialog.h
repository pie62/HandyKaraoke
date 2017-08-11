/********************************************************************************
** Form generated from reading UI file 'MapSoundfontDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAPSOUNDFONTDIALOG_H
#define UI_MAPSOUNDFONTDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_MapSoundfontDialog
{
public:
    QVBoxLayout *verticalLayout;
    QTreeWidget *treeWidget;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *btnOk;
    QPushButton *btnCancel;

    void setupUi(QDialog *MapSoundfontDialog)
    {
        if (MapSoundfontDialog->objectName().isEmpty())
            MapSoundfontDialog->setObjectName(QStringLiteral("MapSoundfontDialog"));
        MapSoundfontDialog->resize(441, 447);
        QFont font;
        font.setPointSize(10);
        MapSoundfontDialog->setFont(font);
        verticalLayout = new QVBoxLayout(MapSoundfontDialog);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        treeWidget = new QTreeWidget(MapSoundfontDialog);
        treeWidget->setObjectName(QStringLiteral("treeWidget"));

        verticalLayout->addWidget(treeWidget);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        btnOk = new QPushButton(MapSoundfontDialog);
        btnOk->setObjectName(QStringLiteral("btnOk"));

        horizontalLayout->addWidget(btnOk);

        btnCancel = new QPushButton(MapSoundfontDialog);
        btnCancel->setObjectName(QStringLiteral("btnCancel"));

        horizontalLayout->addWidget(btnCancel);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(MapSoundfontDialog);

        QMetaObject::connectSlotsByName(MapSoundfontDialog);
    } // setupUi

    void retranslateUi(QDialog *MapSoundfontDialog)
    {
        MapSoundfontDialog->setWindowTitle(QApplication::translate("MapSoundfontDialog", "\340\270\201\340\270\262\340\270\243\340\271\200\340\270\245\340\270\267\340\270\255\340\270\201\340\271\203\340\270\212\340\271\211\340\270\213\340\270\262\340\270\247\340\270\224\340\271\214\340\270\237\340\271\211\340\270\255\340\270\231\340\270\227\340\271\214", 0));
        QTreeWidgetItem *___qtreewidgetitem = treeWidget->headerItem();
        ___qtreewidgetitem->setText(1, QApplication::translate("MapSoundfontDialog", "\340\270\213\340\270\262\340\270\247\340\270\224\340\271\214\340\270\237\340\271\211\340\270\255\340\270\231\340\270\227\340\271\214", 0));
        ___qtreewidgetitem->setText(0, QApplication::translate("MapSoundfontDialog", "\340\271\200\340\270\204\340\270\243\340\270\267\340\271\210\340\270\255\340\270\207\340\270\224\340\270\231\340\270\225\340\270\243\340\270\265", 0));
        btnOk->setText(QApplication::translate("MapSoundfontDialog", "\340\270\225\340\270\201\340\270\245\340\270\207", 0));
        btnCancel->setText(QApplication::translate("MapSoundfontDialog", "\340\270\242\340\270\201\340\271\200\340\270\245\340\270\264\340\270\201", 0));
    } // retranslateUi

};

namespace Ui {
    class MapSoundfontDialog: public Ui_MapSoundfontDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAPSOUNDFONTDIALOG_H
