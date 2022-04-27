#include "HDLaneLayerList.h"
#include "LaneInfo.h"

HDLaneLayerList::HDLaneLayerList()
{
}


HDLaneLayerList::~HDLaneLayerList()
{
}

QVector<HDLaneLayer*> HDLaneLayerList::findLaneLayerByRoadElementId(int id)
{
	return findLaneLayer(id, 0);

}

QVector<HDLaneLayer*> HDLaneLayerList::findLaneLayerByBoundaryIdAndType(int id, int type)
{
	return findLaneLayer(id, type);
}

HDLaneLayer* HDLaneLayerList::findLaneLayerById(int id)
{
	for (int i = 0; i < m_layerList.size(); i++)
	{
		if (m_layerList[i]->asLaneLayer())
		{
			HDLaneLayer* layer = m_layerList[i]->asLaneLayer();
			if (layer->getLaneInfo()->getLaneId() == id)
			{
				return layer;
			}
		}
	}
	return nullptr;
}

int HDLaneLayerList::findLaneIdByPreNodeid(int nodeid)
{
	for (int i = 0; i < m_layerList.size(); i++)
	{
		if (m_layerList[i]->asLaneLayer())
		{
			HDLaneLayer* layer = m_layerList[i]->asLaneLayer();
			if (layer->getLaneInfo()->getPreNodeId() == nodeid)
			{
				return layer->getLaneInfo()->getLaneId();
			}
		}
	}
	return -1;
}

int HDLaneLayerList::findLaneIdBySucNodeid(int nodeid)
{
	for (int i = 0; i < m_layerList.size(); i++)
	{
		if (m_layerList[i]->asLaneLayer())
		{
			HDLaneLayer* layer = m_layerList[i]->asLaneLayer();
			if (layer->getLaneInfo()->getSucNodeId() == nodeid)
			{
				return layer->getLaneInfo()->getLaneId();
			}
		}
	}
	return -1;
}

QVector<HDLaneLayer*> HDLaneLayerList::findLaneLayerByPreNodeid(int id)
{
	QVector<HDLaneLayer*> layerList;
	for (int i = 0; i < m_layerList.size(); i++)
	{
		if (m_layerList[i]->asLaneLayer())
		{
			HDLaneLayer* layer = m_layerList[i]->asLaneLayer();
			if (layer->getLaneInfo()->getPreNodeId() == id)
			{
				layerList.push_back(layer);
			}
		}
	}
	return layerList;
}

QVector<HDLaneLayer*> HDLaneLayerList::findLaneLayerBySucNodeid(int id)
{
	QVector<HDLaneLayer*> layerList;
	for (int i = 0; i < m_layerList.size(); i++)
	{
		if (m_layerList[i]->asLaneLayer())
		{
			HDLaneLayer* layer = m_layerList[i]->asLaneLayer();
			if (layer->getLaneInfo()->getSucNodeId() == id)
			{
				layerList.push_back(layer);
			}
		}
	}
	return layerList;
}

QVector<HDLaneLayer*> HDLaneLayerList::findLaneLayer(int id, int type)
{
	QVector<HDLaneLayer*> layerList;
	for (int i = 0; i < m_layerList.size(); i++)
	{
		if (m_layerList[i]->asLaneLayer())
		{
			HDLaneLayer* layer = m_layerList[i]->asLaneLayer();
			if (layer->getLaneInfo()->getElementId() == id && layer->getLaneInfo()->getElementType() == type)
			{
				layerList.push_back(layer);
			}
		}
	}
	return layerList;
}

