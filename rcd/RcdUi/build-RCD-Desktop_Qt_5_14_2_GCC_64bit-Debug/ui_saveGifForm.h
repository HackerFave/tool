/********************************************************************************
** Form generated from reading UI file 'saveGifForm.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SAVEGIFFORM_H
#define UI_SAVEGIFFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_saveGifForm
{
public:
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox_screenRecord;
    QGridLayout *gridLayout_2;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer_6;
    QCheckBox *checkBox_allselect;
    QSpacerItem *horizontalSpacer_7;
    QSpacerItem *horizontalSpacer_8;
    QSpacerItem *horizontalSpacer_5;
    QHBoxLayout *horizontalLayout_4;
    QSpacerItem *horizontalSpacer_3;
    QCheckBox *checkBox_leftScreen;
    QLabel *label_left;
    QCheckBox *checkBox_mainScreen;
    QLabel *label_main;
    QCheckBox *checkBox_rightScreen;
    QLabel *label_right;
    QSpacerItem *horizontalSpacer_4;
    QFrame *frame;
    QHBoxLayout *horizontalLayout_2;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer_9;
    QPushButton *pushButton_cancel;
    QPushButton *pushButton_ok;
    QSpacerItem *horizontalSpacer_2;

    void setupUi(QWidget *saveGifForm)
    {
        if (saveGifForm->objectName().isEmpty())
            saveGifForm->setObjectName(QString::fromUtf8("saveGifForm"));
        saveGifForm->resize(541, 289);
        saveGifForm->setStyleSheet(QString::fromUtf8("QWidget#saveGifForm{\n"
"background-color: rgb(46, 52, 54);\n"
"}\n"
""));
        gridLayout = new QGridLayout(saveGifForm);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        groupBox_screenRecord = new QGroupBox(saveGifForm);
        groupBox_screenRecord->setObjectName(QString::fromUtf8("groupBox_screenRecord"));
        groupBox_screenRecord->setEnabled(true);
        QFont font;
        font.setFamily(QString::fromUtf8("\346\245\267\344\275\223"));
        font.setBold(true);
        font.setItalic(false);
        font.setWeight(75);
        groupBox_screenRecord->setFont(font);
        groupBox_screenRecord->setLayoutDirection(Qt::LeftToRight);
        groupBox_screenRecord->setStyleSheet(QString::fromUtf8("QGroupBox#groupBox_screenRecord\n"
"{\n"
"color: rgb(243, 243, 243);\n"
"border: 2px solid red;\n"
"border-radius:5px;\n"
"font:bold 28px;\n"
" margin-top:2ex;\n"
"font-family:\346\245\267\344\275\223;\n"
"font:bold 28px;\n"
"}\n"
" \n"
"QGroupBox#groupBox_screenRecord::title\n"
"{\n"
"subcontrol-origin:margin;\n"
"subcontrol-position:top center;\n"
"padding:0 3px;\n"
"}\n"
"QGroupBox#groupBox_screenRecord::indicator {\n"
"    /* \351\200\211\346\213\251\346\241\206\345\260\272\345\257\270 */\n"
"    width:23px;\n"
"    height:23px;\n"
"}\n"
"/*\350\277\231\346\230\257\345\257\271CheckBox\347\232\204\346\240\267\345\274\217\350\277\233\350\241\214\350\256\276\345\256\232*/\n"
"/*QGroupBox::indicator:unchecked {\n"
"    image: url(:/Ampctrl/Recources/image/UnCheck_.png);\n"
" /*image: url(:/Ampctrl/Recources/image/UCheckIn.png);\n"
"}*/\n"
"/*QGroupBox::indicator:checked {\n"
"    image: url(:/Ampctrl/Recources/image/Check_.png);\n"
" image: url(:/Ampctrl/Recources/image/CheckIn.png);\n"
"} */"));
        groupBox_screenRecord->setAlignment(Qt::AlignBottom|Qt::AlignRight|Qt::AlignTrailing);
        groupBox_screenRecord->setFlat(false);
        groupBox_screenRecord->setCheckable(false);
        groupBox_screenRecord->setChecked(false);
        gridLayout_2 = new QGridLayout(groupBox_screenRecord);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_6);

        checkBox_allselect = new QCheckBox(groupBox_screenRecord);
        checkBox_allselect->setObjectName(QString::fromUtf8("checkBox_allselect"));
        checkBox_allselect->setStyleSheet(QString::fromUtf8("QCheckBox#checkBox_allselect{\n"
"   	color: rgb(243, 243, 243);\n"
"font:18px;\n"
"}\n"
"\n"
"QCheckBox#checkBox_allselect::indicator {\n"
"    /* \351\200\211\346\213\251\346\241\206\345\260\272\345\257\270 */\n"
"    width:18px;\n"
"    height:18px;\n"
"	color: rgb(211, 215, 207);\n"
"}\n"
"/*\350\277\231\346\230\257\345\257\271CheckBox\347\232\204\346\240\267\345\274\217\350\277\233\350\241\214\350\256\276\345\256\232*/\n"
"QCheckBox#checkBox_allselect::indicator:unchecked\n"
"{\n"
"    image: url(:/resource/icon/unchecke.png);\n"
"    width:18px;\n"
"    height:18px;\n"
"	color: rgb(211, 215, 207);\n"
"}\n"
"QCheckBox#checkBox_allselect::indicator:checked\n"
"{\n"
"	image: url(:/resource/icon/checked.png);\n"
"    width:18px;\n"
"    height:18px;\n"
"	color: rgb(211, 215, 207);\n"
"}"));

        horizontalLayout_3->addWidget(checkBox_allselect);

        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_7);

        horizontalSpacer_8 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_8);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_5);


        gridLayout_2->addLayout(horizontalLayout_3, 0, 0, 1, 1);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_3);

        checkBox_leftScreen = new QCheckBox(groupBox_screenRecord);
        checkBox_leftScreen->setObjectName(QString::fromUtf8("checkBox_leftScreen"));
        checkBox_leftScreen->setStyleSheet(QString::fromUtf8("QCheckBox#checkBox_leftScreen{\n"
"   /* color:red;\n"
"    background-color:rgb(101,101,101);*/\n"
"	color: rgb(243, 243, 243);\n"
"    font: 18px;\n"
"}\n"
"\n"
"QCheckBox#checkBox_leftScreen::indicator {\n"
"    /* \351\200\211\346\213\251\346\241\206\345\260\272\345\257\270 */\n"
"    width:18px;\n"
"    height:18px;\n"
"}\n"
"/*\350\277\231\346\230\257\345\257\271CheckBox\347\232\204\346\240\267\345\274\217\350\277\233\350\241\214\350\256\276\345\256\232*/\n"
"QCheckBox::indicator:unchecked\n"
"{\n"
"    image: url(:/resource/icon/unchecke.png);\n"
"    width:18px;\n"
"    height:18px;\n"
"}\n"
"QCheckBox::indicator:checked\n"
"{\n"
"	image: url(:/resource/icon/checked.png);\n"
"    width:18px;\n"
"    height:18px;\n"
"}"));

        horizontalLayout_4->addWidget(checkBox_leftScreen);

        label_left = new QLabel(groupBox_screenRecord);
        label_left->setObjectName(QString::fromUtf8("label_left"));
        label_left->setStyleSheet(QString::fromUtf8("color: rgb(243, 243, 243);"));
        label_left->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        horizontalLayout_4->addWidget(label_left);

        checkBox_mainScreen = new QCheckBox(groupBox_screenRecord);
        checkBox_mainScreen->setObjectName(QString::fromUtf8("checkBox_mainScreen"));
        checkBox_mainScreen->setStyleSheet(QString::fromUtf8("QCheckBox#checkBox_mainScreen{\n"
"   	color: rgb(243, 243, 243);\n"
"font:18px;\n"
"}\n"
"\n"
"QCheckBox#checkBox_mainScreen::indicator {\n"
"    /* \351\200\211\346\213\251\346\241\206\345\260\272\345\257\270 */\n"
"    width:18px;\n"
"    height:18px;\n"
"	color: rgb(211, 215, 207);\n"
"}\n"
"/*\350\277\231\346\230\257\345\257\271CheckBox\347\232\204\346\240\267\345\274\217\350\277\233\350\241\214\350\256\276\345\256\232*/\n"
"QCheckBox#checkBox_mainScreen::indicator:unchecked\n"
"{\n"
"    image: url(:/resource/icon/unchecke.png);\n"
"    width:18px;\n"
"    height:18px;\n"
"	color: rgb(211, 215, 207);\n"
"}\n"
"QCheckBox#checkBox_mainScreen::indicator:checked\n"
"{\n"
"	image: url(:/resource/icon/checked.png);\n"
"    width:18px;\n"
"    height:18px;\n"
"	color: rgb(211, 215, 207);\n"
"}"));

        horizontalLayout_4->addWidget(checkBox_mainScreen);

        label_main = new QLabel(groupBox_screenRecord);
        label_main->setObjectName(QString::fromUtf8("label_main"));
        label_main->setStyleSheet(QString::fromUtf8("color: rgb(243, 243, 243);"));
        label_main->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        horizontalLayout_4->addWidget(label_main);

        checkBox_rightScreen = new QCheckBox(groupBox_screenRecord);
        checkBox_rightScreen->setObjectName(QString::fromUtf8("checkBox_rightScreen"));
        checkBox_rightScreen->setLayoutDirection(Qt::LeftToRight);
        checkBox_rightScreen->setStyleSheet(QString::fromUtf8("QCheckBox#checkBox_rightScreen{\n"
"	color: rgb(243, 243, 243);\n"
"font:18px;\n"
"}\n"
"\n"
"QCheckBox#checkBox_rightScreen::indicator {\n"
"    /* \351\200\211\346\213\251\346\241\206\345\260\272\345\257\270 */\n"
"    width:18px;\n"
"    height:18px;\n"
"}\n"
"/*\350\277\231\346\230\257\345\257\271CheckBox\347\232\204\346\240\267\345\274\217\350\277\233\350\241\214\350\256\276\345\256\232*/\n"
"QCheckBox#checkBox_rightScreen::indicator:unchecked\n"
"{\n"
"    image: url(:/resource/icon/unchecke.png);\n"
"    width:18px;\n"
"    height:18px;\n"
"}\n"
"QCheckBox#checkBox_rightScreen::indicator:checked\n"
"{\n"
"	image: url(:/resource/icon/checked.png);\n"
"    width:18px;\n"
"    height:18px;\n"
"}"));
        checkBox_rightScreen->setCheckable(true);

        horizontalLayout_4->addWidget(checkBox_rightScreen);

        label_right = new QLabel(groupBox_screenRecord);
        label_right->setObjectName(QString::fromUtf8("label_right"));
        label_right->setStyleSheet(QString::fromUtf8("color: rgb(243, 243, 243);"));
        label_right->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        horizontalLayout_4->addWidget(label_right);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_4);


        gridLayout_2->addLayout(horizontalLayout_4, 1, 0, 1, 1);


        verticalLayout->addWidget(groupBox_screenRecord);

        frame = new QFrame(saveGifForm);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        horizontalLayout_2 = new QHBoxLayout(frame);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer_9 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_9);

        pushButton_cancel = new QPushButton(frame);
        pushButton_cancel->setObjectName(QString::fromUtf8("pushButton_cancel"));

        horizontalLayout->addWidget(pushButton_cancel);

        pushButton_ok = new QPushButton(frame);
        pushButton_ok->setObjectName(QString::fromUtf8("pushButton_ok"));

        horizontalLayout->addWidget(pushButton_ok);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);


        horizontalLayout_2->addLayout(horizontalLayout);


        verticalLayout->addWidget(frame);


        gridLayout->addLayout(verticalLayout, 0, 0, 1, 1);


        retranslateUi(saveGifForm);

        QMetaObject::connectSlotsByName(saveGifForm);
    } // setupUi

    void retranslateUi(QWidget *saveGifForm)
    {
        saveGifForm->setWindowTitle(QCoreApplication::translate("saveGifForm", "Form", nullptr));
        groupBox_screenRecord->setTitle(QCoreApplication::translate("saveGifForm", "\345\275\225\345\261\217", nullptr));
        checkBox_allselect->setText(QCoreApplication::translate("saveGifForm", "  \345\205\250\351\200\211", nullptr));
        checkBox_leftScreen->setText(QCoreApplication::translate("saveGifForm", "  \345\267\246\345\261\217\345\271\225", nullptr));
        label_left->setText(QString());
        checkBox_mainScreen->setText(QCoreApplication::translate("saveGifForm", "  \344\270\273\345\261\217\345\271\225", nullptr));
        label_main->setText(QString());
        checkBox_rightScreen->setText(QCoreApplication::translate("saveGifForm", "  \345\217\263\345\261\217\345\271\225", nullptr));
        label_right->setText(QString());
        pushButton_cancel->setText(QCoreApplication::translate("saveGifForm", "\345\217\226\346\266\210", nullptr));
        pushButton_ok->setText(QCoreApplication::translate("saveGifForm", "\347\241\256\345\256\232", nullptr));
    } // retranslateUi

};

namespace Ui {
    class saveGifForm: public Ui_saveGifForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SAVEGIFFORM_H
