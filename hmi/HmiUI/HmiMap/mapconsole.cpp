#include "mapconsole.h"
#include "ui_mapconsole.h"

mapConsole::mapConsole(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::mapConsole)
{
    ui->setupUi(this);
}

mapConsole::~mapConsole()
{
    delete ui;
}

void mapConsole::gpsStateChange(char flag)
{
    switch(flag)
    {
        case 0:
        {
            ui->GpsLabel->setStyleSheet("border-image:url(:/image/resource/gpsWarnning.png);");
            break;
        }
        case 1:
        {
            ui->GpsLabel->setStyleSheet("border-image:url(:/image/resource/gpsNormal.png);");
            break;
        }
        case 2:
        {
            ui->GpsLabel->setStyleSheet("border-image:url(:/image/resource/rtk.png);");
            break;
        }
        default:
        {
            break;
        }
    }
}


void mapConsole::on_mapMinusBtn_clicked()
{
    emit zoomOutSignal();
}

void mapConsole::on_mapPlusBtn_clicked()
{
    emit zoomInSignal();
}
