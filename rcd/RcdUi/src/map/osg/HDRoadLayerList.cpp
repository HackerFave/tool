#include "HDRoadLayerList.h"
#include "HDRoadLayer.h"



HDRoadLayerList::HDRoadLayerList()
{
}


HDRoadLayerList::~HDRoadLayerList()
{
}




HDRoadLayer* HDRoadLayerList::getLayerById(int id)
{
	for (int i = 0; i < getNumLayer(); i++)
	{
		HDRoadLayer* roadlayer = getLayer(i)->asRoadLayer();
		if (roadlayer && roadlayer->getElementID() == id)
		{
			return roadlayer;
		}
	}
	return nullptr;
}

