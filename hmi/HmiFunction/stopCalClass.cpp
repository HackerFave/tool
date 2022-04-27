#include "stopCalClass.h"
#include "posinfo.h"
#include "hmicmdst.h"
#include "paraparsing.h"
#include "pub.h"
#include "log.h"
#include <cstring>
#include <QDateTime>

uint8_t stopCalClass::calcLoadStopPointCnt = 0;
struct latlngheading stopCalClass::tmpAvPos = {0, 0, 0};
struct latlngheading stopCalClass::avPos = {0, 0, 0};
bool stopCalClass::stopOk = false;
bool stopCalClass::avPosOk = false;

stopCalClass::stopCalClass(QObject *parent) : QObject(parent)
{

}

bool stopCalClass::stopCalFun(struct latlngheading *ret, float heading)
{
    double stopLength = ParaParsing::instance()->paraSt.excavator.stopLength;
    double stopWidth = ParaParsing::instance()->paraSt.excavator.stopWidth;
    double stopLat = ParaParsing::instance()->paraSt.excavator.stopLat;
    double stopLng = ParaParsing::instance()->paraSt.excavator.stopLng;
    float  stopHeading = ParaParsing::instance()->paraSt.excavator.stopHeading;
    bool   stopFlag = ParaParsing::instance()->paraSt.excavator.fixedPoint;

    if(stopFlag == true) {
        ret->lat = stopLat;
        ret->lng = stopLng;
        ret->heading = stopHeading;
        LOG_DEBUG("fixedPoint true lat[%f], lng[%f], heading[%f]\n", ret->lat, ret->lng, ret->heading);
        return true;
    }

    if(PosInfo::getHandle()->getWarnningBit(gnss))
    {
        return false;
    }

    struct latlngheading cur, tmp;
    PosInfo::getHandle()->getCurPos(&cur);
    if(cur.heading==0 || cur.lat ==0 || cur.lng==0) {
        return false;
    }

    tmp.heading = cur.heading + 90.0;
    if(tmp.heading > 360.0) {
        tmp.heading -= 360.0;
    }

    computerLonLat(cur.lng, cur.lat, cur.heading, stopLength, &tmp.lng, &tmp.lat);
    computerLonLat(tmp.lng, tmp.lat, tmp.heading, stopWidth, &ret->lng, &ret->lat);
    ret->heading = cur.heading + heading;
    if(ret->heading > 360.0) {
        ret->heading -= 360.0;
    } else if(ret->heading < 0.0) {
        ret->heading += 360.0;
    }
    LOG_DEBUG("fixPoint false lat[%f], lng[%f], heading[%f]\n", ret->lat, ret->lng, ret->heading);
    return true;
}

