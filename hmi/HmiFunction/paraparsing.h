#ifndef PARAPARSING_H
#define PARAPARSING_H
#include <QMutex>
#include <QSettings>
#include <QDebug>
#include "hmicmdst.h"

#define PARA_PUBLIC_PATH "/conf/hmi/pubic.ini"
#define PARA_EXCAVATOR_PATH "/conf/hmi/excavator.ini"
#define PARA_CRUSHER_PATH "/conf/hmi/crusher.ini"
#define PARA_MINETRUNCK_PATH "/conf/hmi/minetrunck.ini"
#define PARA_DIGGER_PATH "/conf/hmi/digger.ini"
#define PARA_COLLECTOR_PATH "/conf/hmi/collector.ini"

class ParaParsing
{
public:
    ~ParaParsing();
    static ParaParsing* instance();
    void pubParsing(void);
    void excavatorParsing(void);
    void crusherParsing(void);
    void minetrunckParsing(void);
    void diggerParsing(void);
    void collectorParsing(void);
    void parkingParsing(void);

    void writedigger(double fixvalue, double length, double width);
    void setPara(QString filePath,QString key,QString value);

    struct paraST paraSt;
private:
    ParaParsing();
    ParaParsing & operator=(const ParaParsing & ) = delete;
    ParaParsing(const ParaParsing &) = delete;
    static ParaParsing *handle;
    static QMutex paraMutex;
};

#endif // PARAPARSING_H
