#include "pub.h"

const double RADIO = 6371393;

//求弧度
double radian(double d)
{
    return d * M_PI / 180.0;   //角度1˚ = π / 180
}

void computerLonLat(double lon, double lat, double brng, double dist, double *eLon, double *eLat)
{
    double a = 6378137;
    /** 短半径b=6356752.3142 */
    double b = 6356752.3142;
    /** 扁率f=1/298.2572236 */
    double f = 1 / 298.2572236;

    double alpha1 = radian(brng);
    double sinAlpha1 = sin(alpha1);
    double cosAlpha1 = cos(alpha1);

    double tanU1 = (1 - f) * tan(radian(lat));
    double cosU1 = 1 / sqrt((1 + tanU1 * tanU1));
    double sinU1 = tanU1 * cosU1;

    double sigma1 = atan2(tanU1, cosAlpha1);
    double sinAlpha = cosU1 * sinAlpha1;
    double cosSqAlpha = 1 - sinAlpha * sinAlpha;
    double uSq = cosSqAlpha * (a * a - b * b) / (b * b);
    double A = 1 + uSq / 16384 * (4096 + uSq * (-768 + uSq * (320 - 175 * uSq)));
    double B = uSq / 1024 * (256 + uSq * (-128 + uSq * (74 - 47 * uSq)));
    double cos2SigmaM=0;
    double sinSigma=0;
    double cosSigma=0;
    double sigma = dist / (b * A);
    double sigmaP = 2 * M_PI;

    while (abs(sigma - sigmaP) > 1e-12)
    {
        cos2SigmaM = cos(2 * sigma1 + sigma);
        sinSigma = sin(sigma);
        cosSigma = cos(sigma);
        double deltaSigma = B*sinSigma*(cos2SigmaM+B/4*(cosSigma*(-1+2*cos2SigmaM*cos2SigmaM)-B/6*cos2SigmaM*(-3+4*sinSigma*sinSigma)*(-3+4*cos2SigmaM*cos2SigmaM)));
        sigmaP = sigma;
        sigma = dist / (b * A) + deltaSigma;
    }

    double tmp = sinU1 * sinSigma - cosU1 * cosSigma * cosAlpha1;
    double lat2 = atan2(sinU1 * cosSigma + cosU1 * sinSigma * cosAlpha1,(1 - f) * sqrt(sinAlpha * sinAlpha + tmp * tmp));
    double lambda = atan2(sinSigma * sinAlpha1, cosU1 * cosSigma - sinU1 * sinSigma * cosAlpha1);
    double C = f / 16 * cosSqAlpha * (4 + f * (4 - 3 * cosSqAlpha));
    double L = lambda - (1 - C) * f * sinAlpha* (sigma + C * sinSigma * (cos2SigmaM + C * cosSigma * (-1 + 2 * cos2SigmaM * cos2SigmaM)));

    *eLon = L*180/M_PI + lon;
    *eLat = lat2*180/M_PI;
}

