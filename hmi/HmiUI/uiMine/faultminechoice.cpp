#include "faultminechoice.h"
#include "ui_faultminechoice.h"

FaultMineChoice::FaultMineChoice(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FaultMineChoice)
{
    ui->setupUi(this);
    buttonGroup = new QButtonGroup;

    buttonGroup->addButton(ui->faultMineBtn, 1);
}

FaultMineChoice::~FaultMineChoice()
{
    delete ui;
}

void FaultMineChoice::clearChoice()
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
