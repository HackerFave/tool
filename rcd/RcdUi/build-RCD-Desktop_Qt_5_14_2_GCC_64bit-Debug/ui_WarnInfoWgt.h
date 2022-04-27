/********************************************************************************
** Form generated from reading UI file 'WarnInfoWgt.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WARNINFOWGT_H
#define UI_WARNINFOWGT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>
#include "Component/TGFrame.h"

QT_BEGIN_NAMESPACE

class Ui_WarnInfoWgt
{
public:
    TGFrame *rightTitle;

    void setupUi(QWidget *WarnInfoWgt)
    {
        if (WarnInfoWgt->objectName().isEmpty())
            WarnInfoWgt->setObjectName(QString::fromUtf8("WarnInfoWgt"));
        WarnInfoWgt->resize(800, 600);
        WarnInfoWgt->setStyleSheet(QString::fromUtf8(""));
        rightTitle = new TGFrame(WarnInfoWgt);
        rightTitle->setObjectName(QString::fromUtf8("rightTitle"));
        rightTitle->setGeometry(QRect(20, 10, 361, 241));

        retranslateUi(WarnInfoWgt);

        QMetaObject::connectSlotsByName(WarnInfoWgt);
    } // setupUi

    void retranslateUi(QWidget *WarnInfoWgt)
    {
        WarnInfoWgt->setWindowTitle(QCoreApplication::translate("WarnInfoWgt", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class WarnInfoWgt: public Ui_WarnInfoWgt {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WARNINFOWGT_H