void calcLonLat(double lng, double lat, double angle, double dist, double *eLng, double *eLat)
{
    double B1=lat,L1=lng,A1=angle,S=dist;
    double a=6378245.0000;double b=6356863.018773;double c=6399698.901783;
    double e=0.081813334016931;//第一偏心率
    double W1;
    double r,t,p,s,k,m,n;
    double A,B,C,u,v;
    double q0,q;//σ0,σ
    double theta;double l;
    double B2,L2,A2;

    B1=B1*M_PI/180.0;
    L1=L1*M_PI/180.0;
    A1=A1*M_PI/180.0;

    // 计算起点的归化纬度
    W1=sqrt(1-e*e*(sin(B1)*sin(B1)));
    t=sin(B1)*sqrt(1-e*e)/W1;//sin(u1)
    s=cos(B1)/W1;//cos(u1)

    //计算辅助函数值
    r=s*sin(A1);//sin(A0)
    k=s*cos(A1)/t;//cot(σ1)
    m=2*k/(k*k+1);//sin(2*σ1)
    n=(k*k-1)/(k*k+1);//cos(2*σ1)

    //计算系数A，B，C及u,v
    A=6356863.020+(10708.949-13.474*(1-r*r))*(1-r*r);
    B=(5354.469-8.978*(1-r*r))*(1-r*r);
    C=(2.238*(1-r*r))*(1-r*r)+0.006;
    u=691.46768-(0.58143-0.00144*(1-r*r))*(1-r*r);
    v=(0.2907-0.0010*(1-r*r))*(1-r*r);

    //计算球面长度
    q0=(S-(B+C*n)*m)/A;
    q=q0+(B+5*C*(n*cos(2*q0)-m*sin(2*q0)))*(m*cos(2*q0)+n*sin(2*q0))/A;

    //计算经差改正数
    theta=(u*q+v*((m*cos(2*q0)+n*sin(2*q0))-m))*r;

    //计算终点大地坐标及大地方位角
    p=t*cos(q)+s*cos(A1)*sin(q);
    B2=atan(p/(sqrt(1-e*e)*sqrt(1-p*p)));
    B2=B2*180.0/M_PI;
    //printf("B2=%f\n",B2);
    *eLat = B2;
    l=atan(sin(A1)*sin(q)/(s*cos(q)-t*sin(q)*cos(A1)));
    if(sin(A1)>0)
        if(tan(l)>0)
            l=fabs(l);
        else
            l=180.0-fabs(l);
    else
        if(tan(l)>0)
            l=fabs(l)-180.0;
        else
            l=-fabs(l);

    L2=L1+l-theta/3600/180*M_PI;
    L2=L2*180.0/M_PI;
    //printf("L2=%f\n",L2);//大地经度
    *eLng = L2;

    A2=atan(s*sin(A1)/(s*cos(q)*cos(A1)-t*sin(q)));
    if(sin(A1)>0)
        if(tan(A2)>0)
            A2=180.0+fabs(A2);
        else
            A2=360.0-fabs(A2);
    else
        if(tan(A2)>0)
            A2=fabs(A2);
        else
            A2=180-fabs(A2);
    //printf("A2=%f\n",A2);//大地方位角
}
double offSetDiffAngle(float loadInAngle, double vehicleWidth, double vehicleLength)
{
    double offsetHeading = loadInAngle + 90.0;
    double yoffset = 0.0;
    double tmp;

    vehicleWidth = fabs(vehicleWidth);
    vehicleLength = fabs(vehicleLength);
    if (vehicleLength < vehicleWidth)
    {
        tmp = vehicleLength;
        vehicleLength = vehicleWidth;
        vehicleWidth = tmp;
    }
    if ((vehicleWidth < 0.11) || (vehicleLength < 0.11))
    {
        return yoffset;
    }


    double w = vehicleWidth/2.0;
    double l = vehicleLength/2.0;
    double theta1 = atan(l/w)*180/M_PI;
    double theta2 = (atan(w/l)*180/M_PI) + 90.0;

    if(offsetHeading <= theta1)
    {
        yoffset = w/cos((offsetHeading/180.0)*M_PI);
    }
    else if(offsetHeading <= theta2)
    {
        yoffset = l/sin((offsetHeading/180.0)*M_PI);
    }
    else
    {
        yoffset = -(w/cos((offsetHeading/180.0)*M_PI));
    }
    yoffset -= w;

    return yoffset;
}
double haversine(double lon1, double lat1, double lon2, double lat2)
{
    double distlon = radian(lon2 - lon1);
    double distlat = radian(lat2 - lat1);

    double a = (pow(sin(distlat / 2), 2) + cos(radian(lat1))) * cos(radian(lat2)) * pow(sin( distlon / 2), 2);
    double angle = 2 * asin(sqrt(a));

    return angle * RADIO;
}

double getDistance(double lon1, double lat1, double lon2, double lat2)
{
    double disn = 0,_x,_y;
    _x = qAbs(lon1 - lon2)*111700*cos(lat1/180*M_PI);
    _y = qAbs(lat1 - lat2)*111700;

    disn = sqrt(pow(_x, 2) + pow(_y, 2));
    return disn;
}

