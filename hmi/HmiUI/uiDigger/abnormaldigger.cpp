#include "abnormaldigger.h"
#include "ui_abnormaldigger.h"

AbnormalDigger::AbnormalDigger(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AbnormalDigger)
{
    ui->setupUi(this);
    buttonGroup = new QButtonGroup;

    buttonGroup->addButton(ui->abnormalManBtn, 1);
    buttonGroup->addButton(ui->abnormalRestopBtn, 2);
    buttonGroup->addButton(ui->abnormalClearListBtn, 3);
}

AbnormalDigger::~AbnormalDigger()
{
    delete ui;
}

void AbnormalDigger::clearChoice()
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
