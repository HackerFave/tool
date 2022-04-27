#include "faultchoice.h"
#include "ui_faultchoice.h"

FaultChoice::FaultChoice(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FaultChoice)
{
    ui->setupUi(this);
    buttonGroup = new QButtonGroup;

    buttonGroup->addButton(ui->faultChangeBtn, 1);
    buttonGroup->addButton(ui->faultFoodBtn, 2);
    buttonGroup->addButton(ui->faultPushBtn, 3);
    buttonGroup->addButton(ui->faultAvoidBtn, 4);
    buttonGroup->addButton(ui->faultSweepBtn, 5);
    buttonGroup->addButton(ui->faultInvertBtn, 6);
    buttonGroup->addButton(ui->faultMoveBtn, 7);
    buttonGroup->addButton(ui->faultDigBtn, 8);
    buttonGroup->addButton(ui->faultElectricBtn, 9);
}

FaultChoice::~FaultChoice()
{
    delete ui;
}

void FaultChoice::clearChoice()
{
    if(buttonGroup->checkedButton() != NULL)
    {
        int id = buttonGroup->checkedId();
        QPushButton * checkedButton = static_cast<QPushButton*>(buttonGroup->checkedButton());
        buttonGroup->removeButton(checkedButton);
        checkedButton->setChecked(false);
        buttonGroup->addButton(checkedButton, id);
    }
}
