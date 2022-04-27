#include "workstatechoice.h"
#include "ui_workstatechoice.h"

WorkStateChoice::WorkStateChoice(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WorkStateChoice)
{
    ui->setupUi(this);
    buttonGroup = new QButtonGroup;

    buttonGroup->addButton(ui->stopRightBtn, 1);
    buttonGroup->addButton(ui->loadStateBtn, 2);
    buttonGroup->addButton(ui->addOilBtn, 3);
    buttonGroup->addButton(ui->collectionBtn, 4);
    buttonGroup->addButton(ui->outWorkBtn, 5);
    buttonGroup->addButton(ui->staticBtn, 6);
    buttonGroup->addButton(ui->dynamicBtn, 7);
    buttonGroup->addButton(ui->clearErrorBtn, 8);
}

WorkStateChoice::~WorkStateChoice()
{
    delete ui;
}

void WorkStateChoice::clearChoice()
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
