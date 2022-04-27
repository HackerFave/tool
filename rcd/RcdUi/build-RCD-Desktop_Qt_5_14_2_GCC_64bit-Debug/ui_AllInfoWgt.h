/********************************************************************************
** Form generated from reading UI file 'AllInfoWgt.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ALLINFOWGT_H
#define UI_ALLINFOWGT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>
#include "Component/TGFrame.h"

QT_BEGIN_NAMESPACE

class Ui_AllInfoWgt
{
public:
    TGFrame *rightTitle;

    void setupUi(QWidget *AllInfoWgt)
    {
        if (AllInfoWgt->objectName().isEmpty())
            AllInfoWgt->setObjectName(QString::fromUtf8("AllInfoWgt"));
        AllInfoWgt->resize(800, 600);
        AllInfoWgt->setStyleSheet(QString::fromUtf8(""));
        rightTitle = new TGFrame(AllInfoWgt);
        rightTitle->setObjectName(QString::fromUtf8("rightTitle"));
        rightTitle->setGeometry(QRect(10, 10, 481, 311));

        retranslateUi(AllInfoWgt);

        QMetaObject::connectSlotsByName(AllInfoWgt);
    } // setupUi

    void retranslateUi(QWidget *AllInfoWgt)
    {
        AllInfoWgt->setWindowTitle(QCoreApplication::translate("AllInfoWgt", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AllInfoWgt: public Ui_AllInfoWgt {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ALLINFOWGT_H
