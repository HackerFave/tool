#include "messageBoxForm.h"
#include "ui_messageBoxForm.h"

messageBoxForm::messageBoxForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::messageBoxForm)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::WindowStaysOnTopHint/*| Qt::CustomizeWindowHint| Qt::Tool| Qt::FramelessWindowHint*/);
    this->setAttribute(Qt::WA_ShowModal);
    this->setFocus();
}

messageBoxForm::~messageBoxForm()
{
    delete ui;
}
