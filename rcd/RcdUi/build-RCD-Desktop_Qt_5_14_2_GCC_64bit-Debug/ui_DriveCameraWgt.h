/********************************************************************************
** Form generated from reading UI file 'DriveCameraWgt.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DRIVECAMERAWGT_H
#define UI_DRIVECAMERAWGT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>
#include "Component/OpenglWidget.h"
#include "Component/TGFrame.h"

QT_BEGIN_NAMESPACE

class Ui_DriveCameraWgt
{
public:
    TGFrame *leftVideoFrame1;
    OpenGLWidget *playWgt;

    void setupUi(QWidget *DriveCameraWgt)
    {
        if (DriveCameraWgt->objectName().isEmpty())
            DriveCameraWgt->setObjectName(QString::fromUtf8("DriveCameraWgt"));
        DriveCameraWgt->resize(800, 600);
        DriveCameraWgt->setStyleSheet(QString::fromUtf8(""));
        leftVideoFrame1 = new TGFrame(DriveCameraWgt);
        leftVideoFrame1->setObjectName(QString::fromUtf8("leftVideoFrame1"));
        leftVideoFrame1->setGeometry(QRect(80, 90, 321, 211));
        leftVideoFrame1->setFocusPolicy(Qt::StrongFocus);
        leftVideoFrame1->setStyleSheet(QString::fromUtf8(""));
        playWgt = new OpenGLWidget(leftVideoFrame1);
        playWgt->setObjectName(QString::fromUtf8("playWgt"));
        playWgt->setGeometry(QRect(20, 20, 281, 171));
        playWgt->setFocusPolicy(Qt::NoFocus);
        playWgt->setStyleSheet(QString::fromUtf8(""));

        retranslateUi(DriveCameraWgt);

        QMetaObject::connectSlotsByName(DriveCameraWgt);
    } // setupUi

    void retranslateUi(QWidget *DriveCameraWgt)
    {
        DriveCameraWgt->setWindowTitle(QCoreApplication::translate("DriveCameraWgt", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DriveCameraWgt: public Ui_DriveCameraWgt {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DRIVECAMERAWGT_H
