#include "GearWgt.h"
#include <QRandomGenerator>
#include <QPainter>
#include <QDebug>

#define FREQUENCY       100

GearWgt::GearWgt(QWidget *parent) : QWidget(parent),
    m_currentValue(Gear_D)//党档位初始化
{
    m_gearWgtImage.load(":/resource/logo/gear.png");

    connect(&m_timer, SIGNAL(timeout()), this, SLOT(timerHandle()));
    //m_timer.start(1000);
}

void GearWgt::setValue(int value)
{
    m_currentValue = static_cast<GearWgtType>(value);
    this->update();
}


void GearWgt::timerHandle()
{
    static int i = -1;
    if(i > 1){
        i = -1;
    }
    this->setValue(static_cast<GearWgtType>(i));
    i++;
}

void GearWgt::paintEvent(QPaintEvent *)
{

    QPainter painter(this);

    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::LosslessImageRendering);

    painter.translate(width()/2, height()/2);
    qreal side = qMin(width()/630.0, height()/60.0);
    painter.scale(side , side);

    painter.drawPixmap(m_gearWgtImage.width()/(-2),m_gearWgtImage.height()/(-2),m_gearWgtImage);
    painter.setPen(Qt::white);
    QFont font = painter.font();
    font.setPixelSize(25);
    painter.setFont(font);

    QPoint Dpoint(m_gearWgtImage.width()/(-3),10);
    QPoint Npoint(0,10);
    QPoint Rpoint(m_gearWgtImage.width()/3 - 20,10);

    painter.drawText(Dpoint,QString::fromUtf8("D"));
    painter.drawText(Npoint,QString::fromUtf8("N"));
    painter.drawText(Rpoint,QString::fromUtf8("R"));

    painter.setPen(QPen(QColor(0xfa, 0x5a, 0x00),2));
    QPoint point;
    QString gearStr;

    switch (m_currentValue) {
    case Gear_D:
        point = Dpoint;
        gearStr = QString::fromUtf8("D");
        break;
    case Gear_N:
        point = Npoint;
        gearStr = QString::fromUtf8("N");
        break;
    case Gear_R:
        point = Rpoint;
        gearStr = QString::fromUtf8("R");
        break;
    default:
        point = Npoint;
        gearStr = QString::fromUtf8("N");
        qDebug() << "no this Gear_";
        break;
    }
    painter.drawText(point,gearStr);
    point += QPoint(0,10);
    point -= QPoint(12,0);
    QLine line(point,point+QPoint(40,0));
    painter.drawLine(line);

    painter.end();
}
