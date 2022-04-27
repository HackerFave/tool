#include "HDBoundaryLayerList.h"
#include "HDBoundaryLayer.h"



HDBoundaryLayerList::HDBoundaryLayerList()
{
}


HDBoundaryLayerList::~HDBoundaryLayerList()
{
}

HDBoundaryLayer* HDBoundaryLayerList::GetBoundaryLayer(int id, int type)
{
	//int num = getNumChildren();
	for (int i = 0; i < getNumChildren(); i++)
	{
		HDBoundaryLayer* layer = (HDBoundaryLayer*)getLayer(i);
		if (layer && layer->getRecId() == id && layer->getRecType() == type)
		{
			return layer;
		}

	}
	return nullptr;
}
