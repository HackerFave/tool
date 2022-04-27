#include "fixsetdigger.h"
#include "ui_fixsetdigger.h"

FixSetDigger::FixSetDigger(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FixSetDigger)
{
    ui->setupUi(this);
}

FixSetDigger::~FixSetDigger()
{
    delete ui;
}

double FixSetDigger::getFixValue()
{
    return ui->fixValueSpinBox->value();
}

double FixSetDigger::getLength()
{
    return ui->mineLengthSpinBox->value();
}

double FixSetDigger::getWidth()
{
    return ui->mineWidthSpinBox->value();
}

void FixSetDigger::setFixValue(double value)
{
    ui->fixValueSpinBox->setValue(value);
}

void FixSetDigger::setLength(double value)
{
    ui->mineLengthSpinBox->setValue(value);
}

void FixSetDigger::setWidth(double value)
{
    ui->mineWidthSpinBox->setValue(value);
}
