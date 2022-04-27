/********************************************************************************
** Form generated from reading UI file 'CloudWebWgt.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CLOUDWEBWGT_H
#define UI_CLOUDWEBWGT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>
#include "Component/TGFrame.h"

QT_BEGIN_NAMESPACE

class Ui_CloudWebWgt
{
public:
    TGFrame *webFrame1;

    void setupUi(QWidget *CloudWebWgt)
    {
        if (CloudWebWgt->objectName().isEmpty())
            CloudWebWgt->setObjectName(QString::fromUtf8("CloudWebWgt"));
        CloudWebWgt->resize(800, 600);
        CloudWebWgt->setStyleSheet(QString::fromUtf8(""));
        webFrame1 = new TGFrame(CloudWebWgt);
        webFrame1->setObjectName(QString::fromUtf8("webFrame1"));
        webFrame1->setGeometry(QRect(60, 140, 671, 151));
        webFrame1->setStyleSheet(QString::fromUtf8(""));

        retranslateUi(CloudWebWgt);

        QMetaObject::connectSlotsByName(CloudWebWgt);
    } // setupUi

    void retranslateUi(QWidget *CloudWebWgt)
    {
        CloudWebWgt->setWindowTitle(QCoreApplication::translate("CloudWebWgt", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CloudWebWgt: public Ui_CloudWebWgt {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CLOUDWEBWGT_H
