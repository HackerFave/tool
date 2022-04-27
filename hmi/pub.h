#ifndef PUB
#define PUB

#include <QDebug>

#include <QDir>
#include <QFile>

#include <stdlib.h>
#include <unistd.h>

#include <QDateTime>
#include <QThread>

#include <QList>

#include <cmath>
#include <cstdlib>

#include "./HmiFunction/hmicmdst.h"
#include "HmiFunction/paraparsing.h"

#define real2pix    60
#define pix2real    10
#define equator     1113222.2222
//#define M_PI        3.1415926

#define TEST    false

enum MapUpdateMode
{
    MapUpdateMode_FASLE = 0,
    MapUpdateMode_MD5_SAME,
    MapUpdateMode_NEW_FILE
};

double radian(double d);

void computerLonLat(double lon, double lat, double brng, double dist, double *eLon, double *eLat);

void calcLonLat(double lng, double lat, double angle, double dist, double *eLng, double *eLat);

double offSetDiffAngle(float loadInAngle, double vehicleWidth, double vehicleLength);

double haversine(double lon1, double lat1, double lon2, double lat2);

double getDistance(double lon1, double lat1, double lon2, double lat2);

double getHeading(double lonStart, double latStart, double lonEnd, double latEnd);

char xorCheck(char *buf, int len);

bool boundaryDataFixFun(QList<collectlatlngheading* >* list, double fixdirect, double fixvalue);

bool boundaryDataFun(QList<latlngheading* >* list);

bool addPosDataFun(QList<struct latlngheading *> *list, struct latlngheading&);

void addCollectPosDataFun(QList<struct collectlatlngheading *> *list);

QList<latlngheading* > computerStoplist(QList<latlngheading* > list , double headfix, double vehiclewidth, double vehiclelength);

bool generateBoundaryLpxFile(QString filename, QList<collectlatlngheading* > list,char areaside);

bool generateRoadLpxFile(QString filename, QList<latlngheading* > list);

bool generatePathLpxFile(QString filename, QList<collectlatlngheading* > list);

#endif // PUB

