/********************************************************************************
** Form generated from reading UI file 'MidWgt.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MIDWGT_H
#define UI_MIDWGT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MidWgt
{
public:

    void setupUi(QWidget *MidWgt)
    {
        if (MidWgt->objectName().isEmpty())
            MidWgt->setObjectName(QString::fromUtf8("MidWgt"));
        MidWgt->resize(1280, 800);

        retranslateUi(MidWgt);

        QMetaObject::connectSlotsByName(MidWgt);
    } // setupUi

    void retranslateUi(QWidget *MidWgt)
    {
        MidWgt->setWindowTitle(QCoreApplication::translate("MidWgt", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MidWgt: public Ui_MidWgt {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MIDWGT_H
