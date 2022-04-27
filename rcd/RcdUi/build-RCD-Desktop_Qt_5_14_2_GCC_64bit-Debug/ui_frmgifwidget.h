/********************************************************************************
** Form generated from reading UI file 'frmgifwidget.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FRMGIFWIDGET_H
#define UI_FRMGIFWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_frmGifWidget
{
public:
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *pushButton_togif;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton_movie;
    QSpacerItem *horizontalSpacer_4;
    QPushButton *pushButton_test;
    QSpacerItem *horizontalSpacer_3;
    QSpacerItem *verticalSpacer_2;
    QLabel *label_time;

    void setupUi(QWidget *frmGifWidget)
    {
        if (frmGifWidget->objectName().isEmpty())
            frmGifWidget->setObjectName(QString::fromUtf8("frmGifWidget"));
        frmGifWidget->resize(526, 416);
        gridLayout = new QGridLayout(frmGifWidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        pushButton_togif = new QPushButton(frmGifWidget);
        pushButton_togif->setObjectName(QString::fromUtf8("pushButton_togif"));
        pushButton_togif->setCheckable(true);

        horizontalLayout->addWidget(pushButton_togif);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pushButton_movie = new QPushButton(frmGifWidget);
        pushButton_movie->setObjectName(QString::fromUtf8("pushButton_movie"));

        horizontalLayout->addWidget(pushButton_movie);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_4);

        pushButton_test = new QPushButton(frmGifWidget);
        pushButton_test->setObjectName(QString::fromUtf8("pushButton_test"));

        horizontalLayout->addWidget(pushButton_test);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_3);


        verticalLayout->addLayout(horizontalLayout);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);

        label_time = new QLabel(frmGifWidget);
        label_time->setObjectName(QString::fromUtf8("label_time"));
        label_time->setMinimumSize(QSize(0, 30));
        label_time->setStyleSheet(QString::fromUtf8("background-color: rgb(46, 52, 54);\n"
"color: rgb(255, 255, 255);"));
        label_time->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label_time);


        gridLayout->addLayout(verticalLayout, 0, 0, 1, 1);


        retranslateUi(frmGifWidget);

        QMetaObject::connectSlotsByName(frmGifWidget);
    } // setupUi

    void retranslateUi(QWidget *frmGifWidget)
    {
        frmGifWidget->setWindowTitle(QCoreApplication::translate("frmGifWidget", "Form", nullptr));
        pushButton_togif->setText(QCoreApplication::translate("frmGifWidget", "\345\274\200\345\220\257", nullptr));
        pushButton_movie->setText(QCoreApplication::translate("frmGifWidget", "\350\247\206\351\242\221\345\233\236\346\224\276", nullptr));
        pushButton_test->setText(QCoreApplication::translate("frmGifWidget", "\346\265\213\350\257\225", nullptr));
        label_time->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class frmGifWidget: public Ui_frmGifWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FRMGIFWIDGET_H
