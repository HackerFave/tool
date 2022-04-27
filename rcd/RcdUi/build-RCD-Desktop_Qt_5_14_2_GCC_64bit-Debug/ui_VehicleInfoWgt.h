/********************************************************************************
** Form generated from reading UI file 'VehicleInfoWgt.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VEHICLEINFOWGT_H
#define UI_VEHICLEINFOWGT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "Component/Dashboard.h"
#include "Component/TGFrame.h"

QT_BEGIN_NAMESPACE

class Ui_VehicleInfoWgt
{
public:
    TGFrame *rightTitleFrame1;
    QGridLayout *gridLayout_3;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QGridLayout *gridLayout;
    QWidget *map_widget;
    QHBoxLayout *horizontalLayout;
    Dashboard *vehicleWgt;
    Dashboard *vehicleWgt_2;
    QHBoxLayout *horizontalLayout_2;
    Dashboard *vehicleWgt_3;
    Dashboard *vehicleWgt_4;
    QSpacerItem *verticalSpacer;
    TGFrame *rightTitleFrame2;
    QVBoxLayout *verticalLayout_2;
    QWidget *sysInfoWgt;
    QTextBrowser *sysInfo;

    void setupUi(QWidget *VehicleInfoWgt)
    {
        if (VehicleInfoWgt->objectName().isEmpty())
            VehicleInfoWgt->setObjectName(QString::fromUtf8("VehicleInfoWgt"));
        VehicleInfoWgt->resize(1017, 1057);
        VehicleInfoWgt->setStyleSheet(QString::fromUtf8(""));
        rightTitleFrame1 = new TGFrame(VehicleInfoWgt);
        rightTitleFrame1->setObjectName(QString::fromUtf8("rightTitleFrame1"));
        rightTitleFrame1->setGeometry(QRect(50, 70, 791, 451));
        gridLayout_3 = new QGridLayout(rightTitleFrame1);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        scrollArea = new QScrollArea(rightTitleFrame1);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 771, 369));
        gridLayout = new QGridLayout(scrollAreaWidgetContents);
        gridLayout->setSpacing(0);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        map_widget = new QWidget(scrollAreaWidgetContents);
        map_widget->setObjectName(QString::fromUtf8("map_widget"));

        gridLayout->addWidget(map_widget, 0, 0, 1, 1);

        scrollArea->setWidget(scrollAreaWidgetContents);

        gridLayout_3->addWidget(scrollArea, 1, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        vehicleWgt = new Dashboard(rightTitleFrame1);
        vehicleWgt->setObjectName(QString::fromUtf8("vehicleWgt"));
        vehicleWgt->setStyleSheet(QString::fromUtf8("background-color: rgb(78, 154, 6);"));

        horizontalLayout->addWidget(vehicleWgt);

        vehicleWgt_2 = new Dashboard(rightTitleFrame1);
        vehicleWgt_2->setObjectName(QString::fromUtf8("vehicleWgt_2"));
        vehicleWgt_2->setStyleSheet(QString::fromUtf8("background-color: rgb(138, 226, 52);"));

        horizontalLayout->addWidget(vehicleWgt_2);


        gridLayout_3->addLayout(horizontalLayout, 2, 0, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        vehicleWgt_3 = new Dashboard(rightTitleFrame1);
        vehicleWgt_3->setObjectName(QString::fromUtf8("vehicleWgt_3"));
        vehicleWgt_3->setStyleSheet(QString::fromUtf8("background-color: rgb(233, 185, 110);"));

        horizontalLayout_2->addWidget(vehicleWgt_3);

        vehicleWgt_4 = new Dashboard(rightTitleFrame1);
        vehicleWgt_4->setObjectName(QString::fromUtf8("vehicleWgt_4"));
        vehicleWgt_4->setStyleSheet(QString::fromUtf8("background-color: rgb(117, 80, 123);"));

        horizontalLayout_2->addWidget(vehicleWgt_4);


        gridLayout_3->addLayout(horizontalLayout_2, 3, 0, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout_3->addItem(verticalSpacer, 0, 0, 1, 1);

        rightTitleFrame2 = new TGFrame(VehicleInfoWgt);
        rightTitleFrame2->setObjectName(QString::fromUtf8("rightTitleFrame2"));
        rightTitleFrame2->setGeometry(QRect(270, 850, 571, 181));
        verticalLayout_2 = new QVBoxLayout(rightTitleFrame2);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(20, 20, 20, 20);
        sysInfoWgt = new QWidget(rightTitleFrame2);
        sysInfoWgt->setObjectName(QString::fromUtf8("sysInfoWgt"));
        sysInfo = new QTextBrowser(sysInfoWgt);
        sysInfo->setObjectName(QString::fromUtf8("sysInfo"));
        sysInfo->setGeometry(QRect(10, 10, 151, 71));
        QFont font;
        font.setPointSize(14);
        font.setBold(true);
        font.setWeight(75);
        sysInfo->setFont(font);
        sysInfo->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background:transparent;\n"
"border:none;"));
        sysInfo->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        sysInfo->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        sysInfo->setTextInteractionFlags(Qt::NoTextInteraction);

        verticalLayout_2->addWidget(sysInfoWgt);

        verticalLayout_2->setStretch(0, 9);

        retranslateUi(VehicleInfoWgt);

        QMetaObject::connectSlotsByName(VehicleInfoWgt);
    } // setupUi

    void retranslateUi(QWidget *VehicleInfoWgt)
    {
        VehicleInfoWgt->setWindowTitle(QCoreApplication::translate("VehicleInfoWgt", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class VehicleInfoWgt: public Ui_VehicleInfoWgt {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VEHICLEINFOWGT_H
