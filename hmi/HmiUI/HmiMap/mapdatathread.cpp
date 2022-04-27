#include "mapdatathread.h"

MapDataThread* MapDataThread::handle = NULL;
QMutex* MapDataThread::handleMutex = new QMutex;

//数据库道路提取回调函数
int sqlroadcallback(void * para, int n_column, char ** column_value, char ** column_name)
{
    Q_UNUSED(n_column)
    Q_UNUSED(column_name)

    MapDataThread* parent = static_cast<MapDataThread*>(para);

    QString trajectory = column_value[0];
    QString elementname = column_value[1];
    QStringList tralist = trajectory.split(";");

    QList<struct latlngheading*> *pointlist = new QList<struct latlngheading*>;
    pointlist->clear();

    for(int count = 1; count < tralist.size(); count++)
    {
        double latcur, loncur;
        float headcur;

        QString posnodeStr = tralist.at(count);
        latcur = posnodeStr.split(",").at(1).toDouble();
        loncur = posnodeStr.split(",").at(0).toDouble();
        headcur = posnodeStr.split(",").at(3).toFloat();

        struct latlngheading* latlonpos = new struct latlngheading;
        latlonpos->lng = loncur;
        latlonpos->lat = latcur;
        latlonpos->heading = headcur;

        pointlist->append(latlonpos);
    }

    parent->dbroadMap.insert(elementname, pointlist);


    return SQLITE_OK;
}

//数据库边界提取回调函数
int sqlboundarycallback(void * para, int n_column, char ** column_value, char ** column_name)
{
    Q_UNUSED(n_column)
    Q_UNUSED(column_name)

    MapDataThread* parent = static_cast<MapDataThread*>(para);

    QString trajectory = column_value[0];
    QString elementname = column_value[1];
    QStringList tralist = trajectory.split(";");

    QList<struct latlngheading*>* pointlist = new QList<struct latlngheading*>;
    pointlist->clear();

    for(int count = 1; count < tralist.size(); count++)
    {
        double latcur, loncur;

        QString posnodeStr = tralist.at(count);
        latcur = posnodeStr.split(",").at(1).toDouble();
        loncur = posnodeStr.split(",").at(0).toDouble();

        struct latlngheading* latlonpos = new struct latlngheading;
        latlonpos->lng = loncur;
        latlonpos->lat = latcur;
        latlonpos->heading = 0;

        pointlist->append(latlonpos);
    }

    parent->dbboundaryMap.insert(elementname, pointlist);

    return SQLITE_OK;
}

//解析边界信息文件
void MapDataThread::getPosInfoFromLpxCollection(QString path)
{
    QFile lastboundaryfile;
    lastboundaryfile.setFileName(path);

    if(lastboundaryfile.open(QIODevice::ReadOnly))
    {
        collectList.clear();
//读取原数据文件
        while(!lastboundaryfile.atEnd())
        {
            QString content = lastboundaryfile.readLine();
            QByteArray contentarr = content.mid(1, content.indexOf("*") - 1).toLatin1();
            char result = xorCheck(contentarr.data(), contentarr.size());

            QString resultStr = QString::number(result, 16).toUpper();
            if(resultStr == content.mid(content.indexOf("*") + 1, 2))
            {
                QStringList contentlist = content.split(",");
                struct latlngheading *point = new struct latlngheading;
                point->heading = contentlist.at(1).toFloat();
                point->lat = contentlist.at(2).toDouble();
                point->lng = contentlist.at(3).toDouble();

                collectList.append(point);
            }
        }
        lastboundaryfile.close();
    }

    emit lpxCollAnalysisComplete();
}

