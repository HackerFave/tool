#ifndef VEHICLEFRAME_H
#define VEHICLEFRAME_H

#include <QObject>

#include <QTextDocument>
#include <QPixmap>

#include <QGraphicsTextItem>
#include <QGraphicsPixmapItem>
#include <QGraphicsItemGroup>

#include "HmiUI/uiExcavator/vehicleitem.h"

/*
 * type定义:
 * 1:矿卡(装载)
 * 2:矿卡(空载)
 * 3:电铲
 * 4:推土机
 * 5:破碎站
 * 6:装载机
 * 7:边界采集车
*/

#define MINELOADER      1
#define MINEUNLOADER    2
#define EXCAVATOR       3
#define DIGGER          4
#define CRUSHER         5
#define LOADER          6
#define COLLECTOR       7
#define BULLDOZER       8
#define PARKING         9

class VehicleFrame : public QGraphicsItemGroup
{
public:
    VehicleFrame(char type, QString name, QSize size, uint8_t personalization=1);
    ~VehicleFrame();
    virtual int getType() = 0;
    virtual void setType(char,uint8_t) = 0;
    virtual QString getName() = 0;
    virtual void setName(QString) = 0;
    virtual char getState() = 0;
    virtual void setState(char) = 0;

    void setHeading(float heading);//设置航向
    float getHeading();//获取航向

    void setPos(QPointF pos);//设置位置
    void setPos(double, double);
    QPointF getPos();//获取位置
    void setPointOffset(QPointF offset);
    QPointF off;

    void setImei(QByteArray imei);//设置IMEI
    QByteArray getImei();//获取IMEI

    int type;
    float heading;
    QString name;
    char Imei[15];
    char state;
    QSize size;

    QGraphicsTextItem* nameItem;
    QGraphicsPixmapItem* typeItem;
    QGraphicsItemGroup* group;

signals:

public slots:
};

#endif // VEHICLEFRAME_H
