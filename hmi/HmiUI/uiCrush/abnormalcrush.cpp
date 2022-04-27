#include "abnormalcrush.h"
#include "ui_abnormalcrush.h"

AbnormalCrush::AbnormalCrush(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AbnormalCrush)
{
    ui->setupUi(this);
    buttonGroup = new QButtonGroup;

    buttonGroup->addButton(ui->abnormalManBtn, 1);
    buttonGroup->addButton(ui->abnormalRestopBtn, 2);
    buttonGroup->addButton(ui->abnormalClearListBtn, 3);
    buttonGroup->addButton(ui->abnormalRecoveryBtn, 4);
    ui->abnormalClearListBtn->hide();
    ui->abnormalClearListBtnLabel->hide();
}

AbnormalCrush::~AbnormalCrush()
{
    delete ui;
}

void AbnormalCrush::clearChoice()
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