//解析任务信息文件
void MapDataThread::getPosInfoFromLpxTask(QStringList pathList)
{
    if(taskItem != NULL)
    {
        delete taskItem;
        taskItem = NULL;
    }

    taskList.clear();

    for(int count = 0; count < pathList.size(); count++)
    {
        QString path = pathList.at(count);
        QFile lastboundaryfile;
        lastboundaryfile.setFileName(path);

        if(lastboundaryfile.open(QIODevice::ReadOnly))
        {
    //读取原数据文件
            while(!lastboundaryfile.atEnd())
            {
                QString content = lastboundaryfile.readLine();
                QByteArray contentarr = content.mid(1, content.indexOf("*") - 1).toLatin1();
                char result = xorCheck(contentarr.data(), contentarr.size());

                QString resultStr = QString::number(result, 16).toUpper();
                if(resultStr == content.mid(content.indexOf("*") + 1, 2))
                {
                    QStringList contentlist = content.split(",");
                    struct latlngheading *point = new struct latlngheading;
                    point->heading = contentlist.at(2).toFloat();
                    point->lat = contentlist.at(3).toDouble();
                    point->lng = contentlist.at(4).toDouble();
                    point->flag &= ~1;
                    taskList.append(point);
                }
            }
            lastboundaryfile.close();
        }
    }

    taskItem = new PathTask(&taskList);

    emit lpxTaskAnalysisComplete();
}
//解析许可文件
void MapDataThread::getPosInfoFromLpxPermission(QStringList pathList,quint32 endPointNum)
{
    if(permissionItem!=NULL)
    {
        delete permissionItem;
        permissionItem=NULL;
    }
    permissionList.clear();
    uint32_t line=endPointNum;

    for(int count = 0; count < pathList.size(); count++)//只支持size()==1的情况
    {
        QString path = pathList.at(count);
        QFile permissionFile;
        permissionFile.setFileName(path);

        if(permissionFile.open(QIODevice::ReadOnly))
        {
            //读取原数据文件,找到那个停止点
            while(!permissionFile.atEnd())
            {
                QString content = permissionFile.readLine();
                QByteArray contentarr = content.mid(1, content.indexOf("*") - 1).toLatin1();
                char result = xorCheck(contentarr.data(), contentarr.size());

                QString resultStr = QString::number(result, 16).toUpper();
                if(resultStr == content.mid(content.indexOf("*") + 1, 2))
                {
                    if(line--)//找到第500个点作为终止点
                        continue;
                    else
                    {
                        QStringList contentlist = content.split(",");
                        struct latlngheading *point = new struct latlngheading;
                        point->heading = contentlist.at(2).toFloat();
                        point->lat = contentlist.at(3).toDouble();
                        point->lng = contentlist.at(4).toDouble();
                        permissionList.append(point);
                        line=0;
                        break;
                     }
                }
            }
            permissionFile.close();
        }
    }
    if(line==0)
    {
        permissionItem = new PathPermission(&permissionList);
        emit lpxPermissionAnalysisComplete();
    }
}
//解析数据道路、边界信息文件
void MapDataThread::getPosInfoFromDB(QString path)
{
    sqlite3 *db;
    int ret;
    char *errmsg;

    ret = sqlite3_open(path.toLatin1().data(), &db);
    if(ret != SQLITE_OK)
    {
        LOG_DEBUG("Warning, No map information exists");
        return ;
    }
    else
    {
        char roadcommand[100] = "select trajectory, laneid from HDMAP_LANES order by laneid;";
        char boundarycommand[100] = "select boundary, elementname from HDMAP_ROADELEMENTS order by elementid;";
        sqlite3_exec(db, roadcommand, sqlroadcallback, this, &errmsg);
        LOG_DEBUG(QByteArray(errmsg, sizeof(errmsg)));
        sqlite3_exec(db, boundarycommand, sqlboundarycallback, this, &errmsg);
        LOG_DEBUG(QByteArray(errmsg, sizeof(errmsg)));
    }

    sqlite3_close(db);

    boundaryItemMap.clear();
    roadItemMap.clear();

    QMap<QString, QList<struct latlngheading*>* >::iterator boundaryIt;
    for(boundaryIt = dbboundaryMap.begin(); boundaryIt != dbboundaryMap.end(); ++boundaryIt)
    {
        PathBoundary* boundary = new PathBoundary(boundaryIt.value());
        boundaryItemMap.insert(boundaryIt.key(), boundary);
    }

    QMap<QString, QList<struct latlngheading*>* >::iterator roadIt;
    for(roadIt = dbroadMap.begin(); roadIt != dbroadMap.end(); ++roadIt)
    {
        PathRoad* road = new PathRoad(roadIt.value());
        roadItemMap.insert(roadIt.key(), road);
    }

    emit dbAnalysisComplete();
}

//清除道路信息映射表
void MapDataThread::clearRoadMap()
{
    dbroadMap.clear();
}

//清除边界信息映射表
void MapDataThread::clearBoundaryMap()
{
    dbboundaryMap.clear();
}

//清除采集信息列表
void MapDataThread::clearCollectionList()
{
    collectList.clear();
}

//清除任务信息列表
void MapDataThread::clearTaskList()
{
    taskList.clear();
}
void MapDataThread::clearRtcCollectionList()
{
    rtcCollectList.clear();
}

void MapDataThread::addCollectorPoint(struct collectlatlngheading* pos)
{
//    if(rtcCollectList.size() < 2)
//    {
//        rtcCollectList.append(pos);
//    }
//    else if(((rtcCollectList.last()->lat - pos->lat) > 0.000001) || ((rtcCollectList.last()->lat - pos->lat) > 0.000001))
//    {
        rtcCollectList.append(pos);
//    }
}

void MapDataThread::addApendCollectorPoint(struct collectlatlngheading& pos)
{
    addCollectPosDataFun(&rtcCollectList);
}

QList<collectlatlngheading*>* MapDataThread::getRtcCollectList()
{
    return &rtcCollectList;
}
//获取道路信息映射表
QMap<QString, PathRoad* > MapDataThread::getRoadMap()
{
    return roadItemMap;
}

//获取边界信息映射表
QMap<QString, PathBoundary* > MapDataThread::getBoundaryMap()
{
    return boundaryItemMap;
}

//获取采集信息列表
QList<struct latlngheading*> MapDataThread::getCollectionList()
{
    return collectList;
}

//获取任务信息列表
PathTask* MapDataThread::getTaskList()
{
    return taskItem;
}

//获取任务信息列表
PathPermission* MapDataThread::getPermissionList()
{
    return permissionItem;
}

