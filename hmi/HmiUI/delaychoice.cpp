#include "delaychoice.h"
#include "ui_delaychoice.h"

DelayChoice::DelayChoice(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DelayChoice)
{
    ui->setupUi(this);
    buttonGroup = new QButtonGroup;

    buttonGroup->addButton(ui->delayChangeBtn, 1);
    buttonGroup->addButton(ui->delayFoodBtn, 2);
    buttonGroup->addButton(ui->delayPushBtn, 3);
    buttonGroup->addButton(ui->delayAvoidBtn, 4);
    buttonGroup->addButton(ui->delaySweepBtn, 5);
    buttonGroup->addButton(ui->delayInvertBtn, 6);
    buttonGroup->addButton(ui->delayMoveBtn, 7);
    buttonGroup->addButton(ui->delayDigBtn, 8);
    buttonGroup->addButton(ui->delayElectricBtn, 9);
}
DelayChoice::~DelayChoice()
{
    delete ui;
}

void DelayChoice::clearChoice()
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
