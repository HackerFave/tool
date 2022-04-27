#include "hmimap.h"
#include "ui_hmimap.h"

HmiMap::HmiMap(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HmiMap)
{
    ui->setupUi(this);

    view = new GraphicsView;
    scene = new GraphicsScene;
    console = new mapConsole;
    mainlayout = new QStackedLayout;

    datathread = new MapDataThread;

    sceneMutex = new QMutex;
    selfVehicle = NULL;
    taskItemGroup = NULL;
    permissionItemGroup = NULL;
    realTimeItem = NULL;

    lngcenter = -1;
    latcenter = -1;

    view->setScene(scene);
    scene->setBackgroundBrush(Qt::transparent);

    mainlayout->setStackingMode(QStackedLayout::StackAll);
    mainlayout->addWidget(view);
    mainlayout->addWidget(console);

    this->setLayout(mainlayout);

    QThread *data_thread = new QThread;
    datathread->moveToThread(data_thread);
    data_thread->start();

    connect(datathread, SIGNAL(dbAnalysisComplete()), this, SLOT(addDbMap()));
    connect(datathread, SIGNAL(lpxCollAnalysisComplete()), this, SLOT(addLpxCollection()));
    connect(datathread, SIGNAL(lpxTaskAnalysisComplete()), this, SLOT(addLpxTask()));
    connect(datathread, SIGNAL(lpxPermissionAnalysisComplete()), this, SLOT(addLpxPermission()));

    connect(this, SIGNAL(changeGpsStateSignal(char)), console, SLOT(gpsStateChange(char)));
    connect(console, SIGNAL(zoomInSignal()), view, SLOT(zoomInSlot()));
    connect(console, SIGNAL(zoomOutSignal()), view, SLOT(zoomOutSlot()));

}

HmiMap::~HmiMap()
{
    delete ui;
}

void HmiMap::setSelfVehicle(char type, QString name, QSize size)
{
    if(selfVehicle != NULL)
    {
        scene->removeItem(selfVehicle->typeItem);
        scene->removeItem(selfVehicle->nameItem);
        scene->removeItem(selfVehicle);

        delete selfVehicle;
        selfVehicle = NULL;
    }

    switch(type)
    {
        case MINELOADER:
        {
            selfVehicle = new VehicleMine(type, name, size,0);
            break;
        }
        case MINEUNLOADER:
        {
            selfVehicle = new VehicleMine(type, name, size,0);
            break;
        }
        case EXCAVATOR:
        {
            selfVehicle = new VehicleExcavator(type, name, size,0);
            break;
        }
        case DIGGER:
        {
            selfVehicle = new VehicleMine(type, name, size,0);
            break;
        }
        case CRUSHER:
        {
            selfVehicle = new VehicleCrush(type, name, size,0);
            break;
        }

        case PARKING:
        {
            selfVehicle = new VehicleParking(type, name, size,0);
            break;
        }
        case LOADER:
        {
            break;
        }
        case COLLECTOR:
        {
            selfVehicle = new VehicleCrush(type, name, size,0);
            break;
        }
        default:
            break;
    }
    if(selfVehicle!=NULL)
    {

        scene->addItem(selfVehicle);
        scene->addItem(selfVehicle->typeItem);
        scene->addItem(selfVehicle->nameItem);
    }
}

void HmiMap::addCollectionLpxInfo(QString lpxpath)//获取采集文件并绘制
{
    datathread->getPosInfoFromLpxCollection(lpxpath);
}

void HmiMap::addCollectionListInfo(QList<latlngheading> list)
{
    collectionItemGroup->update();
}

void HmiMap::addDbInfo(QString dbpath)
{
    clearMap();    
    datathread->getPosInfoFromDB(dbpath);
}

void HmiMap::clearMap()
{
    scene->clear();
    boundaryItemMap.clear();
    roadItemMap.clear();
    if(selfVehicle!=NULL)
    {
        //scence clear已经清除
        //delete selfVehicle;
        selfVehicle = NULL;
    }
    if(realTimeItem!=NULL)
    {
        //delete realTimeItem;
        realTimeItem = NULL;
    }
}

void HmiMap::addTaskInfo(QStringList taskinfolist)
{
    if(taskItemGroup != NULL)
    {
        scene->removeItem(taskItemGroup);
        taskItemGroup = NULL;
    }

    datathread->getPosInfoFromLpxTask(taskinfolist);
}

void HmiMap::addPermissionInfo(QStringList permissionInfolist,quint32 endPointNum)
{
    if(permissionItemGroup != NULL)
    {
        scene->removeItem(permissionItemGroup);
        permissionItemGroup = NULL;
    }

    datathread->getPosInfoFromLpxPermission(permissionInfolist,endPointNum);
}

