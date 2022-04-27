#include "stoplistdeal.h"

StopListDeal* StopListDeal::handle = NULL;
QMutex* StopListDeal::handleMutex = new QMutex;

StopListDeal::StopListDeal()
{
    stopList.clear();
}

void StopListDeal::clearStopList()
{
    stopList.clear();
}

void StopListDeal::generateStopList(QString lpxpath, double sidefixvalue, double vehiclewidth, double vehiclelength)
{
    QFile lastboundaryfile;
    lastboundaryfile.setFileName(lpxpath);
    QList<struct latlngheading* > boundarypointlist;

    if(lastboundaryfile.open(QIODevice::ReadOnly))
    {
        boundarypointlist.clear();
//读取原数据文件
        while(!lastboundaryfile.atEnd())
        {
            struct latlngheading *point;
            QString content = lastboundaryfile.readLine();
            QByteArray contentarr = content.mid(1, content.indexOf("*") - 1).toLatin1();
            char result = xorCheck(contentarr.data(), contentarr.size());

            QString resultStr = QString::number(result, 16);
            if(resultStr == content.mid(content.indexOf("*") + 1, 2))
            {
                QStringList contentlist = content.split(",");
//                qDebug()<<contentlist;
                point = new struct latlngheading;
                point->heading = contentlist.at(1).toFloat();
                point->lat = contentlist.at(2).toDouble();
                point->lng = contentlist.at(3).toDouble();
//                qDebug("%.2f\t%.7f\t%.7f",point->heading, point->lat, point->lng);

                boundarypointlist.push_back(point);
            }
            else
            {
                qDebug()<<content;
            }
        }
        lastboundaryfile.close();

//生成停靠点
        stopList.clear();
        if(!boundarypointlist.isEmpty())
            stopList = computerStoplist(boundarypointlist, sidefixvalue, vehiclewidth, vehiclelength);
//        for(int count = 0; count < stopList.size(); count++)
//        {
//            qDebug("%.2f\t%.7f\t%.7f",stopList.at(count)->heading, stopList.at(count)->lat, stopList.at(count)->lng);
//        }

//        qDebug("aaaa");
        //generateLpxFile("/storage/boundary/stoplist.lpxc",stopList);
    }
}
