#include "abnormalexcavator.h"
#include "ui_abnormalexcavator.h"

AbnormalExcavator::AbnormalExcavator(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AbnormalExcavator)
{
    ui->setupUi(this);
    buttonGroup = new QButtonGroup;

    buttonGroup->addButton(ui->abnormalManBtn, 1);
    buttonGroup->addButton(ui->abnormalRestopBtn, 2);
    buttonGroup->addButton(ui->abnormalClearListBtn, 3);
    buttonGroup->addButton(ui->abnormalHalfOutBtn, 4);
    buttonGroup->addButton(ui->abnormalEmptyOutBtn, 5);
}

AbnormalExcavator::~AbnormalExcavator()
{
    delete ui;
}

void AbnormalExcavator::clearChoice()
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
