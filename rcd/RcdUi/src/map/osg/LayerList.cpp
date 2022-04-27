#include "LayerList.h"

LayerList::LayerList()
{
}


LayerList::~LayerList()
{
}

void LayerList::addLayer(Layer* layer)
{
	RenderGroup::addChild((osg::Group*)layer);
	m_layerList.push_back(layer);

}


void LayerList::removeLayer(Layer* layer)
{
	m_layerList.removeOne(layer);
	RenderGroup::removeChild((osg::Group*)layer);
	layer = nullptr;
}

Layer* LayerList::getLayer(int index)
{
	if (index > m_layerList.size())
	{
		return nullptr;
	}
	return m_layerList[index];
}

int LayerList::getNumLayer()
{
	return m_layerList.size();
}
