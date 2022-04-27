#include "abnormalcollector.h"
#include "ui_abnormalcollector.h"

AbnormalCollector::AbnormalCollector(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AbnormalCollector)
{
    ui->setupUi(this);
    buttonGroup = new QButtonGroup;

    buttonGroup->addButton(ui->abnormalManBtn, 1);
    buttonGroup->addButton(ui->abnormalRestopBtn, 2);
    buttonGroup->addButton(ui->abnormalClearListBtn, 3);
}

AbnormalCollector::~AbnormalCollector()
{
    delete ui;
}

void AbnormalCollector::clearChoice()
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
