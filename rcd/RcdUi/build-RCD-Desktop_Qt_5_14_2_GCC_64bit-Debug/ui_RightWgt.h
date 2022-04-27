/********************************************************************************
** Form generated from reading UI file 'RightWgt.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RIGHTWGT_H
#define UI_RIGHTWGT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RightWgt
{
public:

    void setupUi(QWidget *RightWgt)
    {
        if (RightWgt->objectName().isEmpty())
            RightWgt->setObjectName(QString::fromUtf8("RightWgt"));
        RightWgt->resize(1280, 800);

        retranslateUi(RightWgt);

        QMetaObject::connectSlotsByName(RightWgt);
    } // setupUi

    void retranslateUi(QWidget *RightWgt)
    {
        RightWgt->setWindowTitle(QCoreApplication::translate("RightWgt", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class RightWgt: public Ui_RightWgt {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RIGHTWGT_H