void HmiMap::updateVehicleList(QList<struct pubVehicleCollWarnningST> vehiclelist)
{
    //删除不存在的周边车辆
    sceneMutex->lock();

    QMap<struct pubVehicleCollWarnningST, VehicleFrame*>::iterator itor;
//    struct pubVehicleCollWarnningST key;

//    int count = 0;
    for(itor = vehicleIdItemMap.begin(); itor != vehicleIdItemMap.end();)
    {
//        key = itor.key();
        if(!vehiclelist.contains(itor.key()))
        {
            VehicleFrame* value = itor.value();
            scene->removeItem(value->typeItem);
            scene->removeItem(value->nameItem);
            scene->removeItem(value);

            itor = vehicleIdItemMap.erase(itor);
        }
        else
        {
            itor++;
        }

    }

    for(int count = 0; count < vehiclelist.size(); count++)
    {
        //添加存在的周边车辆
        if(!vehicleIdItemMap.contains(vehiclelist.at(count)))
        {
            VehicleFrame* newVehicle;
            quint64 width=real2pix*vehiclelist.at(count).dVehicleWidth;
            quint64 length=real2pix*vehiclelist.at(count).dVehicleLength;
            if(width<10)width=10;
            if(length<10)length=10;
            QSize size(width, length);

            switch(vehiclelist.at(count).vehicleType)
            {
                case 0x51:
                {
                    newVehicle = new VehicleMine(MINEUNLOADER, vehiclelist.at(count).dName, size,1);
                    break;
                }
                case 0x52:
                case 0x54:
                {
                    newVehicle = new VehicleMine(EXCAVATOR, vehiclelist.at(count).dName, size,1);
                    break;
                }
                case 0x53:
                {
                    newVehicle = new VehicleMine(DIGGER, vehiclelist.at(count).dName, size,1);
                    break;
                }
                case 0x55:
                {
                    newVehicle = new VehicleMine(COLLECTOR, vehiclelist.at(count).dName, size,1);
                    break;

                }
                default:
                {
                    newVehicle = new VehicleMine(MINEUNLOADER, vehiclelist.at(count).dName, size,1);
                    break;
                }
            }

            memmove(newVehicle->Imei, vehiclelist.at(count).dimei, 15);
            vehicleIdItemMap.insert(vehiclelist.at(count), newVehicle);
            scene->addItem(newVehicle);
            scene->addItem(newVehicle->typeItem);
            scene->addItem(newVehicle->nameItem);
        }

        //对所有车辆进行位置摆放
        if(lngcenter < 0.0000001)
        {
            lngcenter = vehiclelist.at(count).dVehicleLng;
            latcenter = vehiclelist.at(count).dVehicleLat;
        }

        vehicleIdItemMap.value(vehiclelist.at(count))->setPos(vehiclelist.at(count).dVehicleLng*equator - lngcenter*equator,\
                                                              latcenter*equator - vehiclelist.at(count).dVehicleLat*equator);
        vehicleIdItemMap.value(vehiclelist.at(count))->typeItem->setRotation(vehiclelist.at(count).dVehicleHeading);
    }

    sceneMutex->unlock();
}

void HmiMap::updateGpsState(char flag)
{
    emit changeGpsStateSignal(flag);
}

void HmiMap::followSelf()
{
    if(selfVehicle != NULL)
    {
        view->centerOn(selfVehicle);
    }
}

void HmiMap::followAll()
{
    qreal width = view->size().width() - 2;
    qreal height = view->size().height() - 2;
    qreal scenewidth = scene->sceneRect().width();
    qreal sceneheight = scene->sceneRect().height();

    view->setTransform(QTransform(1, 0, 0, 1, 0, 0));
    qreal widthscale = width / scenewidth;
    qreal heightscale = height/sceneheight;

    if(widthscale <= heightscale)
        view->setMatrix(QMatrix(widthscale, 0, 0, widthscale, 0, 0));
    else
        view->setMatrix(QMatrix(heightscale, 0, 0, heightscale, 0, 0));
}

void HmiMap::updateVehiclePos(latlngheading pos)
{
    if((lngcenter < 0.0000001) && (latcenter < 0.0000001))
    {
        lngcenter = pos.lng;
        latcenter = pos.lat;
    }

    if((selfVehicle != NULL) && (lngcenter > 0.0000001) && (latcenter > 0.0000001))
    {

        selfVehicle->setPos(pos.lng*equator - lngcenter*equator - selfVehicle->boundingRect().width()/2,\
                            latcenter*equator - pos.lat*equator - selfVehicle->boundingRect().height()/2);

        selfVehicle->typeItem->setRotation(pos.heading);
    }
}
void HmiMap::addStopPoint(StopPoints *sp)
{
    if(sp != NULL)
    {
        //添加stoplist
        if(lngcenter < 0.0000001)
        {
            lngcenter = sp->getLeftTopPoint().x();
            latcenter = sp->getLeftTopPoint().y();
        }

        scene->addItem(sp);
        sp->setPos(sp->getLeftTopPoint().x()*equator - lngcenter*equator,\
                     latcenter*equator - sp->getLeftTopPoint().y()*equator);

        scene->setSceneRect(scene->itemsBoundingRect());
    }

    QApplication::processEvents();
}
void HmiMap::removeStopPoint(StopPoints *sp)
{
    if(sp != NULL)
    {
        //添加stoplist
        scene->removeItem(sp);
        sp=NULL;
    }

}

