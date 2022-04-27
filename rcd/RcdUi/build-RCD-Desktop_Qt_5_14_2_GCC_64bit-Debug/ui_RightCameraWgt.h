/********************************************************************************
** Form generated from reading UI file 'RightCameraWgt.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RIGHTCAMERAWGT_H
#define UI_RIGHTCAMERAWGT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "Component/OpenglWidget.h"
#include "Component/TGFrame.h"

QT_BEGIN_NAMESPACE

class Ui_RightCameraWgt
{
public:
    TGFrame *rightVideoFrame1;
    OpenGLWidget *playWgt;
    TGFrame *rightTitleFrame1;
    QVBoxLayout *verticalLayout_2;
    QWidget *taskInfoWgt;
    TGFrame *rightTitleFrame2;
    QVBoxLayout *verticalLayout_3;
    QWidget *doInfoWgt;
    QLabel *stopOverLab;
    QLabel *exitQueueLab;
    QLabel *exitQueueImg;
    QLabel *stopOverImg;

    void setupUi(QWidget *RightCameraWgt)
    {
        if (RightCameraWgt->objectName().isEmpty())
            RightCameraWgt->setObjectName(QString::fromUtf8("RightCameraWgt"));
        RightCameraWgt->resize(800, 600);
        RightCameraWgt->setStyleSheet(QString::fromUtf8(""));
        rightVideoFrame1 = new TGFrame(RightCameraWgt);
        rightVideoFrame1->setObjectName(QString::fromUtf8("rightVideoFrame1"));
        rightVideoFrame1->setGeometry(QRect(90, 50, 291, 211));
        playWgt = new OpenGLWidget(rightVideoFrame1);
        playWgt->setObjectName(QString::fromUtf8("playWgt"));
        playWgt->setGeometry(QRect(21, 21, 249, 169));
        rightTitleFrame1 = new TGFrame(RightCameraWgt);
        rightTitleFrame1->setObjectName(QString::fromUtf8("rightTitleFrame1"));
        rightTitleFrame1->setGeometry(QRect(410, 50, 311, 211));
        verticalLayout_2 = new QVBoxLayout(rightTitleFrame1);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(20, 20, 20, 20);
        taskInfoWgt = new QWidget(rightTitleFrame1);
        taskInfoWgt->setObjectName(QString::fromUtf8("taskInfoWgt"));

        verticalLayout_2->addWidget(taskInfoWgt);

        rightTitleFrame2 = new TGFrame(RightCameraWgt);
        rightTitleFrame2->setObjectName(QString::fromUtf8("rightTitleFrame2"));
        rightTitleFrame2->setGeometry(QRect(70, 270, 411, 291));
        verticalLayout_3 = new QVBoxLayout(rightTitleFrame2);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(20, 20, 20, 20);
        doInfoWgt = new QWidget(rightTitleFrame2);
        doInfoWgt->setObjectName(QString::fromUtf8("doInfoWgt"));
        stopOverLab = new QLabel(doInfoWgt);
        stopOverLab->setObjectName(QString::fromUtf8("stopOverLab"));
        stopOverLab->setGeometry(QRect(49, 160, 84, 32));
        QFont font;
        font.setPointSize(16);
        stopOverLab->setFont(font);
        stopOverLab->setAlignment(Qt::AlignCenter);
        exitQueueLab = new QLabel(doInfoWgt);
        exitQueueLab->setObjectName(QString::fromUtf8("exitQueueLab"));
        exitQueueLab->setGeometry(QRect(223, 160, 84, 32));
        exitQueueLab->setFont(font);
        exitQueueLab->setAlignment(Qt::AlignCenter);
        exitQueueImg = new QLabel(doInfoWgt);
        exitQueueImg->setObjectName(QString::fromUtf8("exitQueueImg"));
        exitQueueImg->setGeometry(QRect(60, 70, 71, 81));
        stopOverImg = new QLabel(doInfoWgt);
        stopOverImg->setObjectName(QString::fromUtf8("stopOverImg"));
        stopOverImg->setGeometry(QRect(230, 70, 71, 81));

        verticalLayout_3->addWidget(doInfoWgt);


        retranslateUi(RightCameraWgt);

        QMetaObject::connectSlotsByName(RightCameraWgt);
    } // setupUi

    void retranslateUi(QWidget *RightCameraWgt)
    {
        RightCameraWgt->setWindowTitle(QCoreApplication::translate("RightCameraWgt", "Form", nullptr));
        stopOverLab->setText(QCoreApplication::translate("RightCameraWgt", "\345\201\234\351\235\240\345\256\214\346\210\220", nullptr));
        exitQueueLab->setText(QCoreApplication::translate("RightCameraWgt", "\351\200\200\345\207\272\346\216\222\351\230\237", nullptr));
        exitQueueImg->setText(QString());
        stopOverImg->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class RightCameraWgt: public Ui_RightCameraWgt {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RIGHTCAMERAWGT_H