double getHeading(double lonStart, double latStart, double lonEnd, double latEnd)
{
    double delta_fy = latEnd-latStart;
    double delta_lnmg=lonEnd-lonStart;
    //经度差应小于180度
    if(delta_lnmg < -180.0)
        delta_lnmg += 360.0;
    if(delta_lnmg > 180.0)
        delta_lnmg -= 360.0;
    //delta_lnmg > 0.0 从 西 ---> 东  delta_lnmg < 0.0 从 东 ---> 西
    bool bGoEast=false,bGoNorth=false;
    if(delta_lnmg >= 0.0)
        bGoEast=true;
    else
        bGoEast=false;
    //delta_fy > 0.0 从 南 ---> 北  delta_fy < 0.0 从 北 ---> 南
    if(delta_fy>=0.0)
        bGoNorth=true;
    else
        bGoNorth=false;

    if(delta_fy==0)
    {
        if(delta_lnmg==0)return 0;
        return bGoEast?90:270;
    }
    double d1=7915.7045*(0.081813369f/2*log10((1-0.081813369f*sin(latStart*M_PI/180))
        /(1+0.081813369f*sin(latStart*M_PI/180)))
        +log10(tan((45+latStart/2)*M_PI/180.0)));//纬度渐长率

    double d2=7915.7045*(0.081813369f/2*log10((1-0.081813369f*sin(latEnd*M_PI/180))/
        (1+0.081813369f*sin(latEnd*M_PI/180)))
        +log10(tan((45+latEnd/2)*M_PI/180.0)));//纬度渐长率

    double delta_d=d2-d1;////纬度渐长率差(分)

    double dbDir=atan(delta_lnmg*60/delta_d)*180/M_PI;

    if(!bGoEast&&bGoNorth)
        dbDir=360+dbDir;

    if(!bGoEast&&!bGoNorth)
        dbDir=180+dbDir;

    if(bGoEast&&!bGoNorth)
        dbDir=180+dbDir;

    return (dbDir);
}



char xorCheck(char *buf, int len)
{
    char result;
    if(len > 2)
    {
        result = buf[0];
        for(int count = 1; count < len; count++)
        {
            result ^= buf[count];
        }

        return result;
    }

    return NULL;
}

bool boundaryDataFixFun(QList<collectlatlngheading *> *list, double fixdirect, double fixvalue)
{
    if(fixdirect<0)fixdirect+=360;
    for(int count = 0; count < list->size(); count++)
    {
        latlngheading fixvaluetmp;
        computerLonLat(list->at(count)->lng, list->at(count)->lat, (list->at(count)->heading + fixdirect), -fixvalue, &(fixvaluetmp.lng), &(fixvaluetmp.lat));
        list->at(count)->lng = fixvaluetmp.lng;
        list->at(count)->lat = fixvaluetmp.lat;
    }

    return true;
}

