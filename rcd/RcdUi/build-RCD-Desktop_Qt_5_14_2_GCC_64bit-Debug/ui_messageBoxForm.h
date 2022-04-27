/********************************************************************************
** Form generated from reading UI file 'messageBoxForm.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MESSAGEBOXFORM_H
#define UI_MESSAGEBOXFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_messageBoxForm
{
public:
    QGridLayout *gridLayout_2;
    QVBoxLayout *verticalLayout;
    QFrame *frame_title;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_4;
    QToolButton *toolButton;
    QWidget *widget;
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *pushButton;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton_2;
    QSpacerItem *horizontalSpacer_3;

    void setupUi(QWidget *messageBoxForm)
    {
        if (messageBoxForm->objectName().isEmpty())
            messageBoxForm->setObjectName(QString::fromUtf8("messageBoxForm"));
        messageBoxForm->resize(438, 265);
        gridLayout_2 = new QGridLayout(messageBoxForm);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        frame_title = new QFrame(messageBoxForm);
        frame_title->setObjectName(QString::fromUtf8("frame_title"));
        frame_title->setMaximumSize(QSize(16777215, 40));
        frame_title->setFrameShape(QFrame::StyledPanel);
        frame_title->setFrameShadow(QFrame::Raised);
        horizontalLayout_2 = new QHBoxLayout(frame_title);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer_4 = new QSpacerItem(365, 17, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_4);

        toolButton = new QToolButton(frame_title);
        toolButton->setObjectName(QString::fromUtf8("toolButton"));
        toolButton->setStyleSheet(QString::fromUtf8("background-color: rgb(85, 87, 83);"));

        horizontalLayout_2->addWidget(toolButton);


        verticalLayout->addWidget(frame_title);

        widget = new QWidget(messageBoxForm);
        widget->setObjectName(QString::fromUtf8("widget"));
        gridLayout = new QGridLayout(widget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        pushButton = new QPushButton(widget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        horizontalLayout->addWidget(pushButton);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pushButton_2 = new QPushButton(widget);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));

        horizontalLayout->addWidget(pushButton_2);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_3);


        gridLayout->addLayout(horizontalLayout, 0, 0, 1, 1);


        verticalLayout->addWidget(widget);


        gridLayout_2->addLayout(verticalLayout, 0, 0, 1, 1);


        retranslateUi(messageBoxForm);

        QMetaObject::connectSlotsByName(messageBoxForm);
    } // setupUi

    void retranslateUi(QWidget *messageBoxForm)
    {
        messageBoxForm->setWindowTitle(QCoreApplication::translate("messageBoxForm", "Form", nullptr));
        toolButton->setText(QString());
        pushButton->setText(QCoreApplication::translate("messageBoxForm", "PushButton", nullptr));
        pushButton_2->setText(QCoreApplication::translate("messageBoxForm", "PushButton", nullptr));
    } // retranslateUi

};

namespace Ui {
    class messageBoxForm: public Ui_messageBoxForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MESSAGEBOXFORM_H
