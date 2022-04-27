#ifndef MAPDATATHREAD_H
#define MAPDATATHREAD_H

#include <QObject>

#include <QList>
#include <QMap>
#include <QPointF>
#include <QString>
#include <QStringList>

#include <QFile>

#include <QMutex>
#include <QThread>

#include "pub.h"
#include "HmiFunction/log.h"

#include "sqlite3.h"

#include "pathboundary.h"
#include "pathroad.h"
#include "pathtask.h"
#include "pathpermission.h"


class MapDataThread : public QObject
{
    Q_OBJECT
public:
    static MapDataThread* getDataThread()
    {
        if(handle == NULL)
        {
            handleMutex->lock();
            if(handle == NULL)
            {
                handle = new MapDataThread;
            }
            handleMutex->unlock();
        }
        return handle;
    }
    static void freeDataThread()
    {
        if(handle != NULL)
        {
            delete handle;
            handle = NULL;
        }
    }

    void getPosInfoFromLpxCollection(QString path);
    void getPosInfoFromDB(QString path);
    void getPosInfoFromLpxTask(QStringList pathlist);
    void getPosInfoFromLpxPermission(QStringList pathList,quint32 endPointNum);

    void clearRoadMap();
    void clearBoundaryMap();
    void clearCollectionList();
    void clearTaskList();

    void clearRtcCollectionList();
    void addCollectorPoint(struct collectlatlngheading*);
    void addApendCollectorPoint(struct collectlatlngheading&);
    QList<struct collectlatlngheading*>* getRtcCollectList();

    QMap<QString, PathRoad* > getRoadMap();
    QMap<QString, PathBoundary* > getBoundaryMap();
    QList<struct latlngheading*> getCollectionList();
    PathTask* getTaskList();
    PathPermission* getPermissionList();

    QMap<QString, QList<struct latlngheading*>* > dbroadMap;
    QMap<QString, QList<struct latlngheading*>* > dbboundaryMap;
    QList<struct latlngheading*> collectList;//最新边界显示列表
    mutable QList<struct collectlatlngheading*> rtcCollectList;//实时采集信息列表

    QList<struct latlngheading*> taskList;
    QList<struct latlngheading*> permissionList;

    QMap<QString, PathBoundary*> boundaryItemMap;//边界号及边界图元组对应表
    QMap<QString, PathRoad*> roadItemMap;//道路号及道路图元组对应表
    PathTask* taskItem = NULL;
    PathPermission* permissionItem = NULL;

private:
    static MapDataThread* handle;
    static QMutex* handleMutex;

signals:
    void lpxCollAnalysisComplete();
    void lpxTaskAnalysisComplete();
    void lpxPermissionAnalysisComplete();
    void dbAnalysisComplete();

};


#endif // DATATHREAD_H
