#include "stopheadingchoice.h"
#include "ui_stopheadingchoice.h"
#include "HmiFunction/paraparsing.h"

StopHeadingChoice::StopHeadingChoice(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StopHeadingChoice)
{
    ui->setupUi(this);

    vehicleScene = new QGraphicsScene;
    ui->graphicsView->setScene(vehicleScene);

    itemSub90 = new vehicleItem(QSize(50,70), ":/image/resource/mineUnChecked.png", -90);
    itemSub45 = new vehicleItem(QSize(50,70), ":/image/resource/mineUnChecked.png", -45);
    itemAdd0  = new vehicleItem(QSize(50,70), ":/image/resource/mineUnChecked.png", 0);
    itemAdd45 = new vehicleItem(QSize(50,70), ":/image/resource/mineUnChecked.png", 45);
    itemAdd90 = new vehicleItem(QSize(50,70), ":/image/resource/mineUnChecked.png", 90);
    itemExcavator = new vehicleItem(QSize(220,100), ":/image/resource/excavator.png", 0, false);

    connect(itemSub90, SIGNAL(itemFocusedSignal(qreal )), this, SLOT(itemFocusedSlots(qreal )));
    connect(itemSub45, SIGNAL(itemFocusedSignal(qreal )), this, SLOT(itemFocusedSlots(qreal )));
    connect(itemAdd0 , SIGNAL(itemFocusedSignal(qreal )), this, SLOT(itemFocusedSlots(qreal )));
    connect(itemAdd45, SIGNAL(itemFocusedSignal(qreal )), this, SLOT(itemFocusedSlots(qreal )));
    connect(itemAdd90, SIGNAL(itemFocusedSignal(qreal )), this, SLOT(itemFocusedSlots(qreal )));

    vehicleScene->addItem(itemSub90);
    vehicleScene->addItem(itemSub45);
    vehicleScene->addItem(itemAdd0);
    vehicleScene->addItem(itemAdd45);
    vehicleScene->addItem(itemAdd90);
    vehicleScene->addItem(itemExcavator);



    qreal x = ui->graphicsView->width()/2-30;
    qreal y = ui->graphicsView->height()/2 + 30;
    qreal r = 120;

    itemExcavator->setPos(x,y);
    itemSub90->setPos(x-r,y);
    itemSub45->setPos(x-0.7*r, y-0.7*r);
    itemAdd0->setPos(x,y-r);
    itemAdd45->setPos(x+0.7*r, y-0.7*r);
    itemAdd90->setPos(x+r,y);

    //xuzhongkun 20200513

    ParaParsing* para = ParaParsing::instance();
    para->excavatorParsing();
    para->minetrunckParsing();

    stopLength = para->paraSt.excavator.stopLength;
    stopWidth = para->paraSt.excavator.stopWidth;
    value = stopCompAngle = para->paraSt.excavator.stopCompAngle;

    vehicleWidth = para->paraSt.minetrunck.vehicleWidth;
    vehicleLength = para->paraSt.minetrunck.vehicleLength;

    //qDebug()<<stopCompAngle;




    ui->labelStopLenA->setText("偏移\n长度");
    ui->labelStopWidthA->setText("偏移\n宽度");
    ui->labelVehicleLenA->setText("补偿\n长度");
    ui->labelVehicleWidthA->setText("补偿\n宽度");

    ui->doubleSpinBoxVehicleLen->setSingleStep(0.1);
    ui->doubleSpinBoxVehicleLen->setDecimals(1);
    ui->doubleSpinBoxVehicleLen->setMaximum(50);
    ui->doubleSpinBoxVehicleLen->setMinimum(-50);
    ui->doubleSpinBoxVehicleLen->setValue(vehicleLength);

    ui->doubleSpinBoxVehicleWidth->setSingleStep(0.1);
    ui->doubleSpinBoxVehicleWidth->setDecimals(1);
    ui->doubleSpinBoxVehicleWidth->setMaximum(50);
    ui->doubleSpinBoxVehicleWidth->setMinimum(-50);
    ui->doubleSpinBoxVehicleWidth->setValue(vehicleWidth);

    ui->doubleSpinBoxStopLen->setSingleStep(0.1);
    ui->doubleSpinBoxStopLen->setDecimals(1);
    ui->doubleSpinBoxStopLen->setMaximum(50);
    ui->doubleSpinBoxStopLen->setMinimum(-50);
    ui->doubleSpinBoxStopLen->setValue(stopLength);

    ui->doubleSpinBoxStopWidth->setSingleStep(0.1);
    ui->doubleSpinBoxStopWidth->setDecimals(1);
    ui->doubleSpinBoxStopWidth->setMaximum(50);
    ui->doubleSpinBoxStopWidth->setMinimum(-50);
    ui->doubleSpinBoxStopWidth->setValue(stopWidth);

    ui->valueLabel->setText(QString::number(stopCompAngle));
    if(para->paraSt.pub.vehicleType==2)//通用电铲不显示4个参数，默认0
    {
        ui->labelStopLenA->hide();
        ui->labelStopWidthA->hide();
        ui->labelVehicleLenA->hide();
        ui->labelVehicleWidthA->hide();

        ui->doubleSpinBoxVehicleLen->hide();

        ui->doubleSpinBoxVehicleWidth->hide();

        ui->doubleSpinBoxStopLen->hide();

        ui->doubleSpinBoxStopWidth->hide();


    }
    //xuzhongkun 20200513 end
}

