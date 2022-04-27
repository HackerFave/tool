#include "messageLable.h"
#include "ui_messageLable.h"

messageLable::messageLable(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::messageLable)
{
    ui->setupUi(this);
    connect(&_timer,SIGNAL(timeout()),this,SLOT(slotMessage()));
    _timer.start(1000);
//    setAttribute(Qt::WA_ShowModal, true);
    QPalette pal = palette();
    pal.setColor(QPalette::Background, QColor(0x00,0xff,0x00,0x00));
    setPalette(pal);
//    setAttribute(Qt::WA_DeleteOnClose);
}

messageLable::~messageLable()
{
    delete ui;
}
void messageLable::slotMessage()
{
    static int num = 0;
    if(num==0){
        ui->labelHint->setText("正在回收资源...");
    }
    else if (num == 1) {
        ui->labelHint->setText(" 正在回收资源...");
    }
    else if (num == 2) {
        ui->labelHint->setText("  正在回收资源...");
    }
    else if (num == 3) {
        ui->labelHint->setText("   正在回收资源...");
        num = 0;
    }
    num++;
}