bool stopCalClass::stopCalAvOffsetFun(struct latlngheading *ret, float stopHeadingCompAngle)
{
    ParaParsing* para = ParaParsing::instance();
    para->excavatorParsing();
    para->minetrunckParsing();

    double stopLength = para->paraSt.excavator.stopLength;
    double stopWidth = para->paraSt.excavator.stopWidth;
    double stopLat = para->paraSt.excavator.stopLat;
    double stopLng = para->paraSt.excavator.stopLng;
    float  stopHeading = para->paraSt.excavator.stopHeading;
    bool   stopFlag = para->paraSt.excavator.fixedPoint;
    double vehicleWidth = para->paraSt.minetrunck.vehicleWidth;
    double vehicleLength = para->paraSt.minetrunck.vehicleLength;

    LOG_DEBUG("cfg: offlen[%f], offwidth[%f], fix[%d], vehicleWidth[%.1f], vehicleLength[%.1f],stopHeading[%.1f],stopHeadingCompAngle[%.1f]\n",
                  stopLength, stopWidth, stopFlag, vehicleWidth, vehicleLength,stopHeading,stopHeadingCompAngle);

    if(stopFlag == true) {
        ret->lat = stopLat;
        ret->lng = stopLng;
        ret->heading = stopHeading;
        stopOk = true;
        LOG_DEBUG("fixedPoint true lat[%.7f], lng[%.7f], heading[%.3f]\n", ret->lat, ret->lng, ret->heading);
        return true;
    }

    if(!avPosOk)
    {
        stopOk = false;
        return false;
    }

    struct latlngheading tmp;
    std::memmove(&tmp, &avPos, sizeof(latlngheading));

    double tmp1lng;
    double tmp1lat;
    double tmp1heading, tmp2heading;
    double lng;
    double lat;
    double heading;
    double yoffset;
    //LOG_DEBUG("curAvPos stopHeadingCompAngle[%.7f], vehicleWidth[%.7f], vehicleLength[%.3f]\n", stopHeadingCompAngle, vehicleWidth, vehicleLength);
    yoffset = offSetDiffAngle(stopHeadingCompAngle, vehicleWidth, vehicleLength);
    LOG_DEBUG("curAvPos lat[%.7f], lng[%.7f], heading[%.3f], yoffset[%.3f]\n", tmp.lat, tmp.lng, tmp.heading, yoffset);
    if (stopWidth >= 0)
    {
        tmp1heading = tmp.heading+270.0;
    }
    else
    {
        tmp1heading = tmp.heading+90.0;
        stopWidth = -stopWidth;
    }
    if(tmp1heading > 360.0) {
        tmp1heading -= 360.0;
    }

    if (stopLength >= 0)
    {
        tmp2heading = tmp.heading;
    }
    else
    {
        tmp2heading = tmp.heading + 180.0;
        stopLength = -stopLength;
    }
    if(tmp2heading > 360.0) {
        tmp2heading -= 360.0;
    }

    if(stopWidth<0.11)
    {
        tmp1lng=tmp.lng;
        tmp1lat=tmp.lat;
    }else
    {
        calcLonLat(tmp.lng, tmp.lat, tmp1heading, stopWidth, &tmp1lng, &tmp1lat);
    }

    //LOG_DEBUG("calPos1 lat[%.7f], lng[%.7f], heading[%.3f]\n", tmp1lat, tmp1lng, heading);
    if(stopLength+yoffset<0.11)
    {
        lng=tmp1lng;
        lat=tmp1lat;
    }else
    {
        calcLonLat(tmp1lng, tmp1lat, tmp2heading, stopLength + yoffset, &lng, &lat);
    }
    //LOG_DEBUG("calPos2 lat[%.7f], lng[%.7f], heading[%.3f]\n", lat, lng, stopHeadingCompAngle);
    heading = tmp.heading + stopHeadingCompAngle;
    if(heading < 0)
    {
        heading += 360.0;
    }
    else if(heading > 360.0)
    {
        heading -= 360.0;
    }

    ret->lat = lat;
    ret->lng = lng;
    ret->heading = heading;
    stopOk = true;
    LOG_DEBUG("calPos lat[%.7f], lng[%.7f], heading[%.3f]\n", lat, lng, heading);

    return true;
}

void stopCalClass::averPosFun(latlngheading *pos)
{
//    if(PosInfo::getHandle()->getWarnningBit(gnss))
    if(PosInfo::getHandle()->getGpsState() != 2)
    {
        std::memset(&tmpAvPos, 0, sizeof(latlngheading));
        std::memset(&avPos, 0, sizeof(latlngheading));
        calcLoadStopPointCnt = 0;
        avPosOk = false;
        return;
    }

    if((pos->heading == 0) && (pos->lat == 0) && (pos->lng == 0))
    {
        std::memset(&tmpAvPos, 0, sizeof(latlngheading));
        std::memset(&avPos, 0, sizeof(latlngheading));
        calcLoadStopPointCnt = 0;
        avPosOk = false;
        return;
    }

    tmpAvPos.heading += pos->heading;
    tmpAvPos.lat += pos->lat;
    tmpAvPos.lng += pos->lng;
    calcLoadStopPointCnt++;

    if (calcLoadStopPointCnt == POS_AVER_TIMES)
    {
        std::memmove(&avPos, &tmpAvPos, sizeof(latlngheading));
        std::memset(&tmpAvPos, 0, sizeof(latlngheading));
        calcLoadStopPointCnt = 0;

        avPos.heading /= POS_AVER_TIMES;
        avPos.lat /= POS_AVER_TIMES;
        avPos.lng /= POS_AVER_TIMES;
        avPosOk = true;
    }
}
