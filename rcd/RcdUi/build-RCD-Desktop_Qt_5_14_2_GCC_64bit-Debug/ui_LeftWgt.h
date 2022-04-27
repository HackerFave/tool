/********************************************************************************
** Form generated from reading UI file 'LeftWgt.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LEFTWGT_H
#define UI_LEFTWGT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LeftWgt
{
public:
    QWidget *logoWgt;

    void setupUi(QWidget *LeftWgt)
    {
        if (LeftWgt->objectName().isEmpty())
            LeftWgt->setObjectName(QString::fromUtf8("LeftWgt"));
        LeftWgt->resize(1280, 800);
        logoWgt = new QWidget(LeftWgt);
        logoWgt->setObjectName(QString::fromUtf8("logoWgt"));
        logoWgt->setGeometry(QRect(90, 110, 371, 211));
        logoWgt->setStyleSheet(QString::fromUtf8(""));

        retranslateUi(LeftWgt);

        QMetaObject::connectSlotsByName(LeftWgt);
    } // setupUi

    void retranslateUi(QWidget *LeftWgt)
    {
        LeftWgt->setWindowTitle(QCoreApplication::translate("LeftWgt", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class LeftWgt: public Ui_LeftWgt {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LEFTWGT_H