void HmiMap::addDbMap()
{
    boundaryItemMap = datathread->getBoundaryMap();
    roadItemMap = datathread->getRoadMap();

    sceneMutex->lock();

    QMap<QString, PathBoundary*>::iterator boundaryitor;
    for(boundaryitor = boundaryItemMap.begin(); boundaryitor != boundaryItemMap.end(); ++boundaryitor)
    {
        if(lngcenter < 0.0000001)
        {
            if((boundaryitor.value()->getLeftTopPoint().x() > 0.0000001) && (boundaryitor.value()->getLeftTopPoint().x() < 360.0000000))
            {
                lngcenter = boundaryitor.value()->getLeftTopPoint().x();
                latcenter = boundaryitor.value()->getLeftTopPoint().y();
            }
        }

        scene->addItem(boundaryitor.value());
        boundaryitor.value()->setPos(boundaryitor.value()->getLeftTopPoint().x()*equator - lngcenter*equator,\
                     latcenter*equator - boundaryitor.value()->getLeftTopPoint().y()*equator);
    }

    QMap<QString, PathRoad*>::iterator roaditor;
    for(roaditor = roadItemMap.begin(); roaditor != roadItemMap.end(); ++roaditor)
    {
        if(lngcenter < 0.0000001)
        {
            if((boundaryitor.value()->getLeftTopPoint().x() > 0.0000001) && (boundaryitor.value()->getLeftTopPoint().x() < 360.0000000))
            {
                lngcenter = roaditor.value()->getLeftTopPoint().x();
                latcenter = roaditor.value()->getLeftTopPoint().y();
            }
        }

        scene->addItem(roaditor.value());
        roaditor.value()->setPos(roaditor.value()->getLeftTopPoint().x()*equator - lngcenter*equator,\
                     latcenter*equator - roaditor.value()->getLeftTopPoint().y()*equator);

    }

    scene->setSceneRect(scene->itemsBoundingRect());

    sceneMutex->unlock();

    scene->update(scene->itemsBoundingRect());

    QApplication::processEvents();
}

void HmiMap::addCollection(struct collectlatlngheading *pos)
{
    if(realTimeItem == NULL)
    {
        realTimeItem = new PathRealTime(pos);
    }

    if((lngcenter < 0.0000001) && (latcenter < 0.0000001))
    {
        lngcenter = pos->lng;
        latcenter = pos->lat;
    }

    if((realTimeItem != NULL) && (lngcenter > 0.0000001) && (latcenter > 0.0000001))
    {
        //realTimeItem->list.clear();
        scene->addItem(realTimeItem);
        realTimeItem->setPos(pos->lng*equator - lngcenter*equator,\
                             latcenter*equator - pos->lat*equator);
    }
}

void HmiMap::addCollectionPoint(struct collectlatlngheading *pos)
{
    if(realTimeItem != NULL)
    {
        realTimeItem->list.append(pos);
    }
}

void HmiMap::removeCollection()
{
    if(realTimeItem != NULL)
    {
        //realTimeItem->list.clear();
        scene->removeItem(realTimeItem);
        //delete realTimeItem;
        realTimeItem = NULL;
    }
}

void HmiMap::updateCollection()
{
    realTimeItem->updateList(datathread->rtcCollectList);
    scene->setSceneRect(scene->itemsBoundingRect());
    QApplication::processEvents();
}

void HmiMap::initView(QMatrix matrix)
{

    view->setMatrix(matrix);
}

void HmiMap::calcFinishedTask(latlngheading pos)
{
    PathTask* taskItem=datathread->getTaskList();
    if(taskItem!=NULL)
        taskItem->calcFinishedTask(pos);
}

void HmiMap::addLpxCollection()
{
    datathread->getCollectionList();
}

void HmiMap::addLpxTask()
{
    taskItemGroup = datathread->getTaskList();
    sceneMutex->lock();
    if(lngcenter < 0.0000001)
    {
        lngcenter = taskItemGroup->getLeftTopPoint().x();
        latcenter = taskItemGroup->getLeftTopPoint().y();
    }

    scene->addItem(taskItemGroup);
    taskItemGroup->setPos(taskItemGroup->getLeftTopPoint().x()*equator - lngcenter*equator,\
                 latcenter*equator - taskItemGroup->getLeftTopPoint().y()*equator);

    scene->setSceneRect(scene->itemsBoundingRect());
    sceneMutex->unlock();

    QApplication::processEvents();

}

void HmiMap::addLpxPermission()
{

    permissionItemGroup = datathread->getPermissionList();
    sceneMutex->lock();
    if(lngcenter < 0.0000001)
    {
        lngcenter = permissionItemGroup->getLeftTopPoint().x();
        latcenter = permissionItemGroup->getLeftTopPoint().y();
        //qDebug()<<"lng,lat"<<lngcenter<<latcenter;
    }

    scene->addItem(permissionItemGroup);
    permissionItemGroup->setPos(permissionItemGroup->getLeftTopPoint().x()*equator - lngcenter*equator,\
                 latcenter*equator - permissionItemGroup->getLeftTopPoint().y()*equator);

    scene->setSceneRect(scene->itemsBoundingRect());
    sceneMutex->unlock();
    scene->update();

    QApplication::processEvents();

}
