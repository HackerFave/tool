#include "materchoice.h"
#include "ui_materchoice.h"

MaterChoice::MaterChoice(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MaterChoice)
{
    ui->setupUi(this);

    buttonGroup = new QButtonGroup;

    buttonGroup->addButton(ui->baiYunMaterBtn, 1);
    buttonGroup->addButton(ui->highMagneticBtn, 2);
    buttonGroup->addButton(ui->lowMagneticBtn, 3);
}

MaterChoice::~MaterChoice()
{
    delete ui;
}

void MaterChoice::clearChoice()
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
