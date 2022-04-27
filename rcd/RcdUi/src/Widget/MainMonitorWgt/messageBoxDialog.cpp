#include "messageBoxDialog.h"
#include "ui_messageBoxDialog.h"

messageBoxDialog::messageBoxDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::messageBoxDialog)
{
    ui->setupUi(this);
}

messageBoxDialog::~messageBoxDialog()
{
    delete ui;
}
