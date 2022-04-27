#include "pathframe.h"

PathFrame::PathFrame(QList<latlngheading *>* datalist)
{
    //nodeList = new QList<latlngheading*>;
    nodeList = datalist;

    latmin = -1;
    latmax = -1;
    lngmin = -1;
    lngmax = -1;

    if(nodeList != NULL)
    {
        if(nodeList->size() > 0)
        {
            latmin = nodeList->at(0)->lat;
            latmax = nodeList->at(0)->lat;
            lngmin = nodeList->at(0)->lng;
            lngmax = nodeList->at(0)->lng;
        }

        int count = 1;
        while(count < nodeList->size())
        {
            if(nodeList->at(count)->lat < latmin)
            {
                latmin = nodeList->at(count)->lat;
            }

            if(nodeList->at(count)->lat > latmax)
            {
                latmax = nodeList->at(count)->lat;
            }

            if(nodeList->at(count)->lng < lngmin)
            {
                lngmin = nodeList->at(count)->lng;
            }

            if(nodeList->at(count)->lng > lngmax)
            {
                lngmax = nodeList->at(count)->lng;
            }

            count++;
        }
    }
}

PathFrame::~PathFrame()
{
    if(!nodeList->isEmpty())
        nodeList->clear();
}

