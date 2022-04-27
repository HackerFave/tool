#ifndef HMIMAP_H
#define HMIMAP_H

#include <QThread>

#include <QWidget>
#include <QStackedLayout>

#include "graphicsview.h"
#include "graphicsscene.h"

#include "mapdatathread.h"
#include "mapconsole.h"

#include "vehicleframe.h"
#include "vehicleexcavator.h"
#include "vehiclemine.h"
#include "vehiclecrush.h"

#include "pathroad.h"
#include "pathtask.h"
#include "pathframe.h"
#include "pathboundary.h"
#include "pathrealtime.h"
#include "pathcollection.h"
#include "vehicleparking.h"

#include "HmiFunction/filemanager.h"
#include "stoppoints.h"


namespace Ui {
class HmiMap;
}

class HmiMap : public QWidget
{
    Q_OBJECT

public:
    explicit HmiMap(QWidget *parent = 0);
    ~HmiMap();

    void setSelfVehicle(char type, QString name, QSize size);

    Ui::HmiMap *ui;

    GraphicsView* view;//视口
    GraphicsScene* scene;//画板
    mapConsole* console;//控制板
    QStackedLayout* mainlayout;//层叠布局

    MapDataThread *datathread;//外部数据文件处理线程

    VehicleFrame* selfVehicle;//车辆自身

    QMap<QString, PathBoundary*> boundaryItemMap;//边界号及边界图元组对应表
    QMap<QString, PathRoad*> roadItemMap;//道路号及道路图元组对应表
    PathCollection* collectionItemGroup;//装载区/卸载区边界图元
    PathTask* taskItemGroup;//任务图元祖对应表
    PathRealTime* realTimeItem;//实时采集图元
    PathPermission* permissionItemGroup;//许可图元对应表
    void addPermissionInfo(QStringList,quint32);

    //锚点
    volatile double latcenter;
    volatile double lngcenter;

    QMutex* sceneMutex;//场景操作锁
    QMap<struct pubVehicleCollWarnningST, VehicleFrame*> vehicleIdItemMap;//周边车辆图元对应表

public slots:
    void addCollectionLpxInfo(QString lpxpath);//添加边界采集信息
    void addCollectionListInfo(QList<struct latlngheading> list);
    void addDbInfo(QString dbpath);//添加全区地图数据信息
    void addTaskInfo(QStringList taskinfolist);//添加任务信息
    void clearMap();//清理地图数据信息
    void updateVehicleList(QList<struct pubVehicleCollWarnningST> vehiclelist);//更新v2v周边车辆信息

    void updateGpsState(char flag);//更新GPS信号状态

    void followSelf();//关注本车周边
    void followAll();//关注全区

    void updateVehiclePos(latlngheading);

    void addDbMap();
    void addCollection(struct collectlatlngheading*);
    void addCollectionPoint(struct collectlatlngheading *pos);
    void addLpxCollection();
    void addLpxTask();
    void addStopPoint(StopPoints *sp);
    void removeStopPoint(StopPoints *sp);

    void removeCollection();
    void updateCollection();
    void initView(QMatrix);

    void calcFinishedTask(struct latlngheading);
    void addLpxPermission();

signals:
    void changeGpsStateSignal(char);
};

#endif // HMIMAP_H