StopHeadingChoice::~StopHeadingChoice()
{
    delete ui;
}

qreal StopHeadingChoice::getFixHeading()
{
    qreal ret;
    if(ui->valueLabel->text().isEmpty())
    {
        ret = 0;
    }
    else
    {
        ret = value;
    }
    if(qAbs(ret)>90)
        return 0;
    else return qAbs(ret);
}

void StopHeadingChoice::clearValue()
{
    value = 0;
    ui->valueLabel->setText("0");
    stopCompAngle=0;
}

void StopHeadingChoice::itemFocusedSlots(qreal angle)
{
    value = angle;
    ui->valueLabel->setText(QString::number(value));
    stopCompAngle=value;
}

void StopHeadingChoice::on_upBtn_clicked()
{
    if(value < 90)
    {
        value += 5;
        ui->valueLabel->setText(QString::number(value));
        stopCompAngle=value;
    }
}

void StopHeadingChoice::on_downBtn_clicked()
{
    if(value > -90)
    {
        value -= 5;
        ui->valueLabel->setText(QString::number(value));
        stopCompAngle=value;
    }
}

void StopHeadingChoice::on_doubleSpinBoxVehicleLen_valueChanged(double arg1)
{
    this->vehicleLength=arg1;

}

double StopHeadingChoice::getStopWidth()
{
    return stopWidth;
}

double StopHeadingChoice::getStopLength()
{
    return stopLength;
}

double StopHeadingChoice::getVehicleLength()
{
    return vehicleLength;
}

double StopHeadingChoice::getVehicleWidth()
{
    return vehicleWidth;
}

double StopHeadingChoice::getStopCompAngle()
{
    return stopCompAngle;
}

void StopHeadingChoice::setStopWidth(double value)
{
    this->stopWidth=value;
    ui->doubleSpinBoxStopWidth->setValue(value);
}

void StopHeadingChoice::setStopLength(double value)
{
    this->stopLength=value;
    ui->doubleSpinBoxStopLen->setValue(value);
}

void StopHeadingChoice::setVehicleLength(double value)
{
    this->vehicleLength=value;
    ui->doubleSpinBoxVehicleLen->setValue(value);
}

void StopHeadingChoice::setVehicleWidth(double value)
{
    this->vehicleWidth=value;
    ui->doubleSpinBoxVehicleWidth->setValue(value);
}

void StopHeadingChoice::setStopCompAngle(double value)
{
    this->stopCompAngle=value;
    ui->valueLabel->setText(QString::number(value));
}

void StopHeadingChoice::on_doubleSpinBoxVehicleWidth_valueChanged(double arg1)
{
    this->vehicleWidth=arg1;
}

void StopHeadingChoice::on_doubleSpinBoxStopLen_valueChanged(double arg1)
{
    this->stopLength=arg1;
}

void StopHeadingChoice::on_doubleSpinBoxStopWidth_valueChanged(double arg1)
{
    this->stopWidth=arg1;
}