bool boundaryDataFun(QList<latlngheading*> *list)
{
    if(list->size() > 2)
    {
        QList<latlngheading*> resultlist;
        resultlist.append(list->first());
        for(int count = 1; count < list->size(); count++)
        {
#if 0
            double distance = getDistance(resultlist.last()->lon, resultlist.last()->lat, list->at(count)->lon, list->at(count)->lat);
            if(distance > 0.2)
            {
                struct latlngheading *point = new struct latlngheading;
                double heading = getHeading(resultlist.last()->lon, resultlist.last()->lat, list->at(count)->lon, list->at(count)->lat);
                point->heading = heading;
                computerLonLat(resultlist.last()->lon, resultlist.last()->lat, heading, 0.2, &(point->lon), &(point->lat));
                resultlist.append(point);
                qDebug()<<append(point);
                distance = getDistance(resultlist.last()->lon, resultlist.last()->lat, list->at(count)->lon, list->at(count)->lat);
                continue;
            }
#endif
            while(1)
            {
                double elon, elat;
                double dist = getDistance(resultlist.last()->lng, resultlist.last()->lat, list->at(count)->lng, list->at(count)->lat);

                if(dist < 0.2)
                {
                    break;
                }

                double heading = getHeading(resultlist.last()->lng, resultlist.last()->lat, list->at(count)->lng, list->at(count)->lat);

                computerLonLat(resultlist.last()->lng,resultlist.last()->lat, heading, 0.2, &elon, &elat);
                struct latlngheading *point = new struct latlngheading;
                point->lat = elat;
                point->lng = elon;
                point->heading = heading;
                resultlist.append(point);
            }
        }

        QFile saveFile(QString(ParaParsing::instance()->paraSt.pub.mapPath+"/%1.lpx").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd.hh.mm.ss")));
        if(saveFile.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QString lastStr;
            lastStr.clear();
            //qDebug()<<"resultlist size:"<<resultlist.size();
            for(int count = 0; count < resultlist.size(); count++)
            {
                QString curStr = QString("$,%1,%2,%3,,,,,,,,,").arg(QString::number(resultlist.at(count)->heading, 'f', 2))\
                                                      .arg(QString::number(resultlist.at(count)->lat, 'f', 7))\
                                                      .arg(QString::number(resultlist.at(count)->lng, 'f', 7));
                char xorsum = xorCheck(curStr.right(curStr.length() - 1).toLatin1().data(), curStr.right(curStr.length() - 1).toLatin1().size());
                curStr.append("*" + QString::number(xorsum, 16) +"\r\n");
                //qDebug()<<curStr;
                saveFile.write(curStr.toLatin1());
            }

            saveFile.close();
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

bool addPosDataFun(QList<struct latlngheading *> *list, struct latlngheading& pos)
{
    if(list->size() == 0)
    {
        struct latlngheading* data = new struct latlngheading;
        memmove((char*)data, (char*)&pos, sizeof(struct latlngheading));
        list->append(data);
    }
    else
    {
        struct latlngheading* data = new struct latlngheading;
        memmove((char*)data, (char*)&pos, sizeof(struct latlngheading));
        while(1)
        {
            double elon, elat;
            double dist = getDistance(list->last()->lng, list->last()->lat, data->lng, data->lat);

            if(dist < 0.2)
            {
                break;
            }

            float heading = getHeading(list->last()->lng, list->last()->lat, data->lng, data->lat);

            computerLonLat(list->last()->lng,list->last()->lat, heading, 0.2, &elon, &elat);
            if(!((elon > 0.0) && (elat < 360.0)))
            {
                break;
            }
            struct latlngheading *point = new struct latlngheading;
            point->lat = elat;
            point->lng = elon;
            point->heading = heading;
            list->append(point);
        }
    }
    return true;
}
void addCollectPosDataFun(QList<struct collectlatlngheading *> *list)
{
    auto itor = list->begin();
    while((itor+1) != list->end())
    {
        struct collectlatlngheading* front = *itor;
        struct collectlatlngheading* back = *(itor + 1);
        if(!(front->flag && back->flag))
        {
            double elon, elat;
            double dist = getDistance(front->lng, front->lat, back->lng, back->lat);

            if(dist < 0.2)
            {
                itor++;
                continue;
            }

            float heading = getHeading(front->lng, front->lat, back->lng, back->lat);

            computerLonLat(front->lng, front->lat, heading, 0.2, &elon, &elat);
            if(!((elon > 0.0) && (elat < 360.0)))
            {
                break;
            }

            struct collectlatlngheading *point = new struct collectlatlngheading;

            point->flag = false;
            point->lat = elat;
            point->lng = elon;
            point->heading = heading;
            point->height = front->height;
            itor = list->insert(itor+1, point);
        }
        else
        {
            itor++;
        }
        usleep(1);
    }
}

QList<latlngheading *> computerStoplist(QList<latlngheading* > list , double headfix, double vehiclewidth, double vehiclelength)
{
    QList<latlngheading *> stoplist;
    latlngheading* nodestart = NULL;
    latlngheading* nodeend = NULL;

    for(int count = 0; count < list.size(); count++)
    {
        if(nodestart == NULL)//获取首个起始点
        {
            nodestart = new latlngheading;
            memmove(nodestart, list.at(count),sizeof(latlngheading));
        }

        if((nodestart != NULL) && (nodeend == NULL))//计算终止点
        {
            if(getDistance(nodestart->lng, nodestart->lat, list.at(count)->lng, list.at(count)->lat) >= vehiclewidth)
            {
                struct latlngheading nodetmp;
                double heading = getHeading(nodestart->lng, nodestart->lat, list.at(count)->lng, list.at(count)->lat);
                computerLonLat(nodestart->lng, nodestart->lat, heading, vehiclewidth, &(nodetmp.lng), &(nodetmp.lat));
                nodeend = new struct latlngheading;
                memmove(nodeend, &nodetmp, sizeof(struct latlngheading));
            }
        }

        if((nodestart != NULL) && (nodeend != NULL))//计算中间点
        {
            struct latlngheading *nodestop = new struct latlngheading;
            struct latlngheading *nodemid = new struct latlngheading;
            double heading = getHeading(nodestart->lng, nodestart->lat, nodeend->lng, nodeend->lat);
            computerLonLat(nodestart->lng, nodestart->lat, heading, vehiclewidth / 2, &(nodemid->lng), &(nodemid->lat));
            nodestop->heading = ((int)(heading + headfix)%360);
            computerLonLat(nodemid->lng, nodemid->lat, nodestop->heading, vehiclelength / 2, &(nodestop->lng), &(nodestop->lat));
            stoplist.append(nodestop);

            memmove(nodestart, nodeend, sizeof(struct latlngheading));
            delete nodemid;
            delete nodeend;
            nodeend = NULL;
        }
    }

    delete nodestart;
    return stoplist;
}
bool generateBoundaryLpxFile(QString filename, QList<collectlatlngheading *> list,char areaside)
{
    QFile file;
    file.setFileName(filename);

    QDir dir;
    dir.setPath(filename.left(filename.lastIndexOf("/")));
    if(!dir.exists())
    {
        dir.mkdir(filename.left(filename.lastIndexOf("/")));
    }

    if(file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        for(int count = 0; count < list.size(); count++)
        {
            QString curStr = QString("$,%1,%2,%3,%4,%5,0B,,,").arg(QString::number(list.at(count)->heading, 'f', 2))\
                                                  .arg(QString::number(list.at(count)->lat, 'f', 7))\
                                                  .arg(QString::number(list.at(count)->lng, 'f', 7))\
                                                  .arg(QString::number(list.at(count)->height, 'f', 2))\
                                                  .arg(QString::number(areaside, 10));
            char xorsum = xorCheck(curStr.right(curStr.length() - 1).toLatin1().data(), curStr.right(curStr.length() - 1).toLatin1().size());
            curStr.append("*" + QString::number(xorsum, 16) +"\r\n");
            file.write(curStr.toLatin1());
        }

        file.close();
        return true;
    }
    else
    {
        return false;
    }
}
bool generatePathLpxFile(QString filename, QList<collectlatlngheading *> list)
{
    QFile file;
    file.setFileName(filename);

    QDir dir;
    dir.setPath(filename.left(filename.lastIndexOf("/")));
    if(!dir.exists())
    {
        dir.mkdir(filename.left(filename.lastIndexOf("/")));
    }

    if(file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        for(int count = 0; count < list.size(); count++)
        {
            QString curStr = QString("$,%1,%2,%3,,,,,,,,,").arg(QString::number(list.at(count)->heading, 'f', 2))\
                                                  .arg(QString::number(list.at(count)->lat, 'f', 7))\
                                                  .arg(QString::number(list.at(count)->lng, 'f', 7));
            char xorsum = xorCheck(curStr.right(curStr.length() - 1).toLatin1().data(), curStr.right(curStr.length() - 1).toLatin1().size());
            curStr.append("*" + QString::number(xorsum, 16) +"\r\n");
            file.write(curStr.toLatin1());
        }

        file.close();
        return true;
    }
    else
    {
        return false;
    }
}

bool generateRoadLpxFile(QString filename, QList<latlngheading *> list)
{
    QFile file;
    file.setFileName(filename);

    QDir dir;
    dir.setPath(filename.left(filename.lastIndexOf("/")));
    if(!dir.exists())
    {
        dir.mkdir(filename.left(filename.lastIndexOf("/")));
    }

    if(file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        for(int count = 0; count < list.size(); count++)
        {
            QString curStr = QString("$,%1,%2,%3,,,,,,,,,").arg(QString::number(list.at(count)->heading, 'f', 2))\
                                                  .arg(QString::number(list.at(count)->lat, 'f', 7))\
                                                  .arg(QString::number(list.at(count)->lng, 'f', 7));
            char xorsum = xorCheck(curStr.right(curStr.length() - 1).toLatin1().data(), curStr.right(curStr.length() - 1).toLatin1().size());
            curStr.append("*" + QString::number(xorsum, 16) +"\r\n");
            file.write(curStr.toLatin1());
        }

        file.close();
        return true;
    }
    else
    {
        return false;
    }
}
