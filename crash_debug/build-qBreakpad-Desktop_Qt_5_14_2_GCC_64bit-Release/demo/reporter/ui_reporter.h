/********************************************************************************
** Form generated from reading UI file 'reporter.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REPORTER_H
#define UI_REPORTER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ReporterExample
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *urlLabel;
    QLineEdit *urlLineEdit;
    QLabel *dumpFilesLabel;
    QPlainTextEdit *dumpFilesTextEdit;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QPushButton *crashButton;
    QPushButton *uploadButton;

    void setupUi(QDialog *ReporterExample)
    {
        if (ReporterExample->objectName().isEmpty())
            ReporterExample->setObjectName(QString::fromUtf8("ReporterExample"));
        ReporterExample->resize(400, 300);
        verticalLayout = new QVBoxLayout(ReporterExample);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        urlLabel = new QLabel(ReporterExample);
        urlLabel->setObjectName(QString::fromUtf8("urlLabel"));

        verticalLayout->addWidget(urlLabel);

        urlLineEdit = new QLineEdit(ReporterExample);
        urlLineEdit->setObjectName(QString::fromUtf8("urlLineEdit"));

        verticalLayout->addWidget(urlLineEdit);

        dumpFilesLabel = new QLabel(ReporterExample);
        dumpFilesLabel->setObjectName(QString::fromUtf8("dumpFilesLabel"));

        verticalLayout->addWidget(dumpFilesLabel);

        dumpFilesTextEdit = new QPlainTextEdit(ReporterExample);
        dumpFilesTextEdit->setObjectName(QString::fromUtf8("dumpFilesTextEdit"));
        QFont font;
        font.setFamily(QString::fromUtf8("Courier"));
        font.setPointSize(11);
        dumpFilesTextEdit->setFont(font);
        dumpFilesTextEdit->setAutoFillBackground(false);
        dumpFilesTextEdit->setUndoRedoEnabled(false);
        dumpFilesTextEdit->setReadOnly(true);
        dumpFilesTextEdit->setBackgroundVisible(false);

        verticalLayout->addWidget(dumpFilesTextEdit);

        widget = new QWidget(ReporterExample);
        widget->setObjectName(QString::fromUtf8("widget"));
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        crashButton = new QPushButton(widget);
        crashButton->setObjectName(QString::fromUtf8("crashButton"));

        horizontalLayout->addWidget(crashButton);

        uploadButton = new QPushButton(widget);
        uploadButton->setObjectName(QString::fromUtf8("uploadButton"));

        horizontalLayout->addWidget(uploadButton);


        verticalLayout->addWidget(widget);


        retranslateUi(ReporterExample);

        QMetaObject::connectSlotsByName(ReporterExample);
    } // setupUi

    void retranslateUi(QDialog *ReporterExample)
    {
        ReporterExample->setWindowTitle(QCoreApplication::translate("ReporterExample", "ReporterExample", nullptr));
        urlLabel->setText(QCoreApplication::translate("ReporterExample", "Crash upload url", nullptr));
        urlLineEdit->setPlaceholderText(QCoreApplication::translate("ReporterExample", "http://your.site/crash_upload", nullptr));
        dumpFilesLabel->setText(QCoreApplication::translate("ReporterExample", "Crash dumps", nullptr));
        crashButton->setText(QCoreApplication::translate("ReporterExample", "Crash!", nullptr));
        uploadButton->setText(QCoreApplication::translate("ReporterExample", "Upload dumps", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ReporterExample: public Ui_ReporterExample {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REPORTER_H
