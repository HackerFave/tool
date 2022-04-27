/********************************************************************************
** Form generated from reading UI file 'LeftCameraWgt.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LEFTCAMERAWGT_H
#define UI_LEFTCAMERAWGT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>
#include "Component/OpenglWidget.h"
#include "Component/TGFrame.h"

QT_BEGIN_NAMESPACE

class Ui_LeftCameraWgt
{
public:
    TGFrame *leftVideoFrame1;
    OpenGLWidget *playWgt;

    void setupUi(QWidget *LeftCameraWgt)
    {
        if (LeftCameraWgt->objectName().isEmpty())
            LeftCameraWgt->setObjectName(QString::fromUtf8("LeftCameraWgt"));
        LeftCameraWgt->resize(800, 600);
        LeftCameraWgt->setStyleSheet(QString::fromUtf8(""));
        leftVideoFrame1 = new TGFrame(LeftCameraWgt);
        leftVideoFrame1->setObjectName(QString::fromUtf8("leftVideoFrame1"));
        leftVideoFrame1->setGeometry(QRect(80, 90, 321, 211));
        leftVideoFrame1->setFocusPolicy(Qt::StrongFocus);
        leftVideoFrame1->setStyleSheet(QString::fromUtf8(""));
        playWgt = new OpenGLWidget(leftVideoFrame1);
        playWgt->setObjectName(QString::fromUtf8("playWgt"));
        playWgt->setGeometry(QRect(20, 20, 281, 171));
        playWgt->setFocusPolicy(Qt::NoFocus);
        playWgt->setStyleSheet(QString::fromUtf8(""));

        retranslateUi(LeftCameraWgt);

        QMetaObject::connectSlotsByName(LeftCameraWgt);
    } // setupUi

    void retranslateUi(QWidget *LeftCameraWgt)
    {
        LeftCameraWgt->setWindowTitle(QCoreApplication::translate("LeftCameraWgt", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class LeftCameraWgt: public Ui_LeftCameraWgt {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LEFTCAMERAWGT_H
