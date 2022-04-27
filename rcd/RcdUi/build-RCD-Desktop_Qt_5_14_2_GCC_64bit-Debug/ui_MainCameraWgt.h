/********************************************************************************
** Form generated from reading UI file 'MainCameraWgt.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINCAMERAWGT_H
#define UI_MAINCAMERAWGT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "Component/GearWgt.h"
#include "Component/OpenglWidget.h"
#include "Component/ReversingTrackLine.h"
#include "Component/RingDial.h"
#include "Component/SteerWheel.h"
#include "Component/TGFrame.h"
#include "Component/TriangleFill.h"
#include "Component/backTrackLine.h"

QT_BEGIN_NAMESPACE

class Ui_MainCameraWgt
{
public:
    QWidget *cameraTitle;
    QVBoxLayout *CVLayout;
    TGFrame *modelWgt;
    TGFrame *pingWgt;
    QGridLayout *gridLayout;
    SteerWheel *wheelWgt;
    GearWgt *gearWgt;
    TriangleFill *throttleWgt;
    TriangleFill *brakeWgt;
    RingDial *tachometerWgt;
    RingDial *speedWgt;
    QWidget *zero_2;
    TGFrame *midVideoFrame1;
    QHBoxLayout *minPLayout;
    OpenGLWidget *backPlayWgt;
    QGridLayout *gridLayout_3;
    backTrackLine *widget_ReversingTrackLine_b;
    OpenGLWidget *frontPlayWgt;
    QGridLayout *gridLayout_2;
    backTrackLine *widget_ReversingTrackLine_f;
    ReversingTrackLine *widget_ReversingTrackLine_old;

    void setupUi(QWidget *MainCameraWgt)
    {
        if (MainCameraWgt->objectName().isEmpty())
            MainCameraWgt->setObjectName(QString::fromUtf8("MainCameraWgt"));
        MainCameraWgt->resize(1024, 768);
        MainCameraWgt->setStyleSheet(QString::fromUtf8(""));
        cameraTitle = new QWidget(MainCameraWgt);
        cameraTitle->setObjectName(QString::fromUtf8("cameraTitle"));
        cameraTitle->setGeometry(QRect(10, 10, 961, 391));
        CVLayout = new QVBoxLayout(cameraTitle);
        CVLayout->setSpacing(0);
        CVLayout->setObjectName(QString::fromUtf8("CVLayout"));
        CVLayout->setContentsMargins(-1, 0, -1, 0);
        modelWgt = new TGFrame(cameraTitle);
        modelWgt->setObjectName(QString::fromUtf8("modelWgt"));

        CVLayout->addWidget(modelWgt);

        pingWgt = new TGFrame(cameraTitle);
        pingWgt->setObjectName(QString::fromUtf8("pingWgt"));

        CVLayout->addWidget(pingWgt);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        wheelWgt = new SteerWheel(cameraTitle);
        wheelWgt->setObjectName(QString::fromUtf8("wheelWgt"));

        gridLayout->addWidget(wheelWgt, 0, 5, 2, 1);

        gearWgt = new GearWgt(cameraTitle);
        gearWgt->setObjectName(QString::fromUtf8("gearWgt"));

        gridLayout->addWidget(gearWgt, 2, 4, 1, 3);

        throttleWgt = new TriangleFill(cameraTitle);
        throttleWgt->setObjectName(QString::fromUtf8("throttleWgt"));

        gridLayout->addWidget(throttleWgt, 1, 7, 2, 1);

        brakeWgt = new TriangleFill(cameraTitle);
        brakeWgt->setObjectName(QString::fromUtf8("brakeWgt"));

        gridLayout->addWidget(brakeWgt, 1, 2, 2, 1);

        tachometerWgt = new RingDial(cameraTitle);
        tachometerWgt->setObjectName(QString::fromUtf8("tachometerWgt"));

        gridLayout->addWidget(tachometerWgt, 1, 6, 1, 1);

        speedWgt = new RingDial(cameraTitle);
        speedWgt->setObjectName(QString::fromUtf8("speedWgt"));

        gridLayout->addWidget(speedWgt, 1, 4, 1, 1);

        zero_2 = new QWidget(cameraTitle);
        zero_2->setObjectName(QString::fromUtf8("zero_2"));

        gridLayout->addWidget(zero_2, 0, 8, 3, 2);

        midVideoFrame1 = new TGFrame(cameraTitle);
        midVideoFrame1->setObjectName(QString::fromUtf8("midVideoFrame1"));
        minPLayout = new QHBoxLayout(midVideoFrame1);
        minPLayout->setSpacing(0);
        minPLayout->setObjectName(QString::fromUtf8("minPLayout"));
        minPLayout->setContentsMargins(20, 15, 20, 15);
        backPlayWgt = new OpenGLWidget(midVideoFrame1);
        backPlayWgt->setObjectName(QString::fromUtf8("backPlayWgt"));
        gridLayout_3 = new QGridLayout(backPlayWgt);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        widget_ReversingTrackLine_b = new backTrackLine(backPlayWgt);
        widget_ReversingTrackLine_b->setObjectName(QString::fromUtf8("widget_ReversingTrackLine_b"));

        gridLayout_3->addWidget(widget_ReversingTrackLine_b, 0, 0, 1, 1);


        minPLayout->addWidget(backPlayWgt);


        gridLayout->addWidget(midVideoFrame1, 0, 0, 2, 2);


        CVLayout->addLayout(gridLayout);

        CVLayout->setStretch(0, 1);
        CVLayout->setStretch(1, 1);
        CVLayout->setStretch(2, 1);
        frontPlayWgt = new OpenGLWidget(MainCameraWgt);
        frontPlayWgt->setObjectName(QString::fromUtf8("frontPlayWgt"));
        frontPlayWgt->setGeometry(QRect(520, 520, 371, 241));
        gridLayout_2 = new QGridLayout(frontPlayWgt);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        widget_ReversingTrackLine_f = new backTrackLine(frontPlayWgt);
        widget_ReversingTrackLine_f->setObjectName(QString::fromUtf8("widget_ReversingTrackLine_f"));

        gridLayout_2->addWidget(widget_ReversingTrackLine_f, 0, 0, 1, 1);

        widget_ReversingTrackLine_old = new ReversingTrackLine(frontPlayWgt);
        widget_ReversingTrackLine_old->setObjectName(QString::fromUtf8("widget_ReversingTrackLine_old"));
        QFont font;
        font.setFamily(QString::fromUtf8("Ubuntu"));
        widget_ReversingTrackLine_old->setFont(font);

        gridLayout_2->addWidget(widget_ReversingTrackLine_old, 0, 1, 1, 1);


        retranslateUi(MainCameraWgt);

        QMetaObject::connectSlotsByName(MainCameraWgt);
    } // setupUi

    void retranslateUi(QWidget *MainCameraWgt)
    {
        MainCameraWgt->setWindowTitle(QCoreApplication::translate("MainCameraWgt", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainCameraWgt: public Ui_MainCameraWgt {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINCAMERAWGT_H
