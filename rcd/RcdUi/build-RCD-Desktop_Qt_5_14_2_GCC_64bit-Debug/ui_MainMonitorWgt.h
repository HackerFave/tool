/********************************************************************************
** Form generated from reading UI file 'MainMonitorWgt.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINMONITORWGT_H
#define UI_MAINMONITORWGT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "Component/OpenglWidget.h"
#include "Component/PageWgt.h"
#include "Component/TGFrame.h"

QT_BEGIN_NAMESPACE

class Ui_MainMonitorWgt
{
public:
    QVBoxLayout *verticalLayout;
    QWidget *title;
    TGFrame *midVideoFrame1;
    QVBoxLayout *verticalLayout_2;
    OpenGLWidget *playWgt1;
    QLabel *labelImei1;
    TGFrame *midVideoFrame2;
    QVBoxLayout *verticalLayout_3;
    OpenGLWidget *playWgt2;
    QLabel *labelImei2;
    TGFrame *midVideoFrame3;
    QVBoxLayout *verticalLayout_4;
    OpenGLWidget *playWgt3;
    QLabel *labelImei3;
    TGFrame *midVideoFrame4;
    QVBoxLayout *verticalLayout_5;
    OpenGLWidget *playWgt4;
    QLabel *labelImei4;
    TGFrame *midVideoFrame5;
    QVBoxLayout *verticalLayout_6;
    OpenGLWidget *playWgt5;
    QLabel *labelImei5;
    TGFrame *midVideoFrame6;
    QVBoxLayout *verticalLayout_7;
    OpenGLWidget *playWgt6;
    QLabel *labelImei6;
    PageWgt *pageWgt;

    void setupUi(QWidget *MainMonitorWgt)
    {
        if (MainMonitorWgt->objectName().isEmpty())
            MainMonitorWgt->setObjectName(QString::fromUtf8("MainMonitorWgt"));
        MainMonitorWgt->resize(800, 600);
        MainMonitorWgt->setStyleSheet(QString::fromUtf8(""));
        verticalLayout = new QVBoxLayout(MainMonitorWgt);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        title = new QWidget(MainMonitorWgt);
        title->setObjectName(QString::fromUtf8("title"));
        midVideoFrame1 = new TGFrame(title);
        midVideoFrame1->setObjectName(QString::fromUtf8("midVideoFrame1"));
        midVideoFrame1->setGeometry(QRect(10, 0, 291, 191));
        verticalLayout_2 = new QVBoxLayout(midVideoFrame1);
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(30, 30, 30, 30);
        playWgt1 = new OpenGLWidget(midVideoFrame1);
        playWgt1->setObjectName(QString::fromUtf8("playWgt1"));
        labelImei1 = new QLabel(playWgt1);
        labelImei1->setObjectName(QString::fromUtf8("labelImei1"));
        labelImei1->setGeometry(QRect(10, 10, 151, 21));

        verticalLayout_2->addWidget(playWgt1);

        midVideoFrame2 = new TGFrame(title);
        midVideoFrame2->setObjectName(QString::fromUtf8("midVideoFrame2"));
        midVideoFrame2->setGeometry(QRect(10, 200, 291, 191));
        midVideoFrame2->setInputMethodHints(Qt::ImhNone);
        verticalLayout_3 = new QVBoxLayout(midVideoFrame2);
        verticalLayout_3->setSpacing(0);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(30, 30, 30, 30);
        playWgt2 = new OpenGLWidget(midVideoFrame2);
        playWgt2->setObjectName(QString::fromUtf8("playWgt2"));
        labelImei2 = new QLabel(playWgt2);
        labelImei2->setObjectName(QString::fromUtf8("labelImei2"));
        labelImei2->setGeometry(QRect(10, 10, 151, 21));

        verticalLayout_3->addWidget(playWgt2);

        midVideoFrame3 = new TGFrame(title);
        midVideoFrame3->setObjectName(QString::fromUtf8("midVideoFrame3"));
        midVideoFrame3->setGeometry(QRect(10, 400, 291, 191));
        verticalLayout_4 = new QVBoxLayout(midVideoFrame3);
        verticalLayout_4->setSpacing(0);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        verticalLayout_4->setContentsMargins(30, 30, 30, 30);
        playWgt3 = new OpenGLWidget(midVideoFrame3);
        playWgt3->setObjectName(QString::fromUtf8("playWgt3"));
        labelImei3 = new QLabel(playWgt3);
        labelImei3->setObjectName(QString::fromUtf8("labelImei3"));
        labelImei3->setGeometry(QRect(10, 10, 151, 21));

        verticalLayout_4->addWidget(playWgt3);

        midVideoFrame4 = new TGFrame(title);
        midVideoFrame4->setObjectName(QString::fromUtf8("midVideoFrame4"));
        midVideoFrame4->setGeometry(QRect(450, 0, 291, 191));
        verticalLayout_5 = new QVBoxLayout(midVideoFrame4);
        verticalLayout_5->setSpacing(0);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        verticalLayout_5->setContentsMargins(30, 30, 30, 30);
        playWgt4 = new OpenGLWidget(midVideoFrame4);
        playWgt4->setObjectName(QString::fromUtf8("playWgt4"));
        labelImei4 = new QLabel(playWgt4);
        labelImei4->setObjectName(QString::fromUtf8("labelImei4"));
        labelImei4->setGeometry(QRect(10, 10, 151, 21));

        verticalLayout_5->addWidget(playWgt4);

        midVideoFrame5 = new TGFrame(title);
        midVideoFrame5->setObjectName(QString::fromUtf8("midVideoFrame5"));
        midVideoFrame5->setGeometry(QRect(450, 200, 291, 191));
        verticalLayout_6 = new QVBoxLayout(midVideoFrame5);
        verticalLayout_6->setSpacing(0);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        verticalLayout_6->setContentsMargins(30, 30, 30, 30);
        playWgt5 = new OpenGLWidget(midVideoFrame5);
        playWgt5->setObjectName(QString::fromUtf8("playWgt5"));
        labelImei5 = new QLabel(playWgt5);
        labelImei5->setObjectName(QString::fromUtf8("labelImei5"));
        labelImei5->setGeometry(QRect(10, 10, 151, 21));

        verticalLayout_6->addWidget(playWgt5);

        midVideoFrame6 = new TGFrame(title);
        midVideoFrame6->setObjectName(QString::fromUtf8("midVideoFrame6"));
        midVideoFrame6->setGeometry(QRect(450, 400, 291, 191));
        verticalLayout_7 = new QVBoxLayout(midVideoFrame6);
        verticalLayout_7->setSpacing(0);
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
        verticalLayout_7->setContentsMargins(30, 30, 30, 30);
        playWgt6 = new OpenGLWidget(midVideoFrame6);
        playWgt6->setObjectName(QString::fromUtf8("playWgt6"));
        labelImei6 = new QLabel(playWgt6);
        labelImei6->setObjectName(QString::fromUtf8("labelImei6"));
        labelImei6->setGeometry(QRect(10, 10, 151, 21));

        verticalLayout_7->addWidget(playWgt6);

        pageWgt = new PageWgt(title);
        pageWgt->setObjectName(QString::fromUtf8("pageWgt"));
        pageWgt->setGeometry(QRect(320, 440, 120, 80));

        verticalLayout->addWidget(title);


        retranslateUi(MainMonitorWgt);

        QMetaObject::connectSlotsByName(MainMonitorWgt);
    } // setupUi

    void retranslateUi(QWidget *MainMonitorWgt)
    {
        MainMonitorWgt->setWindowTitle(QCoreApplication::translate("MainMonitorWgt", "Form", nullptr));
        labelImei1->setText(QString());
        labelImei2->setText(QString());
        labelImei3->setText(QString());
        labelImei4->setText(QString());
        labelImei5->setText(QString());
        labelImei6->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainMonitorWgt: public Ui_MainMonitorWgt {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINMONITORWGT_H
